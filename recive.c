#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include <sys/uio.h>


#define MAXLEN 4096
#define SERV_PORT   10031
#define SEND_PORT   9999
#define SEND_IP   "118.190.27.35"

int main()

{
        int sock_fd;   //套接子描述符号
        int server_fd;
        int recv_num;
        int send_num;
        int client_len;
        char recv_buf[MAXLEN];
        struct sockaddr_in  addr_serv;
        struct sockaddr_in  addr_client;//服务器和客户端地址
        struct sockaddr_in servaddr;
//server
 /* init servaddr */
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SEND_PORT);
        if(inet_pton(AF_INET, SEND_IP, &servaddr.sin_addr) <= 0)
        {
                printf("[%s] is not a valid IPaddress\n", SEND_PORT);
                exit(1);
        }

        server_fd = socket(AF_INET, SOCK_DGRAM, 0);

//client;
        sock_fd = socket(AF_INET,SOCK_DGRAM,0);
        if(sock_fd < 0){
                perror("socket");
                exit(1);
        } else{

                printf("sock sucessful\n");
        }
        //初始化服务器断地址
        memset(&addr_serv,0,sizeof(struct sockaddr_in));
        addr_serv.sin_family = AF_INET;//协议族
        addr_serv.sin_port = htons(SERV_PORT);
        addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);//任意本地址

        client_len = sizeof(struct sockaddr_in);
        /*绑定套接子*/
        if(bind(sock_fd,(struct sockaddr *)&addr_serv,sizeof(struct sockaddr_in))<0 ){
                perror("bind");
                exit(1);
        } else{
                printf("bind sucess\n");
        }
        char buffer[MAXLEN];
        struct sockaddr_storage src_addr;

        struct iovec iov[1];
        iov[0].iov_base=buffer;
        iov[0].iov_len=sizeof(buffer);

        struct msghdr message;
        message.msg_name=&src_addr;
        message.msg_namelen=sizeof(src_addr);
        message.msg_iov=iov;
        message.msg_iovlen=1;
        message.msg_control=0;
        message.msg_controllen=0;

        while(1){
                printf("begin recv:\n");
                ssize_t count=recvmsg(sock_fd,&message,0);
                char* ip_string = inet_ntoa(addr_serv.sin_addr);
                printf("%s", ip_string);        
                if (count==-1) {
                        printf("bad\n");
                        perror("again recvfrom");
                        exit(1);
                } else if (message.msg_flags&MSG_TRUNC) {
                        warn("datagram too large for buffer: truncated");
                } else {
                        sendto(server_fd, buffer,count,0,(struct sockaddr *)&servaddr,sizeof(servaddr));
                        int i = 0;
                        for (i = 0 ;i<count;i++)
                        printf("%02hhX ",buffer[i]);
                        printf("\n");
                //      printBits(count,&buffer);
                        //      handle_datagram(buffer,count);
                }
/*
                recv_num = recvfrom(sock_fd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&addr_client,&client_len);
                if(recv_num <  0){
                        printf("bad\n");
                        perror("again recvfrom");
                        exit(1);
                } else{
                        recv_buf[recv_num]='\0';
                        printf("recv sucess:%s\n",recv_buf);
                }
*/
        }
        close(sock_fd);
        return 0;
}




