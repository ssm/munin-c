/*
 * Copyright (C) 2008-2013 Helmut Grohne <helmut@subdivi.de> - All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License v.2 or v.3.
 */

/* This plugin is compatible with munin-mainline version 2.0.17. */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "common.h"
#include "plugins.h"

int forks(int argc, char **argv) {
	FILE *f;
	char buff[256];
	if(argc > 1) {
		if(!strcmp(argv[1], "config")) {
			puts("graph_title Fork rate\n"
				"graph_args --base 1000 -l 0 \n"
				"graph_vlabel forks / ${graph_period}\n"
				"graph_category processes\n"
				"graph_info This graph shows the forking rate (new processes started).\n"
				"forks.label forks\n"
				"forks.type DERIVE\n"
				"forks.min 0\n"
				"forks.max 100000\n"
				"forks.info The number of forks per second.");
			print_warncrit("forks");
			return 0;
		}
		if(!strcmp(argv[1], "autoconf"))
			return autoconf_check_readable(PROC_STAT);
	}
	if(!(f=fopen(PROC_STAT, "r")))
		return fail("cannot open " PROC_STAT);
	while(fgets(buff, 256, f)) {
		if(!strncmp(buff, "processes ", 10)) {
			fclose(f);
			printf("forks.value %s", buff+10);
			return 0;
		}
	}
	fclose(f);
	return fail("no processes line found in " PROC_STAT);
}
