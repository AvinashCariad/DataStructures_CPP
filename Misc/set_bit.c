#include <stdio.h>
#include <stdlib.h>


// error:
void handle_error()
{
    printf("Error occured \n");
    exit(1);
}


// set bit:
unsigned int set_bit(unsigned int n, unsigned int pos)  
{
    n = n | (1u << pos);

    return n;
}

// reset bit:
unsigned int reset_bit(unsigned int n, unsigned int pos)  
{
    n = n & ~(1u << pos);

    return n;
}



// toggle bit:
unsigned int toggle_bit(unsigned int n, unsigned int pos)  
{
    n = n ^ (1u << pos);

    return n;
}


// main code harness:
int main(int argc, char** argv)
{
    if (set_bit(0xf0, 3) != 0xf8)
        handle_error();

    if (reset_bit(0xf8, 3) != 0xf0)
        handle_error();

    if (toggle_bit(0xf0, 7) != 0x70)
        handle_error();

    printf("Test passed\n");

    return 0;
}

