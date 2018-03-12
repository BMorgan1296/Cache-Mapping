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

int main()
{
	int l = 5;
	int aL = 100;

	int array[l];

	int i;

	for(i = 0; i < l; i++)
	{
		array[i] = rdtsc();
	}

	for(i = 1; i < l; i++)
	{
		printf("%u\n", array[i]-array[i-1]);
	}	
}