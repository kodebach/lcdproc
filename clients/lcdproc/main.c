/** \file clients/lcdproc/main.c
 * Contains main(), plus signal callback functions and a help screen.
 *
 * Program init, command-line handling, and the main loop are
 * implemented here.
 */

/*-
 * This file is part of lcdproc, the lcdproc client.
 *
 * This file is released under the GNU General Public License.
 * Refer to the COPYING file distributed with this package.
 *
 * TODO (kodebach): Copyright
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <locale.h>
#include <signal.h>
#include <netdb.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/param.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "main.h"
#include "mode.h"
#include "shared/sockets.h"
#include "shared/report.h"
#include "getopt.h"		/* This is our local getopt.h! */

/* Import screens */
#include "batt.h"
#include "chrono.h"
#include "cpu.h"
#include "cpu_smp.h"
#include "disk.h"
#include "load.h"
#include "mem.h"
#include "machine.h"
#include "iface.h"
#ifdef LCDPROC_EYEBOXONE
# include "eyebox.h"
#endif

/* The following 8 variables are defined 'external' in main.h! */
int Quit = 0;
int sock = -1;

char *version = VERSION;

int lcd_wid = 0;
int lcd_hgt = 0;
int lcd_cellwid = 0;
int lcd_cellhgt = 0;

static struct utsname unamebuf;

/* local prototypes */
static void HelpScreen(int exit_state);
static void exit_program(int val);
static void main_loop(void);
static int process_command_line(int argc, char **argv);
static int process_configfile(char *cfgfile);


#define TIME_UNIT	125000	/**< 1/8th second is a single time unit. */

#if !defined(SYSCONFDIR)
# define SYSCONFDIR	"/etc"
#endif
#if !defined(PIDFILEDIR)
# define PIDFILEDIR	"/var/run"
#endif

#define UNSET_INT		-1
#define UNSET_STR		"\01"
#define DEFAULT_SERVER		"127.0.0.1"
#define DEFAULT_CONFIGFILE	SYSCONFDIR "/lcdproc.conf"
#define DEFAULT_PIDFILE		PIDFILEDIR "/lcdproc.pid"
#define DEFAULT_REPORTDEST	RPT_DEST_STDERR
#define DEFAULT_REPORTLEVEL	RPT_WARNING

/** list of screen modes to run */
ScreenMode sequence[] =
{
	/* flags default ACTIVE will run by default */
	/* longname    which on  off inv  timer   flags */
	{ "CPU",       'C', "cpu",	      1,    2, 0, 0xffff, ACTIVE, cpu_screen        },	// [C]PU
	{ "Iface",     'I', "ifcace",     1,    2, 0, 0xffff, 0,      iface_screen      }, 	// [I]face
	{ "Memory",    'M', "memory",     4,   16, 0, 0xffff, ACTIVE, mem_screen        },	// [M]emory
	{ "Load",      'L', "load",      64,  128, 1, 0xffff, ACTIVE, xload_screen      },	// [L]oad (load histogram)
	{ "TimeDate",  'T', "timedate",   4,   64, 0, 0xffff, ACTIVE, time_screen       },	// [T]ime/Date
	{ "About",     'A', "about",    999, 9999, 0, 0xffff, ACTIVE, credit_screen     },	// [A]bout (credits)
	{ "SMP-CPU",   'P', "smpcpu",     1,    2, 0, 0xffff, 0,      cpu_smp_screen    },	// CPU_SM[P]
	{ "OldTime",   'O', "oldtime",    4,   64, 0, 0xffff, 0,      clock_screen      },	// [O]ld Timescreen
	{ "BigClock",  'K', "bigclock",   4,   64, 0, 0xffff, 0,      big_clock_screen  },	// big cloc[K]
	{ "Uptime",    'U', "uptime",     4,  128, 0, 0xffff, 0,      uptime_screen     },	// Old [U]ptime Screen
	{ "Battery",   'B', "battery",   32,  256, 1, 0xffff, 0,      battery_screen    },	// [B]attery Status
	{ "CPUGraph",  'G', "cpugraph",   1,    2, 0, 0xffff, 0,      cpu_graph_screen  },	// CPU histogram [G]raph
	{ "ProcSize",  'S', "procsize",  16,  256, 1, 0xffff, 0,      mem_top_screen    },	// [S]ize of biggest processes
	{ "Disk",      'D', "disk",     256,  256, 1, 0xffff, 0,      disk_screen       },	// [D]isk stats
	{ "MiniClock", 'N', "miniclock",  4,   64, 0, 0xffff, 0,      mini_clock_screen },	// Mi[n]i clock
	{  NULL, 0, 0, 0, 0, 0, 0, NULL},			  	// No more..  all done.
};


