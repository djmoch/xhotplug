/* See LICENSE file for copyright and license details. */

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/randr.h>

static char *argv0;

enum {
	ERR_NONE,
	ERR_UNKNOWN_OPTION,
	ERR_NUM_ARGS,
	ERR_NOT_EXECUTABLE,
	ERR_PLEDGE
};

void
usage(const int e_val) {
	printf("usage: %s [-h] [-v] script\n", argv0);
	exit(e_val);
}

void
xhotplug(const char *cmd) {
	xcb_connection_t* conn;
	xcb_screen_t* screen;
	xcb_window_t window;
	xcb_generic_event_t* evt;
	xcb_randr_screen_change_notify_event_t* randr_evt;
	xcb_timestamp_t last_time;
	int monitor_connected = 1;

	conn = xcb_connect(NULL, NULL);

	screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
	window = screen->root;
	xcb_randr_select_input(conn, window, XCB_RANDR_NOTIFY_MASK_SCREEN_CHANGE);
	xcb_flush(conn);

	while ((evt = xcb_wait_for_event(conn)) != NULL) {
		if (evt->response_type & XCB_RANDR_NOTIFY_MASK_SCREEN_CHANGE) {
			randr_evt = (xcb_randr_screen_change_notify_event_t*) evt;
			if (last_time != randr_evt->timestamp) {
				last_time = randr_evt->timestamp;
				monitor_connected = !monitor_connected;
				if (monitor_connected) {
					system(cmd);
				} else {
					system(cmd);
				}
			}
		}
		free(evt);
	}
	xcb_disconnect(conn);
}

int
can_execute(const char *path) {
	uid_t uid = getuid();
	gid_t gid = getgid();
	gid_t gidset[NGROUPS_MAX];
	struct stat sb;

	if (stat(path, &sb) != 0) {
		fprintf(stderr, "%s: ERROR -- Unable to stat file %s\n", argv0, path);
		fprintf(stderr, "%s: errno -- %d\n", argv0, errno);
		return 0;
	}
	if ((sb.st_mode & (S_IWGRP | S_IWOTH)) != 0) {
		fprintf(stderr, "%s: script is writeable by others\n", argv0);
		return 0;
	}
	if (sb.st_uid != uid && sb.st_uid != 0) {
		fprintf(stderr, "%s: script not owned by user or root\n", argv0);
		return 0;
	}
	if (sb.st_uid == uid && (sb.st_mode & S_IXUSR) == 0) {
		fprintf(stderr, "%s: script is not executable\n", argv0);
		return 0;
	}
	if (sb.st_uid == 0) {
		for (int i=0; i<getgroups(NGROUPS_MAX, gidset); i++) {
			if (gidset[i] == gid) {
				if ((sb.st_mode & S_IXGRP) == 0) {
					fprintf(stderr, "%s: script is not executable\n", argv0);
					return 0;
				}
				else {
					return 1;
				}
			}
		}
	}
	return 1;
}

int
main(int argc, char **argv) {
	char ch, script[PATH_MAX], sbuf[PATH_MAX];
	ssize_t sbuf_len;
	int pval;

	argv0 = basename(argv[0]);

	while ((ch = getopt(argc, argv, "vh")) != -1) {
		switch (ch) {
		case 'v':
			printf("%s %s\n", argv0, VERSION);
			exit(ERR_NONE);
			break;
		case 'h':
			usage(ERR_NONE);
			break;
		default:
			usage(ERR_UNKNOWN_OPTION);
		}
	}
	argc -= optind;
	argv += optind;

	if ( argc < 1 || argc > 1) {
		printf("%s: invalid number of arguments\n", argv0);
		usage(ERR_NUM_ARGS);
	}

	sbuf_len = strnlen(argv[0], PATH_MAX);
	strncpy(script, argv[0], PATH_MAX - 1);
	script[PATH_MAX - 1] = '\0';
	while ((sbuf_len = readlink(script, sbuf, PATH_MAX)) != -1) {
		sbuf[sbuf_len] = '\0';
		strncpy(script, sbuf, PATH_MAX - 1);
		script[PATH_MAX - 1] = '\0';
	}

	if(!can_execute(script)) {
		fprintf(stderr, "%s: cannot run provided script. quitting.\n", argv0);
		usage(ERR_NOT_EXECUTABLE);
	}

#ifdef __OpenBSD__
	if ((pval = pledge("stdio unix rpath proc exec", NULL)) != 0) {
		fprintf(stderr, "%s: call to pledge(2) failed -- %d. quitting.\n", argv0, pval);
		exit(ERR_PLEDGE);
	}
#endif

	xhotplug(script);
}
