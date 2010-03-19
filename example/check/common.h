/*
 * 2008+ Copyright (c) Evgeniy Polyakov <zbr@ioremap.net>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __DNET_CHECK_COMMON_H
#define __DNET_CHECK_COMMON_H

#include "dnet/packet.h"
#include "dnet/interface.h"

struct dnet_check_worker
{
	struct dnet_node			*n;

	int					id;
	pthread_t				tid;

	int					wait_num;
	int					object_present, object_missing;

	pthread_cond_t				wait_cond;
	pthread_mutex_t				wait_lock;
};

struct dnet_check_request
{
	unsigned char			id[DNET_ID_SIZE];
	unsigned char 			addr[DNET_ID_SIZE];
	unsigned int			type;
	unsigned int			present;
	int				pos;

	struct dnet_check_worker	*w;
};

int dnet_check_add_hash(struct dnet_node *n, char *hash);
int dnet_check_del_hash(struct dnet_node *n, char *hash);
int dnet_check_read_transactions(struct dnet_check_worker *worker, struct dnet_check_request *req);
int dnet_check_cleanup_transactions(struct dnet_check_worker *w, struct dnet_check_request *existing);

extern void *(* dnet_check_ext_init)(char *data);
extern void (* dnet_check_ext_exit)(void *priv);
extern int (* dnet_check_ext_merge)(void *priv, char *path, int start, int end,
		struct dnet_check_request *req, int num, int update_existing);
extern void *dnet_check_ext_private;
extern void *dnet_check_ext_library;

int dnet_check_start(int argc, char *argv[], void *(* process)(void *data), int check_file);

extern char dnet_check_tmp_dir[128];
extern FILE *dnet_check_file;
extern pthread_mutex_t dnet_check_file_lock;

#endif /* __DNET_CHECK_COMMON_H */