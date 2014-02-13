#include <stdio.h>
#include <stdlib.h>

//algorithm sourced from wikipedia page on exponentiation
//Calculate a^(2^n) for n such that 2^n < p. If the binary expansion of p
//contains a 1 in the ith position, multiply the result by a^(2^i), so that
//the final result is a^p.
unsigned long aspow(int a, int p) {
    unsigned long a_2_pows; //the powers of a
    int xps; //int for the 2^n 
    a_2_pows = a; 
    xps = 1;
    unsigned long result = 1; 
    while(1) {
        if(xps>p) break; //stop when we pass the size of the exponent
        if(xps & p) { //check if the exponent has the power of 2 by using bitwise and
            result *= a_2_pows; 
        }
        a_2_pows = a_2_pows * a_2_pows; 
        xps = xps * 2; //increment the powers of 2
        
    }
    return result;
}

int main(int argc, char *argv[]) {
    int base = strtol(argv[1], NULL, 10);
    int exponent = strtol(argv[2], NULL, 10);
    printf("%lu\n", aspow(base,exponent));
    exit(0);
}
    
