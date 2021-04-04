#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include "define.h"
#define DEFAULT_PORT 6666
int main( int argc, char ** argv){
    int sockfd,acceptfd; /* 监听socket: sock_fd,数据传输socket: acceptfd */
    struct sockaddr_in my_addr; /* 本机地址信息 */
    struct sockaddr_in their_addr; /* 客户地址信息 */
    unsigned int sin_size, myport=6666, lisnum=10;
    if ((sockfd = socket(AF_INET , SOCK_STREAM, 0)) == -1) {
       perror("socket" );
       return -1;
    }
    printf("socket ok \n");
    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(DEFAULT_PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero), 0);
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr )) == -1) {
        perror("bind" );
        return -2;
    }
    printf("bind ok \n");
    if (listen(sockfd, lisnum) == -1) {
        perror("listen" );
        return -3;
    }
    printf("listen ok \n");
    char recvMsg[1000];
    sin_size = sizeof(my_addr);
    acceptfd = accept(sockfd,(struct sockaddr *)&my_addr,&sin_size);
    if (acceptfd < 0) {
       close(sockfd);
       printf("accept failed\n" );
       return -4;
    }
    ssize_t readLen = MyRecv(acceptfd, recvMsg, sizeof(int));
    if (readLen < 0) {
       printf("read failed\n" );
       return -1;
    }
    int len=(int)ntohl(*(int*)recvMsg);
    printf("len:%d\n",len);
    readLen = MyRecv(acceptfd, recvMsg, len);
    if (readLen < 0) {
       printf("read failed\n" );
       return -1;
    }
    char * pBuff=recvMsg;
    Pkg RecvPkg;
    int iLen=0;
    memcpy(&RecvPkg.head.num , pBuff + iLen, sizeof( int));
    iLen += sizeof(int);
    RecvPkg. head. num = ntohl(RecvPkg.head.num);
    printf("RecvPkg.head.num:%d\n" ,RecvPkg.head.num);
    memcpy(&RecvPkg.head.index , pBuff + iLen, sizeof( int));
    iLen += sizeof(int);
    RecvPkg. head. index = ntohl(RecvPkg.head.index);
    printf("RecvPkg.head.index:%d\n" ,RecvPkg.head.index);
    memcpy(&RecvPkg.content.sex , pBuff + iLen, sizeof( char));
    iLen += sizeof(char);
    printf("RecvPkg.content.sex:%c\n" ,RecvPkg.content.sex);
    memcpy(&RecvPkg.content.score , pBuff + iLen, sizeof( int));
    iLen += sizeof(int);
    RecvPkg. content.score = ntohl(RecvPkg. content.score );
    printf("RecvPkg.content.score:%d\n" ,RecvPkg.content.score);
    memcpy(&RecvPkg.content.address, pBuff + iLen, sizeof(RecvPkg.content.address ));
    iLen += sizeof(RecvPkg.content.address);
    printf("RecvPkg.content.address:%s\n" ,RecvPkg.content.address);
    memcpy(&RecvPkg.content.age , pBuff + iLen, sizeof( int));
    iLen += sizeof(int);
    RecvPkg.content.age = ntohl(RecvPkg.content.age );
    printf("RecvPkg.content.age:%d\n" ,RecvPkg.content.age);
	close(acceptfd);
    return 0;
  }
