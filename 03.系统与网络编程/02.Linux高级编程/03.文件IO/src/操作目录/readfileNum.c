#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>


int get_file_num(char* root)
{
	int total = 0;
	DIR* dir = NULL;
	// 打开目录
	dir = opendir(root);
	// 循环从目录中读文件

	char path[1024];
	// 定义记录xiang指针
	struct dirent* ptr = NULL;
	while( (ptr = readdir(dir)) != NULL)
	{
		// 跳过. he ..
		if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
		{
			continue;
		}
		// 判断是不是目录
		if(ptr->d_type == DT_DIR)
		{
			sprintf(path, "%s/%s", root, ptr->d_name);
			// 递归读目录
			total += get_file_num(path);
		}
		// 如果是普通文件
		if(ptr->d_type == DT_REG)
		{
			total++;
		}
	}
	closedir(dir);
	return total;
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("./a.out path");
		exit(1);
	}

	int total = get_file_num(argv[1]);
	printf("%s has regfile number: %d\n", argv[1], total);
	return 0;
}
