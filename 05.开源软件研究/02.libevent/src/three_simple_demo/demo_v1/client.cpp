#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
 
#include<event.h>
#include<event2/bufferevent.h>
#include<event2/buffer.h>
#include<event2/util.h>

int tcp_connect_server(const char* server_ip, int port);

void cmd_msg_cb(int fd, short events, void* arg);

void server_msg_cb(struct bufferevent*bev, void*arg);

void event_cb(struct bufferevent* bev, short event, void* arg);

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        printf("please input 2 paramter !\n");
        return -1;
    }

    int sockfd = tcp_connect_server(argv[1], atoi(argv[2]));
    if(sockfd == -1)
    {
        perror("tcp connet error !");
        return -1;
    }

    printf("connect to server successful !\n");

    struct event_base* base = event_base_new();
    struct bufferevent* bev = bufferevent_socket_new(base, sockfd, BEV_OPT_CLOSE_ON_FREE);

    // 监听终端输入事件
    struct event* ev_cmd = event_new(base, STDIN_FILENO, EV_READ|EV_PERSIST, cmd_msg_cb, (void *)bev);
    event_add(ev_cmd, NULL);

    bufferevent_setcb(bev, server_msg_cb, NULL, event_cb, (void *)ev_cmd);
    bufferevent_enable(bev, EV_READ|EV_PERSIST);

    event_base_dispatch(base);

    printf("finished ....\n");

    return 0;
}

void cmd_msg_cb(int fd, short events, void* arg)
{
    char msg[1024];

    int ret = read(fd, msg, sizeof(msg));
    if(ret < 0)
    {
        perror("read fail !\n");
        exit(1);
    }

    struct bufferevent* bev = (struct bufferevent *)arg;

    //把终端的消息发送给服务端
    bufferevent_write(bev, msg, ret);
}

void server_msg_cb(struct bufferevent* bev, void* arg)
{
    char msg[1024];
    size_t len = bufferevent_read(bev, msg, sizeof(msg));
    msg[len] = '\0';

    printf("recv %s from server.\n", msg);
}

void event_cb(struct bufferevent* bev, short event, void* arg)
{
    if(event & BEV_EVENT_EOF)
    {
        printf("connection cloased !\n");
    }
    else if(event & BEV_EVENT_ERROR)
    {
        printf("some other error !\n");
    }

    //会自动关闭套接字和清空读写缓冲区
    bufferevent_free(bev);
    //所以，socket没有了,event也就没有存在的必要了
    struct event* ev = (struct event *)arg;
    event_free(ev);
}

int tcp_connect_server(const char* server_ip, int port)
{
    int sockfd, status, errno;
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    status = inet_aton(server_ip, &server_addr.sin_addr);
    if(status == 0)
    {
        errno =EINVAL;
        return -1;
    }

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        return -1;
    }

    status = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(status == -1)
    {
        int save_errno = errno;
        close(sockfd);
        errno = save_errno;
        return -1;
    }

    evutil_make_socket_nonblocking(sockfd);
    return sockfd;

}