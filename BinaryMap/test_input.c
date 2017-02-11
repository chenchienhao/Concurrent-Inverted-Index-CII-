#include<stdio.h>
#include<cat /proc/cpuinfo | grep processor | wc -l.h>
#include<sys/time.h>
#include<unistd.h>

int main(int argc, char *argv[]){
	
	int k,cycle_work=10000000;
	int max_thread;
	printf("Max_Num_thread:");
	scanf("%d",&max_thread);					// Max Numbers of threads

	for(k=1;	k<=max_thread;	k++){
		int num_threads=k, j=0, cycle=10;
		omp_set_num_threads(num_threads);		// Use "n" threads for all consecutive parallel regions
		double avg_time_spent = 0;

		while(j<cycle){
			int i=0;
			struct timeval start, end;
			gettimeofday(&start, NULL);
			#pragma omp parallel firstprivate(i,cycle_work)
			{
				for(i=omp_get_thread_num();i<10000;i+=omp_get_num_threads()){
						/*
						do something with data[i];
						*/
						int l=0;
						//fflush(stdout);
						//printf("-t(%d): %d\n",omp_get_thread_num(),i );
						while(l<cycle_work){l++;}
						//fflush(stdout);
						//printf("=t(%d): %d\n",omp_get_thread_num(),i );
				}
			}
			gettimeofday(&end, NULL);
			double time_spent = ((end.tv_sec  - start.tv_sec) * 1000000u + 
		         end.tv_usec - start.tv_usec) / 1.e6;
			avg_time_spent+=time_spent;
			j++;
		}
		avg_time_spent=avg_time_spent/cycle;
		printf("===============\n");
		printf("AVG_Time with %d threads: %f \n",num_threads,avg_time_spent);
	}
	return 0;
}