/* All variables are set to 'unset' values */
static int islow = -1;		/**< pause after mode update (in 1/100s) */
char *progname = "lcdproc";
char *server = NULL;
int port = UNSET_INT;
int foreground = FALSE;
static int report_level = UNSET_INT;
static int report_dest = UNSET_INT;
char *configfile = NULL;
char *pidfile = NULL;
int pidfile_written = FALSE;
char *displayname = NULL;	/**< display name for the main menu */

static KeySet* config = NULL; // TODO (kodebach): document

/** Returns the network name of this machine */
const char *
get_hostname(void)
{
	return (unamebuf.nodename);
}

/** Returns the name of the client's OS */
const char *
get_sysname(void)
{
	return (unamebuf.sysname);
}

/** Returns the release version of the client's OS */
const char *
get_sysrelease(void)
{
	return (unamebuf.release);
}


/** Enables or disables (and deletes) a screen */
static int
set_mode(int shortname, char *longname, int state)
{
	int k;

	for (k = 0; sequence[k].which != 0; k++) {
		if (((sequence[k].longname != NULL) &&
		     (0 == strcasecmp(longname, sequence[k].longname))) ||
		    (toupper(shortname) == sequence[k].which)) {
			if (!state) {
				/*
				 * clean both the active and initialized bits
				 * since we delete the screen
				 */
				sequence[k].flags &= (~ACTIVE & ~INITIALIZED);
				/* delete the screen if we are connected */
				if (sock >= 0) {
					sock_printf(sock, "screen_del %c\n", sequence[k].which);
				}
			}
			else
				sequence[k].flags |= ACTIVE;
			return 1;	/* found */
		}
	}
	return 0;		/* not found */
}


/** Sets all screens inactive */
static void
clear_modes(void)
{
	int k;

	for (k = 0; sequence[k].which != 0; k++) {
		sequence[k].flags &= (~ACTIVE);
	}
}


