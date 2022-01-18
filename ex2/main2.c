#include <stdlib.h>
#include <stdio.h>

///this is code in c, your code need to be in assembly
/// good luck!
int even_(int num, int i){
    return num << i;
}


int go_(int A[10]) {
    int sum = 0;
    int i = 0;
    while (i < 10) {
        if (A[i] % 2 == 0) {
            int num = even (A[i], i);
            sum += num;
        } else {
            sum += A[i];
        }
        i++;
    }
    return sum;
}


int main() {
    time_t t;
   
    srand((unsigned) time(&t));

    int array[10];
    for (int i = 0; i < 10; i++) {
        array[i] = rand() % 100;
    }
    
    printf("%d %d\n", go(array), go_(array));

    return 0;
}