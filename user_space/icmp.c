#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int sock;

    sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_ICMP);
    if(sock<0){
        printf("error:%d\n",sock);
        perror("create sock failed..");
    }
    return 0;
}
