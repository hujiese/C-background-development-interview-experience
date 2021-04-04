#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("a.out filename\n");
        exit(1);
    }
    
    struct stat buf_st;
    int ret = lstat(argv[1], &buf_st);
    if(ret == -1)
    {
        perror("stat");
        exit(1);
    }

    printf("file size = %d\n", (int)buf_st.st_size);

    return 0;
}