int
main(int argc, char **argv)
{
	/* set locale for cwdate & time formatting in chrono.c */
	setlocale(LC_TIME, "");

	/* get uname information */
	if (uname(&unamebuf) == -1) {
		perror("uname");
		return (EXIT_FAILURE);
	}

	/* setup error handlers */
	signal(SIGINT, exit_program);	/* Ctrl-C */
	signal(SIGTERM, exit_program);	/* "regular" kill */
	signal(SIGHUP, exit_program);	/* kill -HUP */
	signal(SIGPIPE, exit_program);	/* write to closed socket */
	signal(SIGKILL, exit_program);	/* kill -9 [cannot be trapped; but ...] */

	if (process_command_line(argc, argv) < 0) {
		report (RPT_ERR, "Error parsing command line arguments");
					exit(EXIT_FAILURE);
				}

	/* Read config file */
	int cfgresult = process_configfile(configfile);
	if (cfgresult < 0) {
		report (RPT_ERR, "Error reading config file\n");
		exit(EXIT_FAILURE);
	}

	/* Set default reporting options */
	if (report_dest == UNSET_INT)
		report_dest = DEFAULT_REPORTDEST;
	if (report_level == UNSET_INT)
		report_level = DEFAULT_REPORTLEVEL;

	/* Set reporting settings */
	set_reporting("lcdproc", report_level, report_dest);

	/* parse non-option arguments: modes to add/delete */
	if (argc > max(optind, 1)) {
		int i;

		/*
		 * if no config file was read, ignore hard coded default
		 * modes
		 */
		if (cfgresult == 0)
			clear_modes();

		/* turn additional options on or off (using ! as prefix) */
		for (i = max(optind, 1); i < argc; i++) {
			int state = (*argv[i] == '!') ? 0 : 1;
			char *name = (state) ? argv[i] : argv[i] + 1;
			int shortname = (strlen(name) == 1) ? name[0] : '\0';
			int found = set_mode(shortname, name, state);

			if (!found) {
				fprintf(stderr, "Invalid Screen: %s\n", name);
				return (EXIT_FAILURE);
			}
		}
	}

	if (server == NULL)
		server = DEFAULT_SERVER;

	/* Connect to the server... */
	sock = sock_connect(server, port);
	if (sock < 0) {
		fprintf(stderr, "Error connecting to LCD server %s on port %d.\n"
			"Check to see that the server is running and operating normally.\n",
			server, port);
		return (EXIT_FAILURE);
	}

	sock_send_string(sock, "hello\n");
	usleep(500000);		/* wait for the server to say hi. */

	/* We grab the real values below, from the "connect" line. */
	lcd_wid = 20;
	lcd_hgt = 4;
	lcd_cellwid = 5;
	lcd_cellhgt = 8;

	if (foreground != TRUE) {
		if (daemon(1, 0) != 0) {
			fprintf(stderr, "Error: daemonize failed\n");
			return (EXIT_FAILURE);
		}

		if (pidfile != NULL) {
			FILE *pidf = fopen(pidfile, "w");

			if (pidf) {
				fprintf(pidf, "%d\n", (int)getpid());
				fclose(pidf);
				pidfile_written = TRUE;
			}
			else {
				fprintf(stderr, "Error creating pidfile %s: %s\n",
					pidfile, strerror(errno));
				return (EXIT_FAILURE);
			}
		}
	}

	/* Init the status gatherers... */
	mode_init();

	/* And spew stuff! */
	main_loop();
	exit_program(EXIT_SUCCESS);

	/* NOTREACHED */
	return EXIT_SUCCESS;
}

/* get options from command line */
static int
process_command_line(int argc, char **argv)
{
	int c;
	int error = 0;

	/* No error output from getopt */
	opterr = 0;
	while ((c = getopt(argc, argv, "s:p:e:c:fhv")) > 0) {
		char *end;

		switch (c) {
			/* c is for config file */
			case 'c':
				configfile = optarg;
				break;
			/* s is for server */
			case 's':
				server = optarg;
				break;
			/* p is for port */
			case 'p':
				port = strtol(optarg, &end, 0);
				if ((*optarg == '\0') || (*end != '\0') ||
				    (port <= 0) || (port >= 0xFFFF)) {
					report(RPT_ERR, "Illegal port value %s\n", optarg);
					error = -1;
				}
				break;
			case 'e':
				islow = strtol(optarg, &end, 0);
				if ((*optarg == '\0') || (*end != '\0') || (islow < 0)) {
					report(RPT_ERR, "Illegal delay value %s\n", optarg);
					error = -1;
				}
				break;
			case 'f':
				foreground = TRUE;
				break;
			case 'h':
				HelpScreen(EXIT_SUCCESS);
				break;
			case 'v':
				fprintf(stderr, "LCDproc %s\n", version);
				exit(EXIT_SUCCESS);
				break;
			/* otherwise...  Get help! */
			case '?':	/* unknown option or missing argument */
				/* FALLTHROUGH */
			default:
				HelpScreen(EXIT_FAILURE);
				break;
		}
	}

	return error;
}

/**
 * Reads and parses configuration file.
 * \param configfile  The configfile to read or NULL for the default config.
 * \retval 1 if configfile was read,
 * \retval 0 if default configfile doesn't exist
 * \retval <0 on error
 */
