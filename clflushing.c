#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <assert.h>
#include <strings.h>


  static inline uint32_t rdtsc() { //without the p as it will crash
  uint32_t rv;
  asm volatile ("rdtsc": "=a" (rv) :: "edx", "ecx"); //these instructions operate on the registers, such as edx (lower than 32 bits), ecx (higher than 32 bits), adx which are registers. 
  return rv; //RDTSC takes timestamp, puts information 
  }

  /*static void clflush(void *v) { //use this with address of variable to see if the variable is not in the cache, use the rdtsc at the same time.
  asm volatile ("clflush 0(%0)": : "r" (v):); //asm is in assembly
  }*/

  static int check(int *i){
  asm volatile ("clflush 0(%0)": : "r" (i):);
  }

  /*static inline void clflush(void *v) { //use this with address of variable to see if the variable is not in the cache, use the rdtsc at the same time.
  asm volatile ("clflush 0(%0)": : "r" (v):); //asm is in assembly
  }*/



int main()
{  
	int *i;
  int *l;

  int j[2];

  j[0] = 1;
  j[1] = 2;

  int k = 0;
  printf("%u\n", check(l));
  l = &k;

  i = &j[0];

	check(i);

  printf("%u\n", check(l));
  k = k+1;
  printf("%u\n", check(l));

  /*printf("%u\n", check(i));
  printf("%u\n", rdtsc());
  printf("%u\n", check(i+1));
  printf("%u\n", rdtsc());*/
}