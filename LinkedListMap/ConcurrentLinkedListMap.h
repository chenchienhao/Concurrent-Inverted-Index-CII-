#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<omp.h>
#include<unistd.h>
#ifndef ConcurrentLinkedListMap
#define ConcurrentLinkedListMap
struct _node;
//Type key of term.
typedef struct _data_map
{
	struct _node *value;
	int size;
}_data_map;
//Type value with information
typedef struct _data_list
{
	float ranking_1;
	float ranking_2;
	int frequency;
}_data_list;
//Union usage
typedef union _private_data
{
	struct _data_map data_map;
	struct _data_list data_list;
}private_data;
//Type Node
typedef struct _node{
	omp_lock_t lock;
	char *key;
	private_data data;
	struct _node *next;
}node_t;
//Type Map
typedef struct _map{
	node_t head;
	int size;
}map_t;
//Check a key value
int IS_SAME_KEY(char *key, char *value){
	if (strcmp(key,value)!=0)
		return 0;
	return 1;
}
void print_node(map_t *map, char *key){
	node_t *node=map->head.next;
	int i,key_found=0;
	while(!IS_SAME_KEY(node->key,"")){
		if(IS_SAME_KEY(node->key,key)){
			key_found=1;
			break;
		}
		node=node->next;
	}
	if (key_found==1)
	{
		node_t *sub_node=node->data.data_map.value->next;
		printf("\n==========================List Pointers==========================\n");
		sub_node=node->data.data_map.value->next;
		while(strcmp(sub_node->key,"")!=0){
			printf("\"%10s\"(%f)\n",sub_node->key,sub_node->data.data_list.ranking_1);
			if (strcmp(sub_node->next->key,"")!=0)
				printf("           |\n           v\n");
			sub_node=sub_node->next;
		}
		printf("==========================%s size:%d==================================\n     ", key,node->data.data_map.size);		
		printf("\n");
	}
}
void print_map(map_t *map){
	int i;
	node_t*node=map->head.next;
	printf("\n==========================List Pointers==========================\n");
	node=map->head.next;
	while(strcmp(node->key,"")!=0){
		fflush(stdout);
		printf("\"%10s\"\n",node->key);
		if (strcmp(node->next->key,"")!=0){
			fflush(stdout);
			printf("       |\n       v\n");
		}
		node=node->next;
	}
	printf("\n==========================MAP size:%d==========================\n", map->size);
	printf("\n");
}
//Free all elements
void free_map(map_t *map){
	int i;
	node_t*node;
	node_t*free_node;
	node=map->head.next;
	
	while(strcmp(node->key,"")!=0){
		free_node=node;
		//Free Sub nodes
		node_t *sub_node=node->data.data_map.value->next;
		node_t *free_sub_node;

		while(sub_node->data.data_list.ranking_1!=1){
			free_sub_node=sub_node;
			sub_node = sub_node->next;
			omp_destroy_lock(&free_sub_node->lock);
			free(free_sub_node->key);
			free(free_sub_node);
			
		}
		omp_destroy_lock(&sub_node->lock);
		free(sub_node->key);
		free(sub_node);
		node=node->next;
		//Free principal node
		omp_destroy_lock(&free_node->lock);
		free(free_node->key);
		free(free_node);
		
	}
	omp_destroy_lock(&map->head.lock);
	free(map->head.key);
	free(map);
}
int check_input(char*key, char*value){
	int error=0;
	if(strcmp(key,"")==0)
		error=1;
	if(strcmp(value,"")==0)
		error=1;
	if(strstr(key,",")!=NULL)
		error=1;
	if(strstr(value,",")!=NULL)
		error=1;
	if(error==1)
		printf("Input Error:\"%s\" \"%s\" \n", key, value);
	return error;
}
//Check if map has error in key.
int check_error(map_t *map){
	int error=0, index=1, sub_index;
	node_t*node=map->head.next;
	node_t*node_next;
	node_t*sub_node;
	node_t*sub_node_next;
	//Checking principal list node
	while(strcmp(node->key,"")!=0){
		node_next=node->next;
		if ((strcmp(node_next->key,node->key)<0)&&(!IS_SAME_KEY(node_next->key,""))){
			error=1;
			break;
		}
		//Checking principal list sub-node
		sub_node=node->data.data_map.value->next;
		sub_index=1;
		while(strcmp(sub_node->key,"")!=0){
			sub_node_next=sub_node->next;
			if (sub_node_next->data.data_list.ranking_1 > sub_node->data.data_list.ranking_1
			  &&sub_node_next->data.data_list.ranking_1 != 1){
				error=2;
				break;
			}	
			sub_node=sub_node_next;
			sub_index++;
		}
		if (error==2)
			break;
		node=node_next;
		index++;
	}
	if(error==1){
		print_map(map);
		printf("Had a error at index[%d]: %s.\n",index,node->next->key);
	}
	if(error==2){
		print_node(map,node->key);
		printf("Had a error at index[%d]: %s >> sub_node[%d]: %s\n",index,node->key,sub_index,sub_node->next->key);
	}
	return error;
}
//Copy and pass string to another variable.
void CopyString(char *strCopy, char *strOriginal){
	strcpy(strCopy,strOriginal);
}
//Initial a map.
map_t *map_init(map_t *map)
{
    int i;
    map=(map_t*)malloc(sizeof(map_t));
    omp_init_lock(&map->head.lock);
    map->head.key = (char *)malloc(sizeof(char));
    map->head.key[0] = '\0';
    map->head.next = &map->head;

    map->size = 0;
    return map;
}
//Insert/update a node.
void map_insert(
	map_t *map,
	char *key,
	char *value,
	float ranking_1,
	float ranking_2,
	int frequency,
	int node_inserted[])
{
	node_t *current_node;
	node_t *lock_node;
	node_t *repeat_node;
	int i, key_repeat;
	//Search location for new node, return node neighbor.
	key_repeat=0;
	current_node = &map->head;
	while(1){
		lock_node = current_node;
		while(!omp_test_lock(&lock_node->lock));
		if(strcmp(current_node->next->key,key)<=0){
			if(IS_SAME_KEY(current_node->next->key,key)){
				repeat_node=current_node->next;
				key_repeat=1;
				break;
			}
			if(IS_SAME_KEY(current_node->next->key,""))
				break;
			current_node=current_node->next;
		}
		else
			break;
		omp_unset_lock(&lock_node->lock);
	}
	//Case if key is repeat.
	if (key_repeat==1){
		omp_unset_lock(&lock_node->lock);
		key_repeat=0;
		node_t *node=repeat_node;
		node_t *sub_node=node->data.data_map.value;
		node_t *rank_node=node->data.data_map.value;
		int ranking=0;
		
		while(1){
			lock_node=sub_node;
			while(!omp_test_lock(&lock_node->lock));

			if(	sub_node->data.data_list.ranking_1 >= ranking_1
			&&	(ranking_1 >= sub_node->next->data.data_list.ranking_1
			||	sub_node->next->data.data_list.ranking_1 == 1)){
				rank_node=sub_node;
				ranking=1;
			}
			if (IS_SAME_KEY(sub_node->key,value)){
				key_repeat=1;
				break;
			}

			if(IS_SAME_KEY(sub_node->next->key,""))
				break;

			sub_node=sub_node->next;


			if(ranking!=1)
				omp_unset_lock(&lock_node->lock);

			if(ranking==1)
				ranking=-1;
		}

		if (key_repeat==0)
		{
			sub_node=rank_node;

			node_t *new_sub_node=(node_t*)malloc(sizeof(node_t));
			omp_init_lock(&new_sub_node->lock);
			new_sub_node->key=(char *)malloc(sizeof(char)*(strlen(value)+1));
			CopyString(new_sub_node->key, value);
			new_sub_node->data.data_list.ranking_1=ranking_1;
			new_sub_node->data.data_list.ranking_2=ranking_2;
			new_sub_node->data.data_list.frequency=frequency;
			
			new_sub_node->next=sub_node->next;
			sub_node->next=new_sub_node;
			
			node->data.data_map.size++;
					
		}
		if(!IS_SAME_KEY(rank_node->key,lock_node->key))
				omp_unset_lock(&rank_node->lock);
		omp_unset_lock(&lock_node->lock);
	}
	else{
		node_t *insert_node;
		insert_node = (node_t *)malloc(sizeof(node_t));
		omp_init_lock(&insert_node->lock);
		insert_node->key=(char *)malloc(sizeof(char)*(strlen(key)+1));
		CopyString(insert_node->key, key);
		
		//First node of value
		node_t *sub_node;
		sub_node=(node_t*)malloc(sizeof(node_t));
		omp_init_lock(&sub_node->lock);
		sub_node->key=(char *)malloc(sizeof(char)*(strlen(value)+1));
		sub_node->data.data_list.ranking_1=ranking_1;
		sub_node->data.data_list.ranking_2=ranking_2;
		sub_node->data.data_list.frequency=frequency;
		
		CopyString(sub_node->key, value);
		//Header of sub_node
		node_t *header;
		header=(node_t*)malloc(sizeof(node_t));
		omp_init_lock(&header->lock);
		header->key=(char *)malloc(sizeof(char));
		CopyString(header->key, "");
		header->data.data_list.ranking_1=1;
		header->data.data_list.ranking_2=1;
		header->data.data_list.frequency=1;
		header->next=sub_node;
		sub_node->next=header;
		insert_node->data.data_map.value=header;
		//Defaults pointers 
		insert_node->data.data_map.size=1;
		insert_node->next=current_node->next;
		current_node->next=insert_node;
		node_inserted[omp_get_thread_num()]++;		
		
		omp_unset_lock(&lock_node->lock);
	}
}
#endif
