#include <stdio.h>

//extern int test_f(double c);
extern int func();

int main(){
    
    //int a = test_f(4);
    printf("%d\n" , func());
    
    return 0;
}
