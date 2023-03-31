#ifndef _APP_TYPES_H
#define _APP_TYPES_H

#include <stddef.h>

/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 * vim:expandtab:shiftwidth=4:tabstop=4:
 */
/*
 * Copyright (C) 2009 CEA/DAM
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the CeCILL License.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license (http://www.cecill.info) and that you
 * accept its terms.
 */
/**
 * \file   lustre_hsm_types.h
 * \brief  Defines data types for Lustre-HSM purpose.
 */

#include <sys/types.h>
#include <sys/param.h>
#include <errno.h>
#include <string.h>
#include "rbh_types.h"
#include "global_config.h"

#ifdef _LUSTRE
#include "lustre_extended_types.h"
#endif

#if (defined (_LUSTRE) && defined(_HAVE_FID))

#define FID_PK
typedef lustre_fid entry_id_t;

#define entry_id_equal( _p_id_1, _p_id_2 ) ( ((_p_id_1)->f_seq == (_p_id_2)->f_seq) && ((_p_id_1)->f_oid == (_p_id_2)->f_oid)  )

#else

#define INUM_PK
typedef struct __entry_id__
{
  uint64_t fs_key;
  ino_t inode;
} entry_id_t;

#define entry_id_equal( _p_id_1, _p_id_2 ) ( ((_p_id_1)->fs_key == (_p_id_2)->fs_key) && ((_p_id_1)->inode == (_p_id_2)->inode)  )

#endif

static int lmgr_gen_depth( void * tgt, const void * src )
{
    const char * fullpath = (char*)src;
    unsigned int * p_depth = (unsigned int *)tgt;

    const char     *curr;
    unsigned int   nb1;
    unsigned int   nb2;
    /* depth = number of '/' - 1 - depth of root fs.
     * E.g.: root="/mnt/lustre", path="/mnt/lustre/dir/foo", depth=4-2-1=1
     */

    nb1 = 0;
    curr = global_config.fs_path;
    while ( ( curr = strchr( curr, '/' ) ) )
    {
        curr++;
        nb1++;
    }

    nb2 = 0;
    curr = fullpath;
    while ( ( curr = strchr( curr, '/' ) ) )
    {
        curr++;
        nb2++;
    }

    (*p_depth) = nb2 - nb1 - 1;

   return 0;
}

typedef union {
    int  num;
    char txt[RBH_LOGIN_MAX];
} uidgid_u;
 

typedef struct __entry_info__ 
{
	entry_id_t 	parent_id;
	char 	name[RBH_NAME_MAX];
	unsigned int 	path_update;
	char 	fullpath[RBH_PATH_MAX];
	unsigned int 	depth;
	unsigned int 	dircount;
	uint64_t 	avgsize;
	uidgid_u 	uid;
	uidgid_u 	gid;
	uint64_t 	size;
	uint64_t 	blocks;
	unsigned int 	creation_time;
	unsigned int 	last_access;
	unsigned int 	last_mod;
	unsigned int 	last_mdchange;
	char 	type[32];
	unsigned short 	mode;
	unsigned int 	nlink;
	unsigned int 	md_update;
	bool 	invalid;
	char 	fileclass[1024];
	unsigned int 	class_update;
	stripe_info_t 	stripe_info;
	stripe_items_t 	stripe_items;
	char 	link[RBH_PATH_MAX];
	unsigned int 	rm_time;
	char const **sm_status;
	void **sm_info;
} entry_info_t;

#define ATTR_INDEX_parent_id 	0
#define ATTR_INDEX_name 	1
#define ATTR_INDEX_path_update 	2
#define ATTR_INDEX_fullpath 	3
#define ATTR_INDEX_depth 	4
#define ATTR_INDEX_dircount 	5
#define ATTR_INDEX_avgsize 	6
#define ATTR_INDEX_uid 	7
#define ATTR_INDEX_gid 	8
#define ATTR_INDEX_size 	9
#define ATTR_INDEX_blocks 	10
#define ATTR_INDEX_creation_time 	11
#define ATTR_INDEX_last_access 	12
#define ATTR_INDEX_last_mod 	13
#define ATTR_INDEX_last_mdchange 	14
#define ATTR_INDEX_type 	15
#define ATTR_INDEX_mode 	16
#define ATTR_INDEX_nlink 	17
#define ATTR_INDEX_md_update 	18
#define ATTR_INDEX_invalid 	19
#define ATTR_INDEX_fileclass 	20
#define ATTR_INDEX_class_update 	21
#define ATTR_INDEX_stripe_info 	22
#define ATTR_INDEX_stripe_items 	23
#define ATTR_INDEX_link 	24
#define ATTR_INDEX_rm_time 	25

#define ATTR_COUNT 26

#if ATTR_COUNT > 32
#error "Standard attribute index must fit in a 32bits mask"
#endif

