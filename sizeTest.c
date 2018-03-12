#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <assert.h>
#include <strings.h>
#include <math.h>


static inline uint32_t rdtsc() //without the p as it will crash
{ 
	uint32_t rv;
	asm volatile ("rdtsc": "=a" (rv) :: "edx", "ecx"); //these instructions operate on the registers, such as edx (lower than 32 bits), ecx (higher than 32 bits), adx which are registers. 
	return rv; //RDTSC takes timestamp, puts information 
}

#define LOOP_SIZE 2500 //how many loops it will check the values for
#define OUTLIER_MIN 1000 //minimum outlier value for the time between accesses

int MIN_SEARCH;
int MAX_SEARCH;

//also MAX_SEARCH cache search limit


int main()
{	
	printf("\nMASTIK L1 Cache Size Finder\n\nInput Minimum Search Size in kilobytes (recommended 2, minimum 1):\n");
	scanf("%d", &MIN_SEARCH);
	printf("\nInput Maximum Search Size in kilobytes (recommended 32, maximum 256):\n");
	scanf("%d", &MAX_SEARCH);
	if(MAX_SEARCH > 256)
	{
		MAX_SEARCH = 256;
	}

	if(MIN_SEARCH < 1)
	{
		MIN_SEARCH = 1;
	}

	if(MIN_SEARCH = MAX_SEARCH)
	{
		printf("\nSPECIFIED CACHE MINIMUM HALVED, CANNOT BE SAME VALUE AS MAX\n");
		MIN_SEARCH = MIN_SEARCH/2;
	}

	MIN_SEARCH = MIN_SEARCH * 1024;
	MAX_SEARCH = MAX_SEARCH * 1024;

	printf("\n*****************************\n       -Begin Search-\n*****************************\n");


	int CACHE_SIZE = 2;

	char array[MAX_SEARCH];
	int *results = (int *)malloc(LOOP_SIZE * sizeof(int));
	int skipCounter = 0;



	for(int skip = MIN_SEARCH; skip <= MAX_SEARCH; skip *= 2) //skips to search for different cache sizes
	{
		skipCounter++;
	}

	int *avgFirst = (int *)malloc(skipCounter);
	int *avgSecond = (int *)malloc(skipCounter);

	int currSkipCount = 0;
	
	for(int skip = MIN_SEARCH; skip <= MAX_SEARCH; skip *= 2) //skips to search for different cache sizes
	{
		for(int k = skip-1; k < skip+1; k++) //tries suspected last byte of cache and the next byte of cache which could be outside
		{
			for(int j = 0; j < LOOP_SIZE; j++)
			{
				for(int i = MAX_SEARCH; i > -1; i--) //resets array so the bottom part is in the cache definitely
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
				if(results[i] < OUTLIER_MIN) //gets rid of any outliers.
				{
					counter += results[i];
					sampleSize++;
				}			
			}
			double avg = counter / sampleSize; //avg finder		

			if(k == skip-1)
			{
				avgFirst[currSkipCount] = (int)avg;
			}
			if(k == skip)
			{	
				avgSecond[currSkipCount] = (int)avg;
			}		
			printf("Current Cache Size Test: %d KB || RDTSC diff: %u || Sample Size %d\n", k/1024, (int)avg, sampleSize);
			//printf("Current Cache Size Test: %d || RDTSC diff: %u\n", k, (int)avg);
		}
		currSkipCount++;
		printf("\n");
	}		

	int ss = 0; //confuses optimised compiler by keeping array value changing
	for (int i = 0; i < 100; i++)
	    ss+=array[i];	

	for(int i = 1; i <= skipCounter; i++)
	{		
		//printf("%d %d", avgFirst[i], avgSecond[i]);

		if(avgSecond[i] > avgFirst[i]+15) //buffer of 15 cycles to be defined as outside of cache
		{
		 	int powerCounter = 0;
			for(int j = 2; j < MAX_SEARCH; j*=2)
			{
				if(powerCounter == i)
				{
					j = MAX_SEARCH+1;
					break;
				}
				CACHE_SIZE = CACHE_SIZE*2;
			}
			printf("Size of Cache is %d bytes", CACHE_SIZE);
			break;
		}
		else
		{
			printf("*** Not enough information to calculate cache size. Use user override ***");
			break;
		}
	}
	char input;
	int num;
	printf("\nUser Override: y/n\n");
	scanf("\n%c", &input);

	if(input == 'y')
	{
		printf("Input cache value in kilobytes:\n");	
		scanf("%d", &num);	
		CACHE_SIZE = num*1024;
		printf("\nCache size is now %d bytes\n", CACHE_SIZE);
	}

	free(results); 
	free(avgFirst); 
	free(avgSecond); 
}