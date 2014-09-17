#ifndef _CONFIG_H
#define _CONFIG_H

#include "myhead.h"
#define _USE_CONF

typedef struct{
	char *key;
	char *value;
	struct config *next;
}config;
struct server_conf{
	char *port;
	char *docroot;
	char *logfile;
	int maxfds;
}
void init_conf(config *);
void destory_conf(config *server);
char *read_conf(config *,char *key);
#endif
