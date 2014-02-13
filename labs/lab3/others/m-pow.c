// part2.c
// Max Stolarski
// ./part2 <base> <exponent>
// Takes <base> to the <exponent> power
// referenced http://eli.thegreenplace.net/2009/03/21/efficient-integer-exponentiation-algorithms/

#include <stdlib.h>
#include <stdio.h>

int power(int a, int x);

int main(int argc, char **argv) {
  // get command line arguments as integers
  int a = atoi(argv[1]);
  int x = atoi(argv[2]);
  // output a^x
  printf("%d\n", power(a,x));
  exit(0);
}

// power(a,x) = a^x is defined recursively as shown on http://eli.thegreenplace.net/2009/03/21/efficient-integer-exponentiation-algorithms/
// if x is 1 then return a
// if x is even, then return (a^(x/2))^2
// if x is odd, then return a*(a^(x-1))
int power(int a, int x) {
  if (x == 1)
    return a;
  else if (x % 2 == 0)
    return power(a, (x / 2)) * power(a, (x / 2));
  else if (x % 2 == 1)
    return a * power(a, (x - 1));
  else
    return 0;
}
