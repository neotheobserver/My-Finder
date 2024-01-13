#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "resultlist.h"

list_t* gp_list_head = NULL;
list_t* gp_list_current = NULL;

list_t* get_list(void)
{
	return gp_list_head;
}

void add_result(result_t* result)
{
	if (NULL == gp_list_head)
	{
		list_t* new_list = (list_t*)malloc(sizeof(list_t));
		assert( NULL != new_list && "Not enough space");
		new_list->result = result;
		new_list->next = NULL;
		gp_list_head = new_list;
		gp_list_current = new_list;
	}
	else
	{
		gp_list_current->next =  malloc(sizeof(list_t));
		assert( NULL != gp_list_current->next && "Not enough space");
		gp_list_current = (list_t*)gp_list_current->next;
		gp_list_current->result = result;
		gp_list_current->next = NULL;
	}
}

result_t* find_result(int index)
{
	list_t* temp = gp_list_head;

	if (NULL == temp)
	{
		return NULL;
	}
	
	while(temp != NULL)
	{
		if (temp->result->index == index)
		{
			return temp->result;
		}
		else
		{
			temp = temp->next;
		}
	}
	
	return NULL;
}

void print_results(void)
{
	list_t* temp = gp_list_head;

	if (NULL == temp)
	{
		return;
	}
	
	while(temp != NULL)
	{
		temp = temp->next;
	}
}

void free_resources(void)
{

	list_t* temp = gp_list_head;

	if (NULL == temp)
	{
		return;
	}
	
	while(temp != NULL)
	{
		free(temp->result->string);
		temp->result->string = NULL;
		free(temp->result->location);
		temp->result->location = NULL;
		temp = gp_list_head->next;
		free(gp_list_head);
		gp_list_head = temp;
	}
	gp_list_head = NULL;
	gp_list_current = NULL;
}
