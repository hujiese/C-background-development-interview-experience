/*************************************************************************
	> File Name: msgsend2.c
	> Author: 
	> Mail: 
	> Created Time: 2019年11月08日 星期五 19时20分51秒
 ************************************************************************/

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
        char name[20];
        int age;
}Person;

typedef struct {
        long type;
        Person person;
}Msg;

int main(int argc, char *argv) 
{
    int id = msgget(0x8888, IPC_CREAT | 0664);
        
    Msg msg[10] = {
                {1, {"Luffy", 17}},
                {1, {"Zoro", 19}},
                {2, {"Nami", 18}},
                {2, {"Usopo", 17}},
                {1, {"Sanji", 19}},
                {3, {"Chopper", 15}},
                {4, {"Robin", 28}},
                {4, {"Franky", 34}},
                {5, {"Brook", 88}},
                {6, {"Sunny", 2}}
            
    };
        
    int i;
    for (i = 0; i < 10; ++i) 
    {
        int res = msgsnd(id, &msg[i], sizeof(Person), 0);
    }
    return 0;
}
