#include"LinkedListMap.h"
#include"InputMethod.h"
#include<sys/time.h>

int main(int argc, char *argv[]){
	srand48(time(NULL));
	int cycle=3;
	int c, f, i;
	struct timeval start, end;
	//Input File
	char *file_name[1]={"2000000"};
	
	for( f = 0; f < (sizeof(file_name)/sizeof(char*)); ++f){
		char path[80]="../Data/";
		strcat(path,file_name[f]);
		strcat(path,".txt");
		input_array_t input_array = get_input_array(path);			// Get input
		double avg_time_spent = 0;
		
		for( c = 0; c < cycle; ++c){	
			map_t *map;												// New Map
			map=map_init(map);		
			
			gettimeofday(&start, NULL);
			
			for(i=0;i<input_array.num_input;i+=1){
				map_insert(
					map,
					input_array.input[i].key,
					input_array.input[i].value,	
					input_array.input[i].ranking_1,
					input_array.input[i].ranking_2,
					input_array.input[i].frequency);
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
		printf("AVG_Time of file %-7s : Got %8f(seg)  \n",file_name[f],avg_time_spent);
		free_input_array(&input_array);
	}
	return 0;
}