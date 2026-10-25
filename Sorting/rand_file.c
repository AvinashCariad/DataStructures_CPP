#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// run the following code using the command:  gcc rand_file.c -o rando -lm && ./rando


// Generates an UNBIASED uniform integer in the inclusive range [low, high] using rand()
int uniform_int_distribution_rand(int low, int high) {
    // 1. Calculate the total number of items in our target range
    // Using long long to prevent potential integer overflows during range computation
    long long range = (long long)high - (long long)low + 1;
    
    // 2. Calculate the rejection threshold to eliminate modulo bias.
    // This finds the highest multiple of 'range' that fits completely within RAND_MAX.
    long long limit = (long long)RAND_MAX - ((long long)RAND_MAX % range);          // note the use of RAND_MAX for random num generation
    
    long long random_val;
    do {
        random_val = rand();
    } while (random_val >= limit); // Reject and re-roll if value falls into the biased remainder zone
    
    // 3. Safely map to our target range
    return low + (int)(random_val % range);
}


// Generates gaussian distribution using Box-Muller Trnaform:

// Generates random numbers following a normal (Gaussian) distribution
double generate_gaussian(double mean, double std_dev) {
    static int has_spare = 0;
    static double spare;

    // If we have a cached number from the previous transformation, return it
    if (has_spare) {
        has_spare = 0;
        return mean + std_dev * spare;
    }

    double u1, u2, v1, v2, s;
    
    // Generate two uniform random variables in the open interval (0, 1)
    // We loop to strictly ensure u1 is never exactly 0 to avoid log(0)
    do {
        u1 = (double)rand() / (double)RAND_MAX;
        u2 = (double)rand() / (double)RAND_MAX;
    } while (u1 <= 0.0);
    
    // Apply the core Box-Muller math transformation
    double radius = sqrt(-2.0 * log(u1));
    double theta = 2.0 * M_PI * u2;

    // The transform simultaneously yields two independent Gaussian numbers
    v1 = radius * cos(theta);
    v2 = radius * sin(theta);

    // Cache the second number (v2) for the next call
    spare = v2;
    has_spare = 1;

    // Scale and shift the first number to match the requested mean and std_dev
    return mean + std_dev * v1;
}



// main code harness:
int main(int argc, char** argv) {
    // Seed the standard C random number generator once at application startup
    // this initializes the seed with the start clock time of the host machine 
    // and produces different result everytime it is run
    srand((unsigned int)time(NULL));
    
    int low_bound = 1;
    int high_bound = 6;
    
    printf("Simulating 10 unbiased dice rolls using rand():\n");
    for (int i = 0; i < 10; ++i) {
        int roll = uniform_int_distribution_rand(low_bound, high_bound);
        printf("%d ", roll);
    }
    printf("\n");

    // Generate Gaussians numbers:
    double target_mean = 0.0;
    double target_std_dev = 1.0;

    printf("Generating 10 Gaussian random numbers (Mean = 0, StdDev = 1):\n");
    for (int i = 0; i < 10; ++i) {
        double val = generate_gaussian(target_mean, target_std_dev);
        printf("%f\n", val);
    }



    return 0;
}
