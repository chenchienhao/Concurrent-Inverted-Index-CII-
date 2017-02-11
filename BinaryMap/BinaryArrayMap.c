#include"BinaryArrayMap.h"
#include"InputMethod.h"
#include<sys/time.h>


int main(int argc, char *argv[]){
	srand48(time(NULL));
	int cycle=3;
	int c, f, d, e, i;
	struct timeval start, end;
	//Input File
	char *file_name[1]={"2000000"};
	//Usage of Umbral
	int num_umbral[4];
	int array_pointers_umbral[4];
	//Reducing the number of updates
	float reduction=1.2;

	for( f = 0; f < (sizeof(file_name)/sizeof(char*)); ++f){
		char path[80]="../Data/";								//Path of file
		strcat(path,file_name[f]);
		strcat(path,".txt");
		input_array_t input_array = get_input_array(path);			// Get input
		num_umbral[0] = input_array.num_input*0.0001;
		num_umbral[1] = input_array.num_input*0.0002;
		num_umbral[2] = input_array.num_input*0.001;
		num_umbral[3] = input_array.num_input*0.002;

		for( d = 0; d < (sizeof(num_umbral)/sizeof(int)); ++d){
			array_pointers_umbral[0] = 1;
			array_pointers_umbral[1] = 5;
			array_pointers_umbral[2] = 10;
			array_pointers_umbral[3] = 20;

			for(e = 0; e < (sizeof(array_pointers_umbral)/sizeof(int)); ++e){
				double avg_time_spent = 0;

				for( c = 0; c < cycle; ++c){
					map_t *map;													// New Map
					map=map_init(map,num_umbral[d]);

					Array_Pointers_t *array_pointers;
					array_pointers=(Array_Pointers_t *)malloc(sizeof(Array_Pointers_t));
					array_pointers->umbral=array_pointers_umbral[e];
					int size_array_pointers=input_array.num_input/array_pointers->umbral;
								
					array_pointers->node=(node_t **)malloc(sizeof(node_t*)*size_array_pointers);
					array_pointers->size=size_array_pointers;
					array_pointers->size_instant=0;

					gettimeofday(&start, NULL);
					
					for(i=0;i<input_array.num_input;i++){
						map_insert(
							map,	
							input_array.input[i].key,	
							input_array.input[i].value,
							input_array.input[i].ranking_1,	
							input_array.input[i].ranking_2,
							input_array.input[i].frequency,
							reduction,
							array_pointers);
					}
					gettimeofday(&end, NULL);
					/*Output*/
					//if(check_error(map)!=0)
					//	exit(0);
					//print_node(map,"5");
					//print_node(map,"MANZANA");
					//print_map(map);
					double time_spent = ((end.tv_sec  - start.tv_sec) * 1000000u + 
										end.tv_usec - start.tv_usec) / 1.e6;
					avg_time_spent+=time_spent;
					free_map(map);
					free_array_pointers(array_pointers);
				}
				
				avg_time_spent=avg_time_spent/cycle;
				printf("%8f(seg) AVG_Time of file %-7s with map->umbral = %-7d, array->umbral = %-7d;\n",avg_time_spent,file_name[f],num_umbral[d],array_pointers_umbral[e]);
			}
		}
		free_input_array(&input_array);
	}
	return 0;
}