#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	pid_t pid;

	pid = fork();
	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}

	if(pid == 0)
	{
		// 子进程执行另外一个程序
#if 0
// ++++++++++++++++++++ execl ++++++++++++++++++++
/*
 * int execl(const char *path, const char *arg, ...);
 		函数描述：加载一个进程，通过路径 + 程序名来加载。
 		函数参数：
 			path：程序路径+程序名字
 			arg：可执行程序的相关参数，使用NULL结尾
 		返回值：
			成功：无返回
			失败：-1
 */
		// 子进程执行指定目录下的自己编写的程序，该程序参数
		printf("this is func +++++++++ execl +++++++++\n");
		execl("/home/robin/a.out", "a.out", NULL);
		// 如果执行成功没有返回值
		perror("execl");
		exit(1);
#endif

#if 0
// ++++++++++++++++++++ execlp ++++++++++++++++++++
/*
 * int execlp(const char *file, const char *arg, ...);
		函数描述：加载一个进程，借助PATH环境变量，该函数需要配合PATH环境变量来使用，
						  当PATH中所有目录搜索后没有参数1则出错返回
		函数参数：可执行程序的相关参数，使用NULL结尾
			file：可执行程序的名字
			arg：可执行程序的相关参数，使用NULL结尾
		返回值：
			成功：无返回
			失败：-1
 */
		printf("this is func +++++++++ execlp +++++++++\n");
		execlp("ls", "ls", "-la", NULL);
		// 如果执行成功没有返回值
		perror("execlp");
		exit(1);
#endif

#if 0
// ++++++++++++++++++++ execvp ++++++++++++++++++++
/*
 * int execvp(const char *file, char *const argv[]);
		用法同 execlp
 */
		printf("this is func +++++++++ execvp +++++++++\n");
		char* arg[] = {"ls", "-a", "-l", NULL};
		execvp("ls", arg);
		// 如果执行成功没有返回值
		perror("execvp");
		exit(1);
#endif

#if 0
// ++++++++++++++++++++ execle ++++++++++++++++++++
/*
 *	 int execle(const char *path, const char *arg, ..., char *const envp[]);
 *	 	从用户指定的目录中搜索指定的命令
 */
		printf("this is func +++++++++ execle +++++++++\n");
		char* env[] = {"/bin", "./", "/home/robin", NULL};	// 自己指定环境变量，对应PATH
		execle("/bin/ls", "ls", "-l", "-a", NULL, env);
		// 如果执行成功没有返回值
		perror("execle");
		exit(1);
#endif

#if 0
// ++++++++++++++++++++ execv ++++++++++++++++++++
/*
 *  int execv(const char *path, char *const argv[]);
 *  	用法同：execl
 */
		printf("this is func +++++++++ execve +++++++++\n");
		char* argv[] = {"a.out", NULL};
		execv("/home/robin/a.out", argv);
		// 如果执行成功没有返回值
		perror("execv");
		exit(1);
#endif

#if 1
// ++++++++++++++++++++ execve ++++++++++++++++++++
/*
 *  int execve(const char *path, char *const argv[], char *const envp[]);
 */
		printf("this is func +++++++++ execve +++++++++\n");
		char* argv[] = {"ps", "a", "u", "x", NULL};
		char* env[] = {"/bin", "/home/", "./", NULL};
		execve("/bin/ps", argv, env);
		// 如果执行成功没有返回值
		perror("execve");
		exit(1);
#endif
	}

	else if(pid > 0)
	{
		printf("parent process +++++++++++++++++++\n");
	}

	printf("*****************************\n");

	return 0;
}





