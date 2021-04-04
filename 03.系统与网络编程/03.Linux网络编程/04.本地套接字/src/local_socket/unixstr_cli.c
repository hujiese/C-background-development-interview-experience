#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UNIXSTR_PATH "./unix.str"
#define LISTENQ 5
#define BUFFER_SIZE 256

int main(void)
{
    int sockfd;
    struct sockaddr_un servaddr;

    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    char buf[BUFFER_SIZE];

    while(1)
    {
        bzero(buf, sizeof(BUFFER_SIZE));
        printf(">> ");
        if(fgets(buf, BUFFER_SIZE, stdin) == NULL)
        {
            break;
        }
        write(sockfd, buf, strlen(buf));
    }

    close(sockfd);

    return 0;
}