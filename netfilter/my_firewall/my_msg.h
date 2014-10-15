#ifndef _MY_MSG
#define _MY_MSG
struct my_msg{
	int cmd;//add delete modify
	int count;//rule count
	void *r[0];
};
struct rule{
	int id;
	int protocol;
	int src;
	int dst;
	short sport;
	short dport;
	int action;
};
#endif
