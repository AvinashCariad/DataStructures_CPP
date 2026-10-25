#include <stdio.h>


// main code harness:
int main(int argc, char** argv) {
    int arr[] = {10, 20, 30, 40, 50};

    int length = sizeof(arr) / sizeof(arr[0]);     // how to find the length of the array

    printf("Length = %d\n", length);

    return 0;
}
