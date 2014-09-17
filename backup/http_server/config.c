#include "myhead.h"
#include "config.h"
void init_conf(config *server){
		server=calloc(1,sizeof(config));
		server->key="test";
		server->value="testvalue";
		server->next=NULL;
}
void destory_conf(config *server){
	free(server);
}
#ifdef DEBUG
#include <assert.h>
main(){
	config *server;
	assert(server);
	init_conf(server);
	printf("config:%s=%s\n",server->key,server->value);
}
#endif