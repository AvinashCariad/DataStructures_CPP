#include <stdio.h>
#include <stdlib.h>

// handle error:
void handle_error()
{
    printf("Error occured \n");
    exit(1);
}



// count bits:
unsigned int count_num_bits_set(unsigned int n)
{
    unsigned int count = 0;
    
    while (n) {
        n &= n - 1;
        count++;
    }

    return count;
}



// main code harness:
int main(int argc, char** argv)
{
    unsigned int num_bits_set;

    num_bits_set = count_num_bits_set(0);

    if (num_bits_set != 0)
        handle_error();

    num_bits_set = count_num_bits_set(0xffffffff);

    if (num_bits_set != 32)
        handle_error();


    num_bits_set = count_num_bits_set(0x70ff00f);

    if (num_bits_set != 15)
        handle_error();

    printf("Test passed\n");

    return 0;
}

