#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct {
    char name[20];
    int age;
}Person;

typedef struct {
    long type;
    Person person;
}Msg;

void printMsg(Msg *msg) {
    printf("{ type = %ld, name = %s, age = %d }\n",
           msg->type, msg->person.name, msg->person.age);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s <type>\n", argv[0]);
        return -1;
    }
    
    // 要获取的消息类型
    long type = atol(argv[1]);
    
    // 获取 ipc 内核对象 id
    int id = msgget(0x8888, 0);
   
    
    Msg msg;
    int res;
    
    while(1) {
        // 以非阻塞的方式接收类型为 type 的消息
        res = msgrcv(id, &msg, sizeof(Person), type, IPC_NOWAIT);
        if (res < 0) {
            // 如果消息接收完毕就退出，否则报错并退出
            if (errno == ENOMSG) {
                printf("No message!\n");
                break;
            }

        }
        // 打印消息内容
        printMsg(&msg);
    }
    return 0;
}