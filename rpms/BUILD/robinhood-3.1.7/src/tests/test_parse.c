/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 * vim:expandtab:shiftwidth=4:tabstop=4:
 */
/*
 * Copyright (C) 2009,2015 CEA/DAM
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the CeCILL License.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license (http://www.cecill.info) and that you
 * accept its terms.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "config_parsing.h"
#include <errno.h>

int main(int argc, char **argv)
{
    config_file_t  config;
    char          *file;
    char          *errtxt;

    if ((argc > 1) && (argv[1]))
    {
        file = argv[1];
    }
    else
    {
        fprintf(stderr, "Usage %s <config_file>\n", argv[0]);
        exit(EINVAL);
    }

    /* Example of parsing */
    config = rh_config_ParseFile(file);

    printf("config_pointer = %p\n", config);

    if (config == NULL)
    {
        errtxt = rh_config_GetErrorMsg();
        fprintf(stderr, "Error parsing %s: %s\n", argv[1], errtxt);
        exit(EINVAL);
    }

    rh_config_Print(stdout, config);

    printf("Brownsing configuration:\n");

    int            i;
    char          *val_a;
    config_item_t  block, item;

    for (i = 0; i < rh_config_GetNbBlocks(config); i++)
    {

        int            j;
        char          *name;
        char          *val;
        bool           uniq = true;

        /* display item name */
        block = rh_config_GetBlockByIndex(config, i);

        printf("block %s\n", rh_config_GetBlockName(block));

        if ((val_a = rh_config_GetKeyValueByName(block, "fs_path", &uniq)) != NULL)
        {
            printf("%s.fs_path is defined and is %s: %s\n",
                   rh_config_GetBlockName(block), uniq?"unique":"not unique", val_a);
        }
        else
        {
            printf("%s.fs_path is not defined\n",
                   rh_config_GetBlockName(block));
        }

        /* browse block variables */
        for (j = 0; j < rh_config_GetNbItems(block); j++)
        {
            int            extra_args;
            char         **extra_arg_tab;
            int            k;

            item = rh_config_GetItemByIndex(block, j);

            if (rh_config_ItemType(item) == CONFIG_ITEM_VAR)
            {
                rh_config_GetKeyValue(item, &name, &val, &extra_args);
                if (extra_args)
                {
                    printf("> %s.%s = %s (", rh_config_GetBlockName(block),
                           name, val);
                    extra_args = rh_config_GetExtraArgs(item, &extra_arg_tab);
                    for (k = 0; k < extra_args; k++)
                    {
                        if (k == 0)
                            printf("%s", extra_arg_tab[k]);
                        else
                            printf(", %s", extra_arg_tab[k]);
                    }
                    printf(")\n");
                }
                else
                    printf("> %s.%s = %s\n", rh_config_GetBlockName(block),
                           name, val);
            }
            else if (rh_config_ItemType(item) == CONFIG_ITEM_BLOCK)
                printf("\tsub-block = %s\n", rh_config_GetBlockName(item));
            else
                printf("\tcomplex boolean expression\n");
        }
        printf("\n");
    }

    /* free and reload the file */
    rh_config_Free(config);

    config = rh_config_ParseFile(file);

    printf("config_pointer = %p\n", config);

    if (config == NULL)
    {
        errtxt = rh_config_GetErrorMsg();
        fprintf(stderr, "Error parsing %s twice: %s\n", argv[1], errtxt);
        exit(EINVAL);
    }

    rh_config_Print(stdout, config);
    rh_config_Free(config);

    exit(0);
}
