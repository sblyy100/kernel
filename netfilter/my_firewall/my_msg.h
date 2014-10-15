#ifndef _MY_MSG
#define _MY_MSG
#define DEV_NAME 16
enum RULE_CMD{
	RULE_NONE,
	RULE_ADD,
	RULE_DEL,
	RULE_MOD,
	RULE_END,
};
#define TEST_GENL_NAME "GENLTEST"
#define TEST_GENL_VERSION 0x1
#define TEST_GENL_HDRLEN 0
#define GENL_ID_TEST 0x110
#define TEST_GENL_CMD 0x1
struct my_msg{
	enum RULE_CMD cmd;//add delete modify
	//int count;//rule count
	
	struct rule r[0];
};
struct rule{
	int id;
	char from[DEV_NAME];
	char to[DEV_NAME];
	int protocol;
	int src;
	int dst;
	short sport;
	short dport;
	int from_index;
	int to_index;
	int action;
};
#endif
