#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
int MySend( int iSock, char * pchBuf, size_t tLen){
        int iThisSend;
        unsigned int iSended=0;
        if(tLen == 0)
               return(0);
        while(iSended<tLen){
              do{
                     iThisSend = send(iSock, pchBuf, tLen-iSended, 0);
              } while((iThisSend<0) && (errno==EINTR));
               if(iThisSend < 0){
                      return(iSended);
              }
              iSended += iThisSend;
              pchBuf += iThisSend;
       }
        return(tLen);
}
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
    ssize_t writeLen;
    char *sendMsg = "0123456789";
    int tLen=strlen(sendMsg);
    printf("tLen:%d\n" ,tLen);
    int iLen=0;
    char * pBuff= new char [100];
    *(int*)(pBuff+iLen)= htonl(tLen);
    iLen+=sizeof( int);
    memcpy(pBuff+iLen,sendMsg,tLen);
    iLen+=tLen;
    writeLen= MySend(connfd, pBuff, iLen);
    if (writeLen < 0) {
       printf("write failed\n" );
       close(connfd);
       return 0;
    }
    else{
       printf("write sucess, writelen :%d, sendMsg:%s\n",writeLen,sendMsg);
    }
    close(connfd);
    return 0;
}
