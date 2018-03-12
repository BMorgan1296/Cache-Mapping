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
	int initialCounter = 0.0;
	int postCounter = 0.0;

	double initialAvg;
	double postAvg;	

	char array[CACHE_SIZE*2];
	char a = '0';

	for(int skip = 2; skip <= 16; skip *= 2) //access outside cache range, check all values of the last way value before this. If any are longer to access, that cache way has been evicted.
	{
		printf("---Number of Ways: %d || Way Size: %d---\n", skip, CACHE_SIZE/skip);

		for(int i = CACHE_SIZE*2; i > -1; i--)
		{
			array[i] = a;
		}

		for(int i = 0; i < CACHE_SIZE*2; i++)
		{
			array[i]++;
		}

		for(int i = 0; i < CACHE_SIZE; i++)
		{
			array[i]++;
		}

		for(int i = 0; i < skip; i++)
		{						
			uint64_t initial = rdtsc();
			array[i*(CACHE_SIZE/skip)] ++;
			uint64_t post = rdtsc();			

			printf("Current Way: %d || Current Val: %d || Time: %u\n", i, i*(CACHE_SIZE/skip), post-initial);
		} 

		for(int i = 0; i < CACHE_SIZE; i++)
		{
			array[i]++;
		}
		
		uint64_t initial = rdtsc();
		array[CACHE_SIZE+1]++;
		uint64_t post = rdtsc();
		printf("Access Value outside Cache: %d || Time: %u\n", CACHE_SIZE+2, post-initial);	
		
		
			

		for(int i = 0; i < skip; i++)
		{						
			uint64_t initial = rdtsc();
			array[(i*(CACHE_SIZE/skip))+2] ++;
			uint64_t post = rdtsc();			

			printf("Current Way: %d || Current Val: %d || Time: %u\n", i, i*(CACHE_SIZE/skip)+2, post-initial);
		} 

		printf("\n");
		
	}	

	

	/*for(int skip = 2; skip <= 64; skip *= 2)
	{
		int nentries = CACHE_SIZE / skip + 1;

		printf("\n\nSkip: %d", skip);

		printf("\nSkip length: %d nentries: %d\n", skip, nentries);

		for(int i = 0; i < skip+1; i++)
		{
			printf("%d\n", skip);

			for(int i = 20000; i > -1; i--)
			{
				array[i]++;
			}

			initial = rdtsc();
			array[i*(CACHE_SIZE/skip)]++;
			post = rdtsc();

			printf("%s", " CurrVal: ");
			printf("%d", i*(CACHE_SIZE/skip));
			printf("%s", " Initial: ");
			printf("%u", initial);
			printf("%s", " Post: ");
			printf("%u", post);
			printf("%s", " || RDTSC diff: ");
			printf("%u\n", post-initial);
		}
	}*/

	

	




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