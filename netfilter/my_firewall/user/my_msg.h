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
#define GENL_ID_TEST 0x110
#define TEST_GENL_CMD 0x1
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
};
struct my_msg{
	int cmd;//add delete modify
	//int count;//rule count
	
	struct rule r[0];
};
struct nl_info{
	struct nlmsghdr nh;
	struct genlmsghdr genlhdr;
	struct my_msg uh;
};
#endif
