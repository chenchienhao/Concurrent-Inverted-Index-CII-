#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#ifndef InputMethod
#define InputMethod
//Type of input.
typedef struct _input
{
	char* key;
	char* value;
	float ranking_1;
	float ranking_2;
	int frequency;
}input_t;
//Type of file
typedef struct _file
{
	input_t* input;
	int num_key;
}file_t;
//Type of input file
typedef struct _input_file
{
	file_t* file;
	int num_file;
}input_file_t;
//Type of input array 
typedef struct _input_array
{
	input_t* input;
	int num_input;
}input_array_t;
void free_input_file(input_file_t* input_file){
	int i,j;
	for (i = 0; i < input_file->num_file; i++)
	{
		for (j = 0; j < input_file->file[i].num_key; j++)
		{
			free(input_file->file[i].input[j].key);
			free(input_file->file[i].input[j].value);
		}
		free(input_file->file[i].input);
	}
	free(input_file->file);
}
void free_input_array(input_array_t* input_array){
	int i;
	for (i = 0; i < input_array->num_input; i++)
	{
		free(input_array->input[i].key);
		free(input_array->input[i].value);
	}
}
input_array_t get_input_array(char *file_name){
	int num_file,num_key,i,j,k=0,num_input=0;
	int len_key, len_value;
	char lines[200];
	FILE *fp = fopen(file_name, "r");
	fscanf(fp, "%d", &num_file);
	fgets(lines, 200, fp);

	input_file_t input_file;
	input_file.num_file = num_file;
	input_file.file=(file_t*)malloc(sizeof(file_t)*num_file);
	for(i=0; i<num_file; i++)
	{	
		fgets(lines, 200, fp);
		lines[strlen(lines)-1]=0;
		char *value=(char*)malloc(sizeof(char)*(strlen(lines)-2));
		while( lines==strstr(lines,"t: "))
			    memmove(lines,lines+strlen("t: "),1+strlen(lines+strlen("t: ")));
		strcpy(value,lines);
		value[strlen(value)]='\0';
		len_value=(int)strlen(value);
		fscanf(fp, "%d", &num_key);
		fgets(lines, 200, fp);
		input_file.file[i].num_key=num_key;
		input_file.file[i].input=(input_t*)malloc(sizeof(input_t)*num_key);
		num_input+=num_key;
		for (j = 0; j < num_key; j++)
		{
			fgets(lines, 200, fp);
			char *key=(char*)malloc(sizeof(char)*(strlen(lines)+1));
			lines[strlen(lines)-1]=0;
			strcpy(key,lines);
			len_key=(int)strlen(key);
			
			input_file.file[i].input[j].key = (char*)malloc(sizeof(char)*(len_key+1));			
			input_file.file[i].input[j].value = (char*)malloc(sizeof(char)*(len_value+1));
			strcpy(input_file.file[i].input[j].key,key);
			strcpy(input_file.file[i].input[j].value,value);
			input_file.file[i].input[j].ranking_1 = drand48();
			input_file.file[i].input[j].ranking_2 = drand48();
			input_file.file[i].input[j].frequency = rand();
			//printf("\"%s\" \"%s\"\n",input_file.file[i].input[j].key,input_file.file[i].input[j].value );
			free(key);
		}
		free(value);
	}
	fclose(fp);

	input_array_t input_array;
	input_array.num_input=num_input;
	input_array.input = (input_t*)malloc(sizeof(input_t)*num_input);
	for (i = 0; i < input_file.num_file; i++)
	{
		for (j = 0; j < input_file.file[i].num_key; j++)
		{	
			input_array.input[k].key = (char*)malloc(sizeof(char)*(strlen(input_file.file[i].input[j].key)+1));
			input_array.input[k].value = (char*)malloc(sizeof(char)*(strlen(input_file.file[i].input[j].value)+1));
			strcpy(input_array.input[k].key, input_file.file[i].input[j].key);
			strcpy(input_array.input[k].value, input_file.file[i].input[j].value);
			input_array.input[k].ranking_1 = input_file.file[i].input[j].ranking_1;
			input_array.input[k].ranking_2 = input_file.file[i].input[j].ranking_2;
			input_array.input[k].frequency = input_file.file[i].input[j].frequency;
			k++;
		}
	}
	free_input_file(&input_file);
	return input_array;
}
#endif