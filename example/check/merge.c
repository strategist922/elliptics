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

#include "config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include <errno.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

#include "../common.h"
#include "../hash.h"

static void *dnet_merge_process(void *data)
{
	struct dnet_check_worker *w = data;
	struct dnet_node *n = w->n;
	unsigned char id[DNET_ID_SIZE];
	int err;

	while (1) {
		pthread_mutex_lock(&dnet_check_file_lock);
		err = fread(id, DNET_ID_SIZE, 1, dnet_check_file);
		pthread_mutex_unlock(&dnet_check_file_lock);

		if (err != 1)
			break;

		dnet_log_raw(n, DNET_LOG_INFO, "merge: %s\n", dnet_dump_id_len(id, DNET_ID_SIZE));
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	return dnet_check_start(argc, argv, dnet_merge_process, 1);
}