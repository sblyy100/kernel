    #include <stdlib.h>  
    #include <stdio.h>  
    #include <errno.h>  
    #include <string.h>  
    #include <netdb.h>  
    #include <sys/types.h>  
    #include <netinet/in.h>  
    #include <sys/socket.h>  
      
    #define portnumber 80
      
    int main(int argc ,char *argv[])  
    {  
            int sockfd,new_fd;  
            struct sockaddr_in server_addr;  
            struct sockaddr_in client_addr;  
            int sin_size;  
            int nbytes;  
            char buffer[1024];  
             int value = 1;
            /*服务器端开始建立sockfd描述符*/  
            if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){  
                    fprintf(stderr,"Socket error:%s\n\a",strerror(errno));    
                    exit(1);  
            }  
              
            /*服务器端填充sockaddr结构*/  
            bzero(&server_addr,sizeof(struct sockaddr_in)); //初始化,置0  
            server_addr.sin_family=AF_INET;   //internet  
            server_addr.sin_addr.s_addr=htonl(INADDR_ANY);//将本机上的long数据转化为网络上的long数据  
            //server_addr.sin_addr.s_addr=inet_addr("192.168.1.1");//用于绑定一个固定ip.inet_addr  
            server_addr.sin_port=htons(portnumber);//将本机上的short数据转化为网络上的short数据  
            setsockopt(sockfd, SOL_IP, IP_TRANSPARENT, &value, sizeof(value));
            /*捆绑sockfd描述符到ip地址*/  
            if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)  
            {  
                    fprintf(stderr,"Bind error:%s\n\a",strerror(errno));  
                    exit(1);      
            }  
              
            /*设置允许连接的最大客户端数*/  
            if(listen(sockfd,5)==-1)  
            {  
                    fprintf(stderr,"Listen errr:%s\n\a",strerror(errno));  
                    exit(1);  
            }  
              
            while(1)  
            {  
                    /*服务器阻塞，直到有客户程序建立连接*/  
                    sin_size=sizeof(struct sockaddr_in);  
                    if((new_fd=accept(sockfd,NULL,NULL))==-1)  
                    {  
                            fprintf(stderr,"Accept error:%s\n\a",strerror(errno));  
                exit(1);  
                    }     
                      
                      
                    if((nbytes=read(new_fd,buffer,1024))==-1)  
                    {  
                        fprintf(stderr,"Read Error:%s\n",strerror(errno));    
                        exit(1);  
                    }  
                    printf("Server received %s\n",buffer);  
                      
                    /*这个通讯已经结束*/  
                    close(new_fd);  
                    /*循环下一个*/  
            }  
            /*结束通讯*/  
    }  
