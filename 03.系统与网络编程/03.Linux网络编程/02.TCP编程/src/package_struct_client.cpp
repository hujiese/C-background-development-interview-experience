#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "define.h"
#define DEFAULT_PORT 6666
int main( int argc, char * argv[]){
    int connfd = 0;
    int cLen = 0;
    struct sockaddr_in client;
    if(argc < 2){
        printf(" Uasge: clientent [server IP address]\n");
        return -1;
    }
    client.sin_family = AF_INET;
    client.sin_port = htons(DEFAULT_PORT);
    client.sin_addr.s_addr = inet_addr(argv[1]);
    connfd = socket(AF_INET, SOCK_STREAM, 0);
    if(connfd < 0){
        printf("socket() failure!\n" );
        return -1;
    }
    if(connect(connfd, (struct sockaddr*)&client, sizeof(client)) < 0){
        printf("connect() failure!\n" );
        return -1;
    }
    Pkg mypkg;
    mypkg.head.num=1;
    mypkg.head.index=10001;
    mypkg.content.sex='m';
    mypkg.content.score=90;
    char * temp="guangzhou and shanghai";
    strncpy(mypkg.content.address,temp,sizeof(mypkg.content.address));
    mypkg.content.age=18;
	ssize_t writeLen;
    int tLen=sizeof(mypkg);
    printf("tLen:%d\n" ,tLen);
    int iLen=0;
    char * pBuff= new char [1000];
    *(int*)(pBuff+iLen)= htonl(tLen);
    iLen+=sizeof( int);
    *(int*)(pBuff+iLen)= htonl(mypkg.head.num);
    iLen+=sizeof( int);
    *(int*)(pBuff+iLen)= htonl(mypkg.head.index);
    iLen+=sizeof( int);
    memcpy(pBuff+iLen,&mypkg.content.sex,sizeof( char));
    iLen+=sizeof( char);
    *(int*)(pBuff+iLen)= htonl(mypkg.content.score);
    iLen+=sizeof( int);
    memcpy(pBuff+iLen,mypkg.content.address,sizeof(mypkg.content.address));
    iLen+=(sizeof(mypkg.content.address));
    *(int*)(pBuff+iLen)= htonl(mypkg.content.age);
    iLen+=sizeof( int);
    writeLen= MySend(connfd, pBuff, iLen);
    if (writeLen < 0) {
       printf("write failed\n" );
       close(connfd);
       return 0;
    }
    else{
       printf("write sucess, writelen :%d, iLen:%d, pBuff: %s\n",writeLen,iLen,pBuff);
    }
    close(connfd);
    return 0;
}
