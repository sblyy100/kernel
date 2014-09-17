#ifndef _RESPONSE_H
#define _RESPONSE_H

struct _responseline {
	/*HTTP/1.1 200 OK\r\n*/
	char *version;//HTTP/1.1 HTTP/1.0
	unsigned int status;
	char *desc;
};
typedef struct _header{
	char *key;
	char *value;//end with \r\n
	struct header *next;//next header
}header,*headerlist;
struct _body{
	char *data;
	unsigned int size;
};
typedef struct _response{
	struct _responseline *responseline;
	headerlist header;
	struct _body *body;
}response;
int combine_response(response *,char *,int);
int return_response(char *,char *,int);
int response_init(response*);
int responseline_init(struct _responseline*);
int header_init(header*);
int body_init(struct _body*);
void response_free(response*);
void responseline_free(struct _responseline*);
void header_free(header*);
void body_free(struct _body*);
void add_header(response *,header *);
int parse_filename(char *request,char *);
int get_response(char *,response *);
#endif