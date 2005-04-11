/*
 this is vermont.
 released under GPL v2

 (C) by Ronny T. Lampert

 */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <vector>

/* 
 foreign subsystems: sampler
 */
#include "ipfixlolib.h"

#include "Packet.h"
#include "Filter.h"
#include "Observer.h"
#include "PacketSink.h"
#include "ExporterSink.h"
#include "PacketProcessor.h"
#include "Template.h"
#include "IPHeaderFilter.h"

/* collector */


/* own systems */
#include "iniparser.h"
#include "msg.h"
#include "subsystems.h"

using namespace std;

/* holding all objects/handles/... for the subsystems like sampler and collector */
struct v_objects {

	/* main vermont: */
        /* the configuration struct, from iniparser */
	dictionary *v_config;
	/* initialized subsystems */
	unsigned int v_subsystems;

	/* for sampler: */
	Template *templ;
	Observer *observer;
        Filter *filter;
	vector<PacketProcessor *> processors;
	ExporterSink *exporter;
		
	/* for collector: */
	int collector_handle;
};

static void usage();
static void sig_handler(int x);
static int setup_signal(int signal, void (*handler)(int));
static int vermont_readconf(dictionary **conf, char *file);
static int vermont_configure(struct v_objects *v, dictionary *conf);
static Template * configure_template(uint16_t template_id, char *list);
static Observer * configure_observer(char *interface, int snaplen);

int main(int ac, char **dc)
{
        dictionary *config;
	int c, debug_level=MSG_DEFAULT;
        char *config_file=NULL;

        struct v_objects v_objects;

	/* parse command line */
	while((c=getopt(ac, dc, "hf:d")) != -1) {

		switch(c) {

		case 'f':
			config_file=optarg;
			break;

		case 'd':
			debug_level++;
                        break;

		case 'h':
			usage();
                        return 0;

		default:
			usage();
                        break;
		}
	}

	/* setup verboseness */
	msg_setlevel(debug_level);

	setup_signal(SIGINT, sig_handler);

	if(vermont_readconf(&config, config_file) || vermont_configure(&v_objects, config)) {
		exit(-1);
	}


        subsys_dump(v_objects.v_subsystems);

        return 0;
}


/*
 read the config from *file and attach the iniparser stuff to **conf
 perform basic checks

 NOTE: the funny thing is, all returned strings are direct pointer into
 the dictionary-struct and as such valid until the dictionary is
 free()'d again.

 SO WE USUALLY DONT COPY!
 */
static int vermont_readconf(dictionary **conf, char *file)
{
        dictionary *d;

	/* read configuration */
	if(!(d=iniparser_new(file))) {
		msg(MSG_FATAL, "could not open config_file %s", file);
                return(-1);
	}

        /* check if all section we need are present */
	if((iniparser_find_entry(d, "collector") == 1) &&
	   (iniparser_find_entry(d, "sampler") == 1) &&
	   (iniparser_find_entry(d, "main") == 1)
	  ) {
		*conf=d;

		return 0;

	} else {
		msg(MSG_FATAL, "Config: not all needed sections in config %s", file);

		return -1;
	}
}


/*
 this is the main configuration entry point
 configure all subsystems peu a peu
 */
static int vermont_configure(struct v_objects *v, dictionary *conf)
{
        /* if the sampler is not needed, interface will say "off" */
	char *run_sampler=iniparser_getvalue(conf, "sampler", "interface");

	if(strcasecmp(run_sampler, "off") == 0) {
		msg(MSG_DIALOG, "not running sampler subsystem");
	} else {
                configure_sampler(v, conf);
	}


}
/* bla bla bla */
static void usage()
{
	printf(
	       "VERsatile MONitoring Tool - VERMONT\n" \
	       " mandatory:\n" \
	       "    -f <inifile>     load config\n" \
	       " optional:\n" \
	       "    -d               increase debug level\n" \
	      );
}


static int setup_signal(int signal, void (*handler)(int))
{
	struct sigaction sig;

	sig.sa_handler=sig_handler;
        sig.sa_flags=SA_RESTART;
        sigemptyset(&sig.sa_mask);
        return(sigaction(signal, &sig, NULL));
}


/* just shallow right now */
static void sig_handler(int x)
{
	msg(MSG_DIALOG, "got signal %d - exiting", x);
	exit(2);
}


static void __cplusplus_really_sucks_andPeopleUsingVeryStrangeNamingConventionsWithLongLongExplicitBlaBla(){};
