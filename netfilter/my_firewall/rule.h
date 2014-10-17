#ifndef _MY_KERNEL
#define _MY_KERNEL
#include "main.h"
#define TEST_GENL_NAME "GENLTEST"
#define TEST_GENL_VERSION 0x1
#define GENL_ID_TEST 0x110
#define TEST_GENL_CMD 0x1
#define TEST_GENL_HDRLEN 0
#define DEV_NAME 16
enum RULE_CMD{
	RULE_NONE,
	RULE_ADD,
	RULE_DEL,
	RULE_MOD,
	RULE_END,
};

struct rule{
	int id;
	union{
		char from[DEV_NAME];
		int from_index;
	};
	union{
		char to[DEV_NAME];
		int to_index;
	};
	int protocol;
	int src;
	int dst;
	short sport;
	short dport;
	int action;
	struct list_head list;
};
struct my_msg{
	enum RULE_CMD cmd;//add delete modify
	//int count;//rule count
	
	//void *data;
	struct rule data[0];
};
#endif
