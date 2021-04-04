#include <stdio.h>
#include "head.h"

int main(void)
{
    int a = 25;
    int sum = add(a, a);
    printf("========== =_= ==========\n");
    printf("%d + %d = %d\n", a, a, sum);
    printf("=========================\n");
    return 0;
}
