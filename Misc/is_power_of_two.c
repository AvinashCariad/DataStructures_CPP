
#include <stdio.h>
#include <stdlib.h>


void handle_error()
{
    printf("Error occured \n");
    exit(1);
}


// power of 2:
int is_power_of_two(unsigned int x)
{
    return (x != 0) && ((x & (x-1)) == 0);
}



// main code harness:
int main(int argc, char** argv)
{
    /*0 is not a power of 2*/
    if (is_power_of_two(0)) 
        handle_error();

    /*10 is not a power of 2*/
    if (is_power_of_two(10)) 
        handle_error();

    /*15 is not a power of 2*/
    if (is_power_of_two(15)) 
        handle_error();


    /*1 is power of 2*/
    if (!is_power_of_two(1)) 
        handle_error();

    /*2 is power of 2*/
    if (!is_power_of_two(2)) 
        handle_error();

    /*4 is a power of 2*/
    if (!is_power_of_two(4)) 
        handle_error();

    /*0x80000000 is a power of 2*/
    if (!is_power_of_two(0x80000000)) 
        handle_error();


    printf("Test passed\n");
    return 0;
}

