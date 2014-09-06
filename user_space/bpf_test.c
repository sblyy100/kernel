/* Missing check for BPF_S_ANC_NLATTR/BPF_S_ANC_NLATTR_NEST allows reading
 *  * beyond the end of the packet with a huge offset -- most probably resulting
 *   * in a Oops.
 *    *
 *     * ..since v2.6.26 (4738c1db15 "[SKFILTER]: Add SKF_ADF_NLATTR instruction").
 *      *
 *       * - minipli
 *        */
 #include <linux/filter.h>
 #include <sys/socket.h>
 #include <sys/wait.h>
 #include <unistd.h>
 #include <alloca.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdio.h>
#include <linux/in.h>
 #define array_size(x)   (sizeof(x) / sizeof(*(x)))
 int main(void) {
    char buf[1024];
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_port = htons(888);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    struct sock_filter bpf_code[] = {
    { 0x28, 0, 0, 0x0000000c },
{ 0x15, 0, 5, 0x000086dd },
{ 0x30, 0, 0, 0x00000014 },
{ 0x15, 6, 0, 0x00000011 },
{ 0x15, 0, 6, 0x0000002c },
{ 0x30, 0, 0, 0x00000036 },
{ 0x15, 3, 4, 0x00000011 },
{ 0x15, 0, 3, 0x00000800 },
{ 0x30, 0, 0, 0x00000017 },
{ 0x15, 0, 1, 0x00000011 },
{ 0x6, 0, 0, 0x0000ffff },
{ 0x6, 0, 0, 0x00000000 },
    };
     struct sock_fprog bpf = {
                         .len = array_size(bpf_code),
                         .filter = bpf_code,
     };
    int sock;
    int err;
    //sock= socket(AF_INET, SOCK_DGRAM, 0);
    sock=socket(PF_PACKET, SOCK_RAW,0);
     if (sock<0) {
                         perror("socket create");
                         exit(1);
     }
    
     err = setsockopt(sock, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf));
     if (err) {
        perror("setsockopt");
        exit(1);
                 
     }
     
    //if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)))
    //    perror("bind1");
    while(1){
    err = recvfrom(sock,buf,1024,0,NULL,NULL);
        if (err > 0) {
        }
        printf("recvmsg:%s\n",buf);
     }
    close(sock);
 }
