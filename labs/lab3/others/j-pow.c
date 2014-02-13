// Author: Jesse Silliman
// pow.c     
// Purpose:  A program to more efficiently exponentiate integers
// Arguments: ./pow <base> <power> 

#include <stdlib.h>
#include <stdio.h>

// http://en.wikipedia.org/wiki/Exponentiation_by_squaring
// I precisely implemented the mathematical recursive definition.
long power(int x, int n) {
  int result = 1;
  if (n == 0) {
    return 1;
  } else if (n % 2 == 0) {
    long half = power(x, n/2);
    return half*half;
  } else {
    long half = power(x, (n-1)/2);
    return x*half*half;
  }
  return 0;
} 

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("You must specify two arguments: pow <base> <power>\n");
  } else {
    int x = strtol(argv[1], NULL, 10);
    int n = strtol(argv[2], NULL, 10);
    printf("%ld \n", power(x,n));
  } 
}

