#include<unistd.h> 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INPUT 0 
#define OUTPUT 1 
int main() {
	int fd[2];
	/*�����ӽ��̺� */ 
	pid_t pid; 
	char buf[256]; 
	int returned_count; 
	/*���������ܵ�*/ 
	pipe(fd);
	/*�����ӽ���*/
    pid=fork();	
	if(pid<0) {
		printf("Error in fork\n");
		exit(1); 
	}else if(pid == 0) { /*ִ���ӽ���*/
	    printf("in the child process...\n"); 
		/*�ӽ����򸸽���д���ݣ��رչܵ��Ķ���*/ 
		close(fd[INPUT]); 
		write(fd[OUTPUT], "hello world", strlen("hello world"));
		exit(0); 
	}else { /*ִ�и�����*/ 	    
		printf("in the parent process...\n"); 
		/*�����̴ӹܵ���ȡ�ӽ���д�����ݣ��رչܵ���д��*/
		close(fd[OUTPUT]); 
		returned_count = read(fd[INPUT], buf, sizeof(buf)); 
		printf("%d bytes of data received from child process: %s\n", returned_count, buf);
	}
	return 0;
}
