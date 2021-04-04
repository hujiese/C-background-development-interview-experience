#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#define MAXLINE 4096
int main(int argc, char** argv){
    int    listenfd, acceptfd;
    struct sockaddr_in     servaddr;
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return -1;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6666);
    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return -1;
    }
    if( listen(listenfd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return -1;
    }
    printf("======waiting for client's request======\n");
	if( (acceptfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
		printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
    }
	char recvMsg[100];
    ssize_t readLen = read(acceptfd, recvMsg, sizeof(recvMsg));
    if (readLen < 0) {
	   printf("read error: %s(errno: %d)\n",strerror(errno),errno);
       return -1;
    }
    recvMsg[9]='\0';
    printf("readLen:%d, recvMsg:%s\n" ,readLen,recvMsg);
    sleep(5);
    recvMsg[1]='9';
    ssize_t writeLen = write(acceptfd, recvMsg, sizeof(recvMsg));
    printf("writeLen:%d, sendMsg:%s\n" ,writeLen,recvMsg);
    if (writeLen < 0) {
	   printf("writeLen error: %s(errno: %d)\n",strerror(errno),errno);
       return -1;
    }
    close(acceptfd);
	return 0;
}
