#include "myhead.h"
#include "response.h"
#include <assert.h>
#include "log.h"
#define MAX_FILENAME_LEN 128
#define MAX_REQUEST_LINE 4096
#define MAX_FILE_LEN 1024*4
#define ROOTDIR "/root/test/www"
int parse_filename(char *request,char *filename){
	char test[MAX_REQUEST_LINE];
	
	//char test[]="GET /resource/rs_041112_02.php Http1.1\nHost: www.maketop.net\n";
	char *str;
	strncpy(test,request,strlen(request+1));
	str=strtok(test," ");
	if((!strcmp(str,"GET"))||(!strcmp(str,"POST"))){
		strcpy(filename,strtok(NULL," "));
	}
#ifdef DEBUG
	printf("filename is %s\n",filename);
#endif
/*
	char filename[MAX_FILENAME_LEN];
	return filename;
	这里filename是栈变量，随着函数的return，栈空间也会释放，所以如果返回栈的变量
	，可能会出现异常情况，即这个下次可能会和下一个函数的某个变量的地址相同！！
*/
	return 1;
}
int get_response(char *request,response *res){
	
	char buf[MAX_FILE_LEN];
	int fd,nread;
	char ab_name[MAX_FILENAME_LEN]="";
	char ab_dir[MAX_FILENAME_LEN]="";
	char *filename=malloc(MAX_FILENAME_LEN*sizeof(char));
	parse_filename(request,filename);
	// printf("debug: filename is %s\n",filename);
	// printf("22222222222222222222:res is %x\n",res);
#ifdef DEBUG	
	printf("=====================\n");
	printf("filename at %x\n",filename);
	printf("ab_name at %x\n",ab_name);
	printf("ab_dir at %x\n",ab_dir);
	printf("=====================\n");
#endif
	strcat(ab_dir,ROOTDIR);
	strcpy(ab_name,ab_dir);
	// printf("abdir is %s ,ab_name is %s\n",ab_dir,ab_name);
	if(!strcmp(filename,"/")){
		strcat(ab_name,"/index.html");
		}
	strcat(ab_name,filename);
	// printf("final ab_name is %s\n",ab_name);
	if((fd=open(ab_name,O_RDONLY))<0){
		
		if((fd=open(strcat(ab_dir,"/404.html"),O_RDONLY))<0){
			// printf("final ab_dir is %s\n",ab_dir);
			printf("open file error\n");
			return NULL;
		}
		printf("3333333333333333333333res:%x\n",res);
		printf("here,res->responseline:%x\n",res->responseline);
		printf("here,res->responseline->status:%x\n",res->responseline->status);
		printf("here,res->responseline->desc:%x\n",res->responseline->desc);
		res->responseline->status=404;
		printf("responseline->status: %x:%d\n",&(res->responseline->status),res->responseline->status);
		stpcpy(res->responseline->desc,"NOT FOUND\r\n");
		//res->responseline->desc="NOT FOUND\r\n";
		printf("responseline->desc: %s\n",res->responseline->desc);
		printf("here2\n");
	}
	if((nread=read(fd,buf,MAX_FILE_LEN))<0){
		printf("read file error\n");
		close(fd);
		return NULL;
	}
	printf("read buf is %s\n",buf);
	strcpy(res->body->data,buf);
	printf("copy to res->body->data is %s\n",res->body->data);
	res->body->size=strlen(buf);
	printf("res->body->size is %d\n",res->body->size);
	sprintf(res->header->value,"%d",res->body->size);
	printf("res->header->value is %s\n",res->header->value);
	close(fd);
	free(filename);//filename is malloc
	printf("free filename ok\n");
	return 1;
}
int return_response(char *request,char *responsebuf,int len){
	response *res=calloc(1,sizeof(response));
	// printf("calloc for res %x\n",res);
	if(response_init(res))
		printf("response init ok\n");
	else
		return -1;
	// printf("111111111111111111:res is %x\n",res);
	// printf("111111111111111111:res->responseline is %x\n",res->responseline);
	// printf("111111111111111111:res->header is %x\n",res->header);
	// printf("111111111111111111:res->body is %x\n",res->body);
	get_response(request,res);
	combine_response(res,responsebuf,len);
	response_free(res);
	return 1;
}
int response_init(response *r){
	r->responseline=calloc(1,sizeof(struct _responseline));
	if(responseline_init(r->responseline)<0)
		return -1;
	// printf("responseline init ok:%x\n",r->responseline);
	r->header=calloc(1,sizeof(header));
	if(header_init(r->header)<0)
		return -1;
	// printf("response header init ok:%x\n",r->header);
	r->body=calloc(1,sizeof(struct _body));
	if(body_init(r->body)<0)
		return -1;
	// printf("response body init ok:%x\n",r->body);
	return 1;
}
int responseline_init(struct _responseline *r){

	if(!(r->version=calloc(10,sizeof(char)))){
		printf("calloc for version %x\n",r->version);
		return -1;
		}
	//printf("calloc for version %x:%s\n",r->version,r->version);
	if(!(r->desc=calloc(100,sizeof(char))))
		return -1;
	//printf("calloc for desc %x:%s\n",r->desc,r->desc);
	stpcpy(r->version,"HTTP/1.1\0");
	//printf("init version with %x:%s\n",r->version,r->version);
	r->status=200;
	//printf("init status with %x:%d\n",&(r->status),r->status);
	strcpy(r->desc,"ok\r\n\0");
	//printf("init desc with %x:%s\n",r->desc,r->desc);
	return 1;
}
int header_init(header *head){
	
	if(!(head->key=calloc(100,sizeof(char))))
		return -1;
	// printf("calloc for head->key %x\n",head->key);
	if(!(head->value=calloc(400,sizeof(char))))
		return -1;
	// printf("calloc for head->value %x\n",head->value);
	strcpy(head->key,"Content-length");
	strcpy(head->value,"0\r\n");
	head->next=NULL;
	return 1;
}
int body_init(struct _body *body){
	if(!(body->data=calloc(MAX_FILE_LEN,sizeof(char))))
		return -1;
	// printf("calloc for body->data %x\n",body->data);
	body->size=0;
	return 1;
}
void response_free(response* res){
	responseline_free(res->responseline);
	headers_free(res->header);
	body_free(res->body);
}
void responseline_free(struct _responseline* r){
	free(r->version);
	free(r->desc);
}
void headers_free(header *h){
	header *tmp=h;
	header *freenode=h;
	while(freenode->next){
		tmp=freenode->next;
		free(freenode->key);
		free(freenode->value);
		free(freenode);
		freenode=tmp;
	}
}
void body_free(struct _body	*b){
	free(b);
}
void add_header(response *src,header *newheader){
	header *tmp;
	if(src->header->next==NULL)
		src->header->next=newheader;
	else{
	tmp=src->header->next;
		while(tmp->next){
			tmp=tmp->next;
		}
		tmp->next=newheader;
	}
}
int combine_response(response *res,char *content,int len){
	/*
		@parm1: 
		@parm2: after combine,fill it with the response
		@notice: please ensure
	*/
	char str[32];
	header *tmp;
	memset(content,0,len);
	strcat(content,res->responseline->version);
	strcat(content," ");
	sprintf(str,"%d",(res->responseline->status));
	strcat(content,str);
	strcat(content," ");
	strcat(content,res->responseline->desc);
	tmp=res->header;
	while((tmp)){
		strcat(content,tmp->key);
		strcat(content,":");
		strcat(content,tmp->value);
		tmp=tmp->next;
	}
	strcat(content,"\r\n\r\n");
	strcat(content,res->body->data);
	return 1;
}
#ifdef MY_DEBUG
main(){
	
	char *buf1="GET /resource/rs_041112_02.php Http1.1\nHost: www.maketop.net\n";
	char *buf2=malloc(4096*sizeof(char));
	return_response(buf1,buf2,4096);
	printf("response is %s\n",buf2);
	free(buf2);
}
#endif