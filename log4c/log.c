/*
 * export LOG4C_RCPATH=`pwd` or ~/.log4crc
 * */
#include <log4c.h>
#include "log.h"
static log(char *str){
	//初始化
	log4c_init();
	//获取一个已有的category
	log4c_category_t* mycat = log4c_category_get("mine");
	//用该category进行日志输出，优先级为DEBUG，输出信息为 "Hello World!"
	//log4c_category_log(mycat, LOG4C_PRIORITY_DEBUG, str);
	
	LOG_DEBUG(mycat,str);
}
int main(){
	log("helloworld");
}
