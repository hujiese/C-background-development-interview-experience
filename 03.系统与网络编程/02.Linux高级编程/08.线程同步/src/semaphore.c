#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>


sem_t produce_sem;
sem_t custom_sem;

typedef struct node
{
    int data;
    struct node* next;
}Node;

Node* head = NULL;

void* producer(void* arg)
{
    while(1)
    {
        sem_wait(&produce_sem);     // porduce_sem --  == 0, 阻塞
        Node * node = (Node*)malloc(sizeof(Node));
        node->data = rand() % 1000;
        node->next = head;
        head = node;
        printf("+++++ 生产者：%lu, %d\n", pthread_self(), node->data);
//        print();
        sem_post(&custom_sem);  // custom_sem++
        
        sleep(rand()%5);
    }

    return NULL;
}

void* customer(void* arg)
{
    while(1)
    {
        sem_wait(&custom_sem);
        Node* del = head;
        head = head->next;
        printf("----- 消费者：%lu, %d\n", pthread_self(), del->data);
        free(del);
        sem_post(&produce_sem);

        sleep(rand()%5);
    }
    return NULL;
}

int main(int argc, const char* argv[])
{
    pthread_t thid[2];

    // 初始化信号量
    sem_init(&produce_sem, 0, 4);   // 初始化生产者线程信号量
    sem_init(&custom_sem, 0, 0);    // 初始化消费者线程信号量

    pthread_create(&thid[0], NULL, producer, NULL);
    pthread_create(&thid[1], NULL, customer, NULL);

    for(int i=0; i<2; ++i)
    {
        pthread_join(thid[i], NULL);
    }

    sem_destroy(&produce_sem);
    sem_destroy(&custom_sem);

    return 0;
}
