#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UNIXDG_PATH "./unix.dg"
#define BUFFER_SIZE 256

int main(void)
{
    int sockfd;
    struct sockaddr_un servaddr, cliaddr;
    socklen_t len;

    sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);

    /* local address */
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sun_family = AF_LOCAL;
    strcpy(cliaddr.sun_path, UNIXDG_PATH);
    bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

    /* remote address */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXDG_PATH);
    len = sizeof(servaddr);

    char buf[BUFFER_SIZE];

    while(1)
    {
        bzero(buf, BUFFER_SIZE);

        printf(">> ");
        if(fgets(buf, BUFFER_SIZE, stdin) == NULL)
        {
            break;
        }
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, len);
    }

    close(sockfd);

    return 0;   
}
