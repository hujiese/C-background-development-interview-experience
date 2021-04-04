/*************************************************************************
	> File Name: temp.c
	> Author: 
	> Mail: 
	> Created Time: 2019年11月07日 星期四 19时51分27秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

char str[6] = "hello";

int main(int argc, char* argv[])
{
   pid_t pid;

   pid = fork();
   
   if(pid == 0)//son
   {
       str[0] = 's';
       printf("son's str is %s . the addr of str is %x.\n", str, (unsigned int)str);
   }
   if(pid > 0)
   {
       sleep(1);
       str[0] = 'f';
       printf("father's str is %s . the addr of str is %x.\n", str, (unsigned int)str);
   }
   return 0; 
}
