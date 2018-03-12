#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <assert.h>
#include <strings.h>


static inline uint32_t rdtsc() //without the p as it will crash
{ 
	uint32_t rv;
	asm volatile ("rdtsc": "=a" (rv) :: "edx", "ecx"); //these instructions operate on the registers, such as edx (lower than 32 bits), ecx (higher than 32 bits), adx which are registers. 
	return rv; //RDTSC takes timestamp, puts information 
}

#define ARRAY_SIZE 32768 //also max cache search limit
#define MIN_SEARCH 512 //must be power of 2
#define LOOP_SIZE 2500 //how many loops it will check the values for
#define OUTLIER_MIN 1000 //minimum outlier value for the time between accesses

int main()
{		
	char array[ARRAY_SIZE];
	int *results = (int *)malloc(LOOP_SIZE * sizeof(int)); //holds results from the loop below
	
	for(int skip = MIN_SEARCH; skip <= ARRAY_SIZE; skip *= 2) //skips to search for different cache sizes
	{
		for(int k = skip-1; k < skip+1; k++) //tries suspected last byte of cache and the next byte of cache which could be outside
		{
			for(int j = 0; j < LOOP_SIZE; j++)
			{
				for(int i = ARRAY_SIZE; i > -1; i--) //resets array so the bottom part is in the cache definitely
				{
					array[i]++;
				}	

				uint64_t initial = rdtsc();
				array[k] ++;
				uint64_t post = rdtsc();
				results[j] = post-initial;
			}	

			int counter = 0;
			int sampleSize = 0;

			for(int i = 0; i < LOOP_SIZE; i++) 
			{
				if(results[i] < OUTLIER_MIN) //gets rid of any outliers
				{
					counter += results[i];
					sampleSize++; //will show if any outliers occurred if it is not the OUTLIER_MIN value
				}			
			}
			double avg = counter / sampleSize; //avg finder
			printf("Skip: %d || RDTSC diff: %u || Sample Size: %d\n", k, (int)avg, sampleSize);
		}
		printf("\n");
	}	
	free(results); 

	int ss = 0; //confuses optimised compiler by keeping array value changing
	for (int i = 0; i < 100; i++)
	    ss+=array[i];
	printf("s is %d (ignore)\n", ss);
}