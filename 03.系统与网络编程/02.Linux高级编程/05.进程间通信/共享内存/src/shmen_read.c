#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>
void error_print(char *msg)
{
	 perror(msg);
	 exit(-1);
}

int main (int argc, char *argv[])
{
	 int ret, i;
	 const char *memname = "/mymem"; //共享内存文件以/开头，可以更好兼容不同系统
	 struct stat statbuf;
	 size_t mem_size = 2 * sysconf(_SC_PAGE_SIZE);
	 int fd = shm_open(memname, O_CREAT|O_TRUNC|O_RDWR, 0666);
	 if (fd == -1)
		error_print("shm_open");
	 ret = ftruncate(fd, mem_size);
	 if (ret != 0)
		error_print("ftruncate");
	 void *ptr = mmap(0, mem_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	 if (ptr == MAP_FAILED)
		error_print("MMAP");
	 close(fd);
	 
	 for(i = 0; i < 20; i++)
	 {
		printf("read data %s\n", (char*)ptr);
		sleep(1);
	 }
	  
	 ret = munmap(ptr, mem_size);
	 if (ret != 0)
		error_print("munmap");
	 ret = shm_unlink(memname);
	 if (ret != 0)
		error_print("shm_unlink");
	 return 0;
}