#include <stdio.h>

//extern int test_f(double c);
extern double func();

int main(){
    
    //int a = test_f(4);
    printf("%f\n" , func());
    
    return 0;
}