static int
process_configfile(char *configfile)
{
	debug(RPT_DEBUG, "%s(%s)", __FUNCTION__, (configfile) ? configfile : "<null>");

	/* Read config settings */

// TODO (kodebach)
//	if (configfile == NULL) {
//		struct stat statbuf;
//
//		/*
//		 * if default config file does not exist, do not consider
//		 * this an error and continue
//		 */
//		if ((lstat(DEFAULT_CONFIGFILE, &statbuf) == -1) && (errno = ENOENT))
//			return 0;
//
//		configfile = DEFAULT_CONFIGFILE;
//	}

	if(configfile != NULL && strcmp(configfile, DEFAULT_CONFIGFILE) != 0) {
		report( RPT_ERR, "currently unsupported"); // TODO (kodebach): support
		return -1;
	}

	config = econfig_open(CONFIG_BASE_KEY);

	if (server == NULL) {
		server = econfig_get_string(config, CONFIG_BASE_KEY"/general/server", DEFAULT_SERVER);
	}
	if (port == UNSET_INT) {
		port = econfig_get_long(config, CONFIG_BASE_KEY"/general/port", LCDPORT);
	}

	if (report_level == UNSET_INT) {
		report_level = econfig_get_long(config, CONFIG_BASE_KEY"/general/reportlevel", RPT_WARNING);
	}
	if (report_dest == UNSET_INT) {
		if (econfig_get_bool(config, CONFIG_BASE_KEY"/general/reporttosyslog", false)) {
			report_dest = RPT_DEST_SYSLOG;
		}
		else {
			report_dest = RPT_DEST_STDERR;
		}
	}
	if (foreground != TRUE) {
		foreground = econfig_get_bool(config, CONFIG_BASE_KEY"/general/foreground", false);
	}
	if (pidfile == NULL) {
		pidfile = econfig_get_string(config, CONFIG_BASE_KEY"/general/pidfile", DEFAULT_PIDFILE);
	}
	if (islow < 0) {
		islow = econfig_get_long(config, CONFIG_BASE_KEY"/general/delay", -1);
	}

	displayname = econfig_get_string(config, CONFIG_BASE_KEY"/general/displayname", displayname);

	/*
	 * check for config file variables to override all the sequence
	 * defaults
	 */
	Key* nameKey = keyNew(CONFIG_BASE_KEY"/screenmode", KEY_END); // only used for string manipulation
	keyAddBaseName(nameKey, "(longname)");
	for (int k = 0; sequence[k].which != 0; k++) {
		if (sequence[k].configname != NULL) {
			keySetBaseName(nameKey, sequence[k].configname);

			keyAddBaseName(nameKey, "ontime");
			sequence[k].on_time = econfig_get_long(config, keyName(nameKey), sequence[k].on_time);

			keySetBaseName(nameKey, "offtime");
			sequence[k].off_time = econfig_get_long(config, keyName(nameKey), sequence[k].off_time);

			keySetBaseName(nameKey, "showinvisible");
			sequence[k].show_invisible = econfig_get_bool(config, keyName(nameKey), sequence[k].show_invisible);

			keySetBaseName(nameKey, "active");
			if (econfig_get_bool(config, keyName(nameKey), sequence[k].flags & ACTIVE)) {
				sequence[k].flags |= ACTIVE;
			}
			else {
				sequence[k].flags &= (~ACTIVE);
			}

			keySetBaseName(nameKey, NULL); // remove sequence[k].longname
		}
	}
	keyDel(nameKey);

	return 1;
}

/** Print help screen and exit */
void
HelpScreen(int exit_state)
{
	fprintf(stderr,
		"lcdproc - LCDproc system status information viewer\n"
		"\n"
		"Copyright (c) 1999-2017 Selene Scriven, William Ferrell, and misc. contributors.\n"
		"This program is released under the terms of the GNU General Public License.\n"
		"\n"
		"Usage: lcdproc [<options>] [<screens> ...]\n"
		"  where <options> are\n"
		"    -s <host>           connect to LCDd daemon on <host>\n"
		"    -p <port>           connect to LCDd daemon using <port>\n"
		"    -f                  run in foreground\n"
		"    -e <delay>          slow down initial announcement of screens (in 1/100s)\n"
		"    -c <config>         use a configuration file other than %s\n"
		"    -h                  show this help screen\n"
		"    -v                  display program version\n"
		"  and <screens> are\n"
		"    C CPU               detailed CPU usage\n"
		"    P SMP-CPU           CPU usage overview (one line per CPU)\n"
		"    G CPUGraph          CPU usage histogram\n"
		"    L Load              load histogram\n"
		"    M Memory            memory & swap usage\n"
		"    S ProcSize          biggest processes size\n"
		"    D Disk              filling level of mounted file systems\n"
		"    I Iface             network interface usage\n"
		"    B Battery           battery status\n"
		"    T TimeDate          time & date information\n"
		"    O OldTime           old time screen\n"
		"    U Uptime            uptime screen\n"
		"    K BigClock          big clock\n"
		"    N MiniClock         minimal clock\n"
		"    A About             credits page\n"
		"\n"
		"Example:\n"
		"    lcdproc -s my.lcdproc.server.com -p 13666 C M L\n"
		, DEFAULT_CONFIGFILE);

	exit(exit_state);
}


