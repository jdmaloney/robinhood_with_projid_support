#!/bin/bash


[[ -e /etc/sysconfig/rbh_cksum ]] && . /etc/sysconfig/rbh_cksum

# "defaults" section
RBH_CKSUM_CMD=${RBH_CKSUM_CMD:-sha1sum}
RBH_CKSUM_DV_CMD=${RBH_CKSUM_DV_CMD:-lfs data_version}
# xattr not set if variable set empty, so allow empty value
RBH_CKSUM_XATTR=${RBH_CKSUM_XATTR-user.sha1sum}

usage() {
	echo "usage: $(readlink -m "$0") <previous_value> <path>" >&2
	exit 1
}


# data version helper. Use lfs iff file is on a luste filesystem
get_dv() {
	local path="$1"

	$RBH_CKSUM_DV_CMD "$path"
}

compute_cksum() {
	local path="$1"
	local cksum

	# run checksum program and take first word
	cksum=$($RBH_CKSUM_CMD "$path")
	echo "${cksum%% *}"
}


getfattr_output() {
	local path="$1"

	[[ -z "$RBH_CKSUM_XATTR" ]] && return

	getfattr -n "$RBH_CKSUM_XATTR" --only-values -- "$path" 2>/dev/null || true
}

setfattr_output() {
	local path="$1"
	local cksum="$2"

	[[ -z "$RBH_CKSUM_XATTR" ]] && return

	setfattr -n "$RBH_CKSUM_XATTR" -v "$cksum" -- "$path"
}


rbh_cksum() {
	local output="$1"
	local path="$2"
	local cksum
	local dv
	local cksum_old=""
	local dv_old
	local xattr_output
	local update_xattr=1


	[[ -z "$path" ]] && usage


	set -e
	set -u


	# compute data version, checksum, then data version again to compare
	dv_old=$(get_dv "$path")
	cksum=$(compute_cksum "$path")
	dv=$(get_dv "$path")

	if [[ "$dv" != "$dv_old" ]]; then
		echo "$path: data version changed during checksuming, try again later" >&2
		setfattr_output "$path" ""
		exit 0
	fi;

	if [[ -z "$dv" || -z "$cksum" ]]; then
		echo "$path: empty dv ($dv) or checksum ($cksum), aborting" >&2
		exit 1
	fi


	# if using xattrs, xattrs win over argument
	xattr_output=$(getfattr_output "$path")
	if [[ -n "$xattr_output" ]]; then
		output="$xattr_output"
		update_xattr=""
	fi

	# check if dv changed since last check
	if [[ -n "$output" ]]; then
		# arbitrary choice: dv can contain :, cksum cannot
		dv_old=${output%:*}

		if [[ "$dv" != "$dv_old" ]]; then
			echo "$path: dv changed since output, using new" >&2
			update_xattr=1
		else
			cksum_old=${output##*:}
		fi
	fi

	# compare with old cksum if relevant
	case "$cksum_old" in
		"")
			echo "$path: new cksum: $dv:$cksum" >&2
			;;
		"$cksum")
			echo "$path: cksum OK: $dv:$cksum" >&2
			;;
		*)
			echo "$path: checksum changed! dv $dv: old $cksum_old, new $cksum" >&2
			exit 1
	esac

	if [[ -n "$update_xattr" ]]; then
		setfattr_output "$path" "$dv:$cksum"
	fi

	echo "$dv:$cksum"
}

# if we're being sourced, don't parse arguments
[[ $(caller | cut -d' ' -f1) != "0" ]] && return

rbh_cksum "$@"
