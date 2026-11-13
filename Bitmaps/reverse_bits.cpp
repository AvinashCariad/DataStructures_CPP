/*
    Problem:
        Reverse the bits of a number

    Input:
        x = number for reversal     

    Output:
        reversed number 
    TC: O(n), n = no. of bits
*/

#include<iostream>
#include<vector>
#include<cmath>


using namespace std;

// Reverses the bits
int reverseBits(int x){
    // for storing the reversed bits representation
    int reversed_bits = 0;

    while(x){
        // left shift the number
        reversed_bits <<= 1;
        // retrieve the LSB of original and make it the LSB of reversed number 
        reversed_bits |= x & 1;
        // right shift the original number
        x = x >> 1;
    }
    return reversed_bits;
} 


// reverses the input number without extra space - this is not related with the bits
int reverseNumber(int num){
	int reversed_num = 0;

	while(num){
		reversed_num *= 10;
		// add the last digit
		reversed_num += num % 10;
		// remove the last digit from the original num
		num /= 10;
	}

	return reversed_num;
}


// swap bits:
int swapBits(int x, int i, int j){
    // get the ith bit using right shift
    int ith_bit = (x >> i) & 1;
    // get the jth bit using right shift
    int jth_bit = (x >> j) & 1;

    // when the bits are not same
    if( ith_bit != jth_bit){
        // mask for flipping the bits at ith and jth position
        unsigned int flip_mask = (1 << i) | (1<< j);

        // XOR with 1 : flips the bit
        // XOR with 0: Unchanged
        x = x ^ flip_mask;
    }

    return x;
}


// for counting no. of set bits 
// Tc: O(n), n=no. of bits in number representation
short countSetBits(unsigned int x){
    short set_bits = 0;
    // traverse through the bits 
    while(x){
        set_bits += (x & 1);
        x >>= 1;
    }
    return set_bits;
}

// counts set bits
// TC: O(s), s=no. of set bits
// we can efficiently count the set bits by going through only the set bit
// using the x & (x-1) expression we can clear the rightmost set bit
short countSetBits2(unsigned int x){
    int set_bits = 0;   
    while(x){
        set_bits++;
        // clear the rightmost bit
        x = x & (x-1);                 // clears the rightmost bit
    }

    return set_bits;
}


// Parity Bits:
// sets parity by looking at the odd bits. We take XOR of set bits
// XOR of odd no. of 1s is 1 and 0 for even no. of 1s.
// TC:O(n), n=no. of bits 
bool parity1(unsigned int x){
    bool parity = 0;

    while(x){
        parity = parity ^ (x & 1);
        x >>= 1;
    }
    return parity;
}

// Since XOR is associative as well as commutative, we can use that fact to 
// divide the bit pattern into segments and subsegments. We find the XOR for the
// smallest segment and work our way up
// Tc: O(logn), n=no. of bits
bool parity2(unsigned long  x){
   // x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    
    // retrieve the final parity bit
    return x & 0x1;
}

// We traverse only the set bits
// Tc: O(k), k=no. of set bits
bool parity3(unsigned int x){
    short parity = 0;
    // the loop runs for every set bit, so we are traversing 
    // and counting only the set bits 
    while(x){
        // XOR the result with 1
        parity ^= 1;
        // clear the current rightmost set bit
        x = x & (x - 1); 
    }
    return parity;
} 




// main code harness: - check all the codes above
int main(int argc, char** argv){
    cout <<"reversed bit representation in decimal would be: " <<reverseBits(11) << std::endl;
    int num = 123;
    cout << reverseNumber(num) << std::endl;

    cout << swapBits(5, 1, 2);

    cout << countSetBits(0) << ", " << countSetBits2(0) << endl;
    cout << countSetBits(1) << ", " << countSetBits2(1) << endl;
    cout << countSetBits(7) << ", " << countSetBits2(7) << endl;

    // check the parity code:
    for(size_t i = 0; i < 10; i++){
        cout << i << ": " << parity1(i) << ", " 
                << parity2(i) << ", " << parity3(i) << endl;
    } 


    cout << "===========SUCCESS==========" << endl;


    return 0;
}
