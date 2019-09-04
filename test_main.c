#include <stdio.h>

extern int test_f(double c);

int main(){
    
    int a = test_f(4);
    printf("%d\n" , a);
    
    return 0;
}
