#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>

#include "wrap.h"

#define SERV_PORT 6666

int main(int argc, char *argv[])
{
    int i, j, n;

    int nready; /* 用于保存select返回的数值，即当前活跃的文件描述符个数 */
	int client[FD_SETSIZE];                 /* 自定义数组client，保存客户端连接套接字, 防止遍历1024个文件描述符  FD_SETSIZE默认为1024 */
	int maxi; /* 用于标记client数组中关注的最后一个文件描述符，maxi保存该文件描述符的下标，以后遍历时就只需要遍历到该下标位置，而不是整个client数组*/
    int maxfd; /* client中数值最大的文件描述符，select第一个参数需要该值 + 1*/
	int listenfd; /* 服务器的监听套接字 */
	int connfd; /* accept返回的一个连接套接字*/
	int sockfd; /* 该套接字是遍历时的一个缓冲套接字sockfd=client[n]，在后面代码中会用到，其实也可以不使用该变量保存套接字 */
    char buf[BUFSIZ]; /* read/write缓冲区 */
	char str[INET_ADDRSTRLEN];         /* #define INET_ADDRSTRLEN 16 */

    struct sockaddr_in clie_addr, serv_addr;
    socklen_t clie_addr_len;
    fd_set rset, allset;                            /* rset 读事件文件描述符集合，作为参数传入select()中。 allset用来暂存 */

    listenfd = Socket(AF_INET, SOCK_STREAM, 0); // 创建监听套接字

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // 设置地址复用

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family= AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port= htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    Listen(listenfd, 128);

    maxfd = listenfd;                                           /* 起初只有一个监听套接字listenfd，所以将 listenfd 设置最大文件描述符 */

    maxi = -1;                                                  /* 将来用作client[]的下标, 初始值指向0个元素之前下标位置 */
    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;                                         /* 用-1初始化client[]，标记为-1的位置表示没有存放文件描述符 */

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);                                  /* 构造select监控文件描述符集，将listenfd监听套接字添加到该集合中 */

    while (1) {   
        rset = allset;                                          /* 每次循环时都重新设置select监控信号集 */
        nready = select(maxfd+1, &rset, NULL, NULL, NULL); // select设置为阻塞监听模式，一旦有活跃套接字便返回。
        if (nready < 0)
            perr_exit("select error");

        if (FD_ISSET(listenfd, &rset)) {                        /* 说明有新的客户端链接请求 */

            clie_addr_len = sizeof(clie_addr);
            connfd = Accept(listenfd, (struct sockaddr *)&clie_addr, &clie_addr_len);       /* Accept 不会阻塞，会立马返回 */
            printf("received from %s at PORT %d\n",
                    inet_ntop(AF_INET, &clie_addr.sin_addr, str, sizeof(str)),
                    ntohs(clie_addr.sin_port));

            for (i = 0; i < FD_SETSIZE; i++)
                if (client[i] < 0) {                            /* 找client[]中没有使用的位置 */
                    client[i] = connfd;                         /* 保存accept返回的文件描述符到client[]里 */
                    break;
                }

            if (i == FD_SETSIZE) {                              /* 检查是否达到select能监控的文件个数上限 1024 */
                fputs("too many clients\n", stderr);
                exit(1);
            }

            FD_SET(connfd, &allset);                            /* 向监控文件描述符集合allset添加新的文件描述符connfd */
            if (connfd > maxfd)
                maxfd = connfd;   /* select第一个参数需要，由于有了新的套接字加入，如果新的套接字connfd比之前的任何文件描述符都大，则设置当前最大文件描述符为connfd */

            if (i > maxi)
                maxi = i;                                       /* 保证maxi存的总是client[]最后一个不为-1的元素下标 */

            if (--nready == 0)  // 已经完成监听套接字的处理，如果当前只有listenfd活动那么--nready==0，那么说明只存在连接事件，则继续阻塞监听是否有其他事件发生。
                continue;   // 如果--nread>0，说明client中还有connfd连接套接字上存在读事件，即客户端向server发送数据，则继续下面对活跃客户端套接字处理，需要处理--nready个套接字
        } 

        for (i = 0; i <= maxi; i++) {                               /* 检测哪个clients 有数据就绪 */

            if ((sockfd = client[i]) < 0)// client[i]如果等于-1则跳过
                continue;
            if (FD_ISSET(sockfd, &rset)) {

                if ((n = Read(sockfd, buf, sizeof(buf))) == 0) {    /* 当client关闭链接时,服务器端也关闭对应链接 */
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);                        /* 解除select对此文件描述符的监控 */
                    client[i] = -1;
                } else if (n > 0) {
                    for (j = 0; j < n; j++)      // 将收到的数据转大写，回写到客户端，同时输出到终端
                        buf[j] = toupper(buf[j]);
                    Write(sockfd, buf, n);
                    Write(STDOUT_FILENO, buf, n);
                }
                if (--nready == 0)
                    break;                                          /* 处理完所有的活跃文件描述符，跳出for, 但还在while中 */
            }
        }
    }
    Close(listenfd);
    return 0;
}

