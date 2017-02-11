#include"BinaryListMap.h"
#include"InputMethod.h"
#include<sys/time.h>

int main(int argc, char *argv[]){
	srand48(time(NULL));
	int cycle=3;
	int c, f, d, i;
	struct timeval start, end;
	//Input File
	char *file_name[1]={"2000000"};
	//Usage of Umbral
	int num_umbral[4];
	//Reducing the number of updates
	float reduction=1.2;

	for( f = 0; f < (sizeof(file_name)/sizeof(char*)); ++f){
		char path[80]="../Data/";//Path of file
		strcat(path,file_name[f]);
		strcat(path,".txt");
		input_array_t input_array = get_input_array(path);			// Get input
		num_umbral[0] = input_array.num_input*0.0001;
		num_umbral[1] = input_array.num_input*0.0002;
		num_umbral[2] = input_array.num_input*0.001;
		num_umbral[3] = input_array.num_input*0.002;
		
		for( d = 0; d < (sizeof(num_umbral)/sizeof(int)); ++d){
			double avg_time_spent = 0;
			
			for( c = 0; c < cycle; ++c){
				map_t *map;										// New Map
				map=map_init(map,num_umbral[d]);					
				
				gettimeofday(&start, NULL);
				
				for(i=0;i<input_array.num_input;i+=1){
					map_insert(
						map,
						input_array.input[i].key,
						input_array.input[i].value,
						input_array.input[i].ranking_1,
						input_array.input[i].ranking_2,
						input_array.input[i].frequency,
						reduction);
				}
				gettimeofday(&end, NULL);
				/*Output*/
				//if(check_error(map)!=0)
				//	exit(0);	
				//print_node(map,"MANZANA");
				//print_node(map,"5");
				//print_map(map);
				double time_spent = ((end.tv_sec  - start.tv_sec) * 1000000u + 
				        end.tv_usec - start.tv_usec) / 1.e6;
				avg_time_spent+=time_spent;
				free_map(map);
			}
			avg_time_spent=avg_time_spent/cycle;
			printf("%8f(seg) in AVG_Time of file %-7s with umbral = %-7d;  \n",avg_time_spent,file_name[f],num_umbral[d]);
		}
		free_input_array(&input_array);
	}
	return 0;
}