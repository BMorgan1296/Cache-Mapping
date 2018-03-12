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

#define CACHE_SIZE 16384
int main()
{	
	int i = 0; //sets skip value for loop
	int s = 0; // interates through skips for loop
	int c = 0; //initalises array to 0 for loop
	uint64_t j = 0; //goes to a million for loop
	char array[CACHE_SIZE+1]; //try without allocation and with allocation, see what happens 


	uint32_t inital;
	uint32_t post;

	int skipLength[6] = {2,4,8,16,32,64};

	for(int skip = 2; skip <= 16384; skip *= 2)
	{
		int nentries = CACHE_SIZE / skip + 1;

		printf("\nSkip length: %d\nnentries: %d", skip, nentries);

		for(c = 0; c < CACHE_SIZE; c++)
		{
			array[c] = 'a';
		}
		
		printf("%s", " Skip Iteration: ");
		printf("%d", s+1);
		inital = rdtsc();

		for(j = 0; j < 1000000; j++) //accesses the array values x amount of times
		{
			uint64_t entry = (j * 16249) % nentries;
			array[entry * skip] ++; //accesses random array value according to a prime number, and the current skipLength
		}

		post = rdtsc();
		printf("%s", " RDTSC difference: ");
		printf("%u\n", post-inital);
	}
        int ss = 0;
        for (int i = 0; i < 100; i++)
            ss+=array[i];
        printf("s is %d (ignore)\n", ss);

}