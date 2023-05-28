
#include "label.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




int compare_label(void* l1, void* l2)
{
	Label* pl1 = (Label*)l1;
	char* pl2 = (char*)l2;
	return strcmp(pl1->name, pl2);
}



Label* create_label(char* label_name, int mem, Label_Type type)
{

	Label* label = (Label*)malloc(sizeof(Label));
	if (label == NULL)
	{
		ErrorMemory(memoryAllocate);
	}
	strcpy(label->name, label_name);
	label->mem_adr = mem;
	label->type = type;

	return label;
}

void free_label(void* label)
{
	Label* pl1 = (Label*)label;
	free(pl1);
}
