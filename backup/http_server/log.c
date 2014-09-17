#include "log.h"
#define LOGFILE "./server.log"
void mylog(char *msg,...){
	//int fd;
	FILE *fn;
	int line=0,num=0;
	char *filename="NULL";
	va_list var;
	pthread_mutex_t rwfile=PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&rwfile);
	fn=fopen(LOGFILE,"a");
	//printf("fd is %d\n",fd);
	if(fn<0){
		perror("open log file error\n");
		exit(0);
		}
/*
@arg:
int num: number of left arg
char *file: __FILE__
int line: __LINE__
...
*/
	va_start(var,msg);
	//while(1){
	
	num=va_arg(var,int);
	if(num>0){
		filename=va_arg(var,char *);
		line=va_arg(var,int);
	}
	//}
	va_end(var);
	
	fprintf(fn,"%s %s (%s:%d) %s\n",__DATE__,__TIME__,filename,line,msg);
	//printf("write log ok\n");
	fclose(fn);
	pthread_mutex_unlock(&rwfile);
}
