#ifndef __RESULT_LIST__
#define __RESULT_LIST__

typedef struct
{
	int index;
	char *string;
	char *location;
} result_t;

typedef struct
{
	result_t* result;
	void* next;
} list_t;

list_t* get_list(void);
void add_result(result_t* result);
result_t* find_result(int index);
void print_results(void);
void free_resources(void);

#endif
