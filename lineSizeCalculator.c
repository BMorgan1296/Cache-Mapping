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
#define LOOP_SIZE 2500 //how many loops it will check the values for
#define OUTLIER_MIN 1000 //minimum outlier value for the time between accesses

int main()
{	
	int initialCounter = 0.0;
	int postCounter = 0.0;

	double initialAvg;
	double postAvg;	

	char array[CACHE_SIZE*2];
	char a = '0';

	int *results = (int *)malloc(LOOP_SIZE * sizeof(int));

	for(int skip = 2; skip <= 32; skip *= 2) //access outside cache range, check all values of the last way value before this. If any are longer to access, that cache way has been evicted.
	{
		printf("---Curr Line Size: %d\n", skip);	

		for(int i = 0; i < CACHE_SIZE*2; i++)
		{
			array[i]++;
		}

		for(int i = 0; i < CACHE_SIZE; i++)
		{
			array[i]++;
		}

		for(int loop = 0; loop < 2500; loop++)
		{
			for(int i = 0; i <= skip; i++)
			{						
				for(int j = skip; j < CACHE_SIZE; j++)
				{
					array[j] ++;	
				}					

				uint64_t initial = rdtsc();
				array[CACHE_SIZE] ++;
				uint64_t post = rdtsc();			
				results[loop] = post-initial;			
			}	
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
			

		printf("Outside RDTSC diff: %u || Sample Size: %d\n", (int)avg, sampleSize);
		
		printf("\n\n");
	}
		
	free(results); 



	

	




	/*for(int k = -1; k < 1; k++)
	{
		initialCounter = 0; //resets averages
		postCounter = 0;

		for(int i = 16390; i > -1; i--) //backwards for loop to ensure that 0-16383 is in the cache and the rest is not. This means 16384 is outside cache. Resets every 'difference' iteration.
		{
			array[i] ++;
		}

		a++; //another 'confuser' for the processor, to keep 'a' changing.

		
		for(int j = 0; j < 10000; j++)
		{	
			for(int i = 0; i < 16383; i++)
			{
				array[i]++; //keeps value changing
			}

			initial = rdtsc(); 
			for(int i = 1; i < 5; i++)
			{			
				array[(i*4096)+k] = a+i;
			}	
			post = rdtsc();	

			initialCounter += initial;
			postCounter += post;
		}	

		printf("%s", " CurrVal: ");
		printf("%d", k);
		printf("%s", " Initial: ");
		printf("%u", initialCounter);
		printf("%s", " Post: ");
		printf("%u", postCounter);

		initialAvg = initialCounter/10000.0;
		postAvg = postCounter/10000.0;

		printf("%s", " || InitialAvg: ");
		printf("%lg", initialAvg);
		printf("%s", " PostAvg: ");
		printf("%lg", postAvg);

		printf("%s", " || RDTSC diff: ");
		printf("%u\n", postCounter-initialCounter);

	}*/
	

	    int ss = 0; //confuses optimised compiler by keeping array value changing
        for (int i = 0; i < 100; i++)
            ss+=array[i];
        printf("s is %d (ignore)\n", ss);
}