/** Called upon TERM and INTR signals. Also removes the pid-file. */
void
exit_program(int val)
{
	if (config != NULL) {
		econfig_close(config);
	}

#ifdef LCDPROC_EYEBOXONE
	/*
	 * Clear Eyebox Leds
	 */
	eyebox_clear();
#endif
	Quit = 1;
	sock_close(sock);
	mode_close();
	if ((foreground != TRUE) && (pidfile != NULL) && (pidfile_written == TRUE))
		unlink(pidfile);
	exit(val);
}

#ifdef LCDPROC_MENUS
int
menus_init()
{
	int k;

	for (k = 0; sequence[k].which; k++) {
		if (sequence[k].longname) {
			sock_printf(sock, "menu_add_item {} %c checkbox {%s} -value %s\n",
				    sequence[k].which, sequence[k].longname,
			       (sequence[k].flags & ACTIVE) ? "on" : "off");
		}
	}

#ifdef LCDPROC_CLIENT_TESTMENUS
	/*
	 * to be entered on escape from test_menu (but overwritten for
	 * test_{checkbox,ring}
	 */
	sock_send_string(sock, "menu_add_item {} ask menu {Leave menus?} -is_hidden true\n");
	sock_send_string(sock, "menu_add_item {ask} ask_yes action {Yes} -next _quit_\n");
	sock_send_string(sock, "menu_add_item {ask} ask_no action {No} -next _close_\n");
	sock_send_string(sock, "menu_add_item {} test menu {Test}\n");
	sock_send_string(sock, "menu_add_item {test} test_action action {Action}\n");
	sock_send_string(sock, "menu_add_item {test} test_checkbox checkbox {Checkbox}\n");
	sock_send_string(sock, "menu_add_item {test} test_ring ring {Ring} -strings {one\ttwo\tthree}\n");
	sock_send_string(sock, "menu_add_item {test} test_slider slider {Slider} -mintext < -maxtext > -value 50\n");
	sock_send_string(sock, "menu_add_item {test} test_numeric numeric {Numeric} -value 42\n");
	sock_send_string(sock, "menu_add_item {test} test_alpha alpha {Alpha} -value abc\n");
	sock_send_string(sock, "menu_add_item {test} test_ip ip {IP} -v6 false -value 192.168.1.1\n");
	sock_send_string(sock, "menu_add_item {test} test_menu menu {Menu}\n");
	sock_send_string(sock, "menu_add_item {test_menu} test_menu_action action {Submenu's action}\n");
	/*
	 * no successor for menus. Since test_checkbox and test_ring have
	 * their own predecessors defined the "ask" rule will not work for
	 * them.
	 */
	sock_send_string(sock, "menu_set_item {} test -prev {ask}\n");

	sock_send_string(sock, "menu_set_item {test} test_action -next {test_checkbox}\n");
	sock_send_string(sock, "menu_set_item {test} test_checkbox -next {test_ring} -prev test_action\n");
	sock_send_string(sock, "menu_set_item {test} test_ring -next {test_slider} -prev {test_checkbox}\n");
	sock_send_string(sock, "menu_set_item {test} test_slider -next {test_numeric} -prev {test_ring}\n");
	sock_send_string(sock, "menu_set_item {test} test_numeric -next {test_alpha} -prev {test_slider}\n");
	sock_send_string(sock, "menu_set_item {test} test_alpha -next {test_ip} -prev {test_numeric}\n");
	sock_send_string(sock, "menu_set_item {test} test_ip -next {test_menu} -prev {test_alpha}\n");
	sock_send_string(sock, "menu_set_item {test} test_menu_action -next {_close_}\n");
#endif				/* LCDPROC_CLIENT_TESTMENUS */

	return 0;
}
#endif				/* LCDPROC_MENUS */


