/* test */

#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<linux/rtc.h>
#include<linux/ioctl.h>
#include "lwfw.h"

int main(int argc,char *argv[])
{
        int fd,i,retval;
        struct lwfw_stats data;
        char msg[128];

char * deny_ip = "192.168.10.10";
char *ifcfg = "eth1";
unsigned char * port = "\x00\x16"; //22


fd = open("/dev/lwfw", O_RDONLY);
if(fd == -1)
{
          perror("open fail!");
          exit(-1);
}
if( ioctl(fd,LWFW_ACTIVATE,0) == -1 )
{
             perror("ioctl LWFW_ACTIVATE fail!\n");
             exit(-1);
}
if( ioctl(fd, LWFW_DENY_IP, inet_addr(deny_ip)) == -1)
         {
            printf("ioctl LWFW_DENY_IP fail\n");
            exit(-1);
         }
if(ioctl(fd, LWFW_DENY_PORT, *(unsigned short *)port) == -1)
         {
           printf("ioctl LWFW_DENY_PORT fail!\n");
           exit(-1);
         }
if(ioctl(fd, LWFW_DENY_IF, (unsigned*)ifcfg) == -1)
         {
               printf("ioctl LWFW_DENY_IF fail!\n");
               exit(-1);
         }
 printf("set saddr,port,ifcfg seting ok.\n");
         
         close(fd);
}
