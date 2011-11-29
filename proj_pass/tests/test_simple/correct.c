#include <stdio.h>

int main(){
    int i = 0;
    int j = 0;
    int a[2] = {1, 2};
    for(;i<1; i++)
        j = a[0];
    printf("j is %d\n", j);
    return 0;
}