/** Main program loop... */
void
main_loop(void)
{
	int i = 0, j;
	int connected = 0;
	char buf[8192];
	char *argv[256];
	int argc, newtoken;
	int len;

	while (!Quit) {
		/* Check for server input... */
		len = sock_recv(sock, buf, 8000);

		/* Handle server input... */
		while (len > 0) {
			/* Now split the string into tokens... */
			argc = 0;
			newtoken = 1;

			for (i = 0; i < len; i++) {
				switch (buf[i]) {
					case ' ':
						newtoken = 1;
						buf[i] = 0;
						break;
					default:	/* regular chars, keep
							 * tokenizing */
						if (newtoken)
							argv[argc++] = buf + i;
						newtoken = 0;
						break;
					case '\0':
					case '\n':
						buf[i] = 0;
						if (argc > 0) {
							if (0 == strcmp(argv[0], "listen")) {
								for (j = 0; sequence[j].which; j++) {
									if (sequence[j].which == argv[1][0]) {
										sequence[j].flags |= VISIBLE;
										debug(RPT_DEBUG, "Listen %s", argv[1]);
									}
								}
							}
							else if (0 == strcmp(argv[0], "ignore")) {
								for (j = 0; sequence[j].which; j++) {
									if (sequence[j].which == argv[1][0]) {
										sequence[j].flags &= ~VISIBLE;
										debug(RPT_DEBUG, "Ignore %s", argv[1]);
									}
								}
							}
							else if (0 == strcmp(argv[0], "key")) {
								debug(RPT_DEBUG, "Key %s", argv[1]);
							}
#ifdef LCDPROC_MENUS
							else if (0 == strcmp(argv[0], "menuevent")) {
								if (argc == 4 && (0 == strcmp(argv[1], "update"))) {
									set_mode(argv[2][0], "", strcmp(argv[3], "off"));
								}
							}
#else
							else if (0 == strcmp(argv[0], "menu")) {
							}
#endif
							else if (0 == strcmp(argv[0], "connect")) {
								int a;
								for (a = 1; a < argc; a++) {
									if (0 == strcmp(argv[a], "wid"))
										lcd_wid = atoi(argv[++a]);
									else if (0 == strcmp(argv[a], "hgt"))
										lcd_hgt = atoi(argv[++a]);
									else if (0 == strcmp(argv[a], "cellwid"))
										lcd_cellwid = atoi(argv[++a]);
									else if (0 == strcmp(argv[a], "cellhgt"))
										lcd_cellhgt = atoi(argv[++a]);
								}
								connected = 1;
								if (displayname != NULL)
									sock_printf(sock, "client_set -name \"%s\"\n", displayname);
								else
									sock_printf(sock, "client_set -name {LCDproc %s}\n", get_hostname());
#ifdef LCDPROC_MENUS
								menus_init();
#endif
							}
							else if (0 == strcmp(argv[0], "bye")) {
								exit_program(EXIT_SUCCESS);
							}
							else if (0 == strcmp(argv[0], "success")) {
							}
							else {
								/*
								int j;
								for (j = 0; j < argc; j++)
									printf("%s ", argv[j]);
								printf("\n");
								*/
							}
						}

						/* Restart tokenizing */
						argc = 0;
						newtoken = 1;
						break;
				}	/* switch( buf[i] ) */
			}

			len = sock_recv(sock, buf, 8000);
		}

		/* Gather stats and update screens */
		if (connected) {
			for (i = 0; sequence[i].which > 0; i++) {
				sequence[i].timer++;

				if (!(sequence[i].flags & ACTIVE))
					continue;

				if (sequence[i].flags & VISIBLE) {
					if (sequence[i].timer >= sequence[i].on_time) {
						sequence[i].timer = 0;
						/* Now, update the screen... */
						update_screen(&sequence[i], 1, config);
					}
				}
				else {
					if (sequence[i].timer >= sequence[i].off_time) {
						sequence[i].timer = 0;
						/* Now, update the screen... */
						update_screen(&sequence[i], sequence[i].show_invisible, config);
					}
				}
				if (islow > 0)
					usleep(islow * 10000);
			}
		}

		/* Now sleep... */
		usleep(TIME_UNIT);
	}
}

/* EOF */