#define ATTR_MASK_parent_id 	(1LL << 0)
#define ATTR_MASK_name 	(1LL << 1)
#define ATTR_MASK_path_update 	(1LL << 2)
#define ATTR_MASK_fullpath 	(1LL << 3)
#define ATTR_MASK_depth 	(1LL << 4)
#define ATTR_MASK_dircount 	(1LL << 5)
#define ATTR_MASK_avgsize 	(1LL << 6)
#define ATTR_MASK_uid 	(1LL << 7)
#define ATTR_MASK_gid 	(1LL << 8)
#define ATTR_MASK_size 	(1LL << 9)
#define ATTR_MASK_blocks 	(1LL << 10)
#define ATTR_MASK_creation_time 	(1LL << 11)
#define ATTR_MASK_last_access 	(1LL << 12)
#define ATTR_MASK_last_mod 	(1LL << 13)
#define ATTR_MASK_last_mdchange 	(1LL << 14)
#define ATTR_MASK_type 	(1LL << 15)
#define ATTR_MASK_mode 	(1LL << 16)
#define ATTR_MASK_nlink 	(1LL << 17)
#define ATTR_MASK_md_update 	(1LL << 18)
#define ATTR_MASK_invalid 	(1LL << 19)
#define ATTR_MASK_fileclass 	(1LL << 20)
#define ATTR_MASK_class_update 	(1LL << 21)
#define ATTR_MASK_stripe_info 	(1LL << 22)
#define ATTR_MASK_stripe_items 	(1LL << 23)
#define ATTR_MASK_link 	(1LL << 24)
#define ATTR_MASK_rm_time 	(1LL << 25)

static const field_info_t field_infos[]=
{
	{ "parent_id", 	DB_ID, 	-1, 	FREQ_ACCESS | INDEXED | DNAMES, offsetof(entry_info_t, parent_id) , -1, NULL }, 
	{ "name", 	DB_TEXT, 	RBH_NAME_MAX-1, 	FREQ_ACCESS | DNAMES, offsetof(entry_info_t, name) , -1, NULL }, 
	{ "path_update", 	DB_UINT, 	-1, 	FREQ_ACCESS | DNAMES, offsetof(entry_info_t, path_update) , -1, NULL }, 
	{ "fullpath", 	DB_TEXT, 	RBH_PATH_MAX-1, 	FUNC_ATTR | DNAMES, offsetof(entry_info_t, fullpath) , -1, NULL }, 
	{ "depth", 	DB_UINT, 	-1, 	GENERATED, offsetof(entry_info_t, depth) , ATTR_INDEX_fullpath, lmgr_gen_depth }, 
	{ "dircount", 	DB_UINT, 	-1, 	DIR_ATTR, offsetof(entry_info_t, dircount) , ATTR_INDEX_type, NULL }, 
	{ "avgsize", 	DB_BIGUINT, 	-1, 	DIR_ATTR, offsetof(entry_info_t, avgsize) , ATTR_INDEX_type, NULL }, 
	{ "uid", 	DB_UIDGID, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, uid) , -1, NULL }, 
	{ "gid", 	DB_UIDGID, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, gid) , -1, NULL }, 
	{ "size", 	DB_BIGUINT, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, size) , -1, NULL }, 
	{ "blocks", 	DB_BIGUINT, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, blocks) , -1, NULL }, 
	{ "creation_time", 	DB_UINT, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, creation_time) , -1, NULL }, 
	{ "last_access", 	DB_UINT, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, last_access) , -1, NULL }, 
	{ "last_mod", 	DB_UINT, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, last_mod) , -1, NULL }, 
	{ "last_mdchange", 	DB_UINT, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, last_mdchange) , -1, NULL }, 
	{ "type", 	DB_ENUM_FTYPE, 	31, 	FREQ_ACCESS, offsetof(entry_info_t, type) , -1, NULL }, 
	{ "mode", 	DB_USHORT, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, mode) , -1, NULL }, 
	{ "nlink", 	DB_UINT, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, nlink) , -1, NULL }, 
	{ "md_update", 	DB_UINT, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, md_update) , -1, NULL }, 
	{ "invalid", 	DB_BOOL, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, invalid) , -1, NULL }, 
	{ "fileclass", 	DB_TEXT, 	1023, 	FREQ_ACCESS | SEPD_LIST, offsetof(entry_info_t, fileclass) , -1, NULL }, 
	{ "class_update", 	DB_UINT, 	-1, 	FREQ_ACCESS, offsetof(entry_info_t, class_update) , -1, NULL }, 
	{ "stripe_info", 	DB_STRIPE_INFO, 	-1, 	INIT_ONLY, offsetof(entry_info_t, stripe_info) , -1, NULL }, 
	{ "stripe_items", 	DB_STRIPE_ITEMS, 	-1, 	INIT_ONLY, offsetof(entry_info_t, stripe_items) , -1, NULL }, 
	{ "link", 	DB_TEXT, 	RBH_PATH_MAX-1, 	ANNEX_INFO | SLINK_ATTR, offsetof(entry_info_t, link) , -1, NULL }, 
	{ "rm_time", 	DB_UINT, 	-1, 	REMOVED, offsetof(entry_info_t, rm_time), -1, NULL }
};

#endif
