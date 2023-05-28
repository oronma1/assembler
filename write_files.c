


#include "write_files.h"
#include "label.h"
#include "func.h"
#include "errors.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BEGINNING_LINE 100
#define MSBIT 13

void write_files(List* lst, short* insWord,int IC,int DC,char* fileName)
{

	Node* node;
	Label* new_label;
	int entryType=0;

	write_ob_file(insWord, IC, DC, fileName);

	/*Checks whether labels of type ENTRY exist*/
	for (node = lst->head;node != NULL; node = node->next)
	{

		new_label = (Label*)node->data;
		if (new_label->type == ENTRY_TYPE)
		{
			entryType++;
		}

	}
	if (entryType > 0)
	{
		write_entry_label(lst, fileName);
	}

	

}

void write_ob_file(short* insWord, int IC,int DC,char* fileName)
{
	
	FILE* fp;
	int endLine = IC + DC;
	int line = BEGINNING_LINE;
	int i;
	int j;
	short temp1;
	short temp2;
	char temp[MAX_NAME_FILE];
	short offset = 1 << MSBIT;


	renameFile(temp,fileName,OBJECT_FILE_EXTENSION);
	fp = fopen(temp, "w");

	if (fp == NULL)
	{
		ErrorFile(cantOpenToWrite,temp);
	}

	fprintf(fp,"\nBase 10 address Base 2 code\n\n");
	fprintf(fp,"\t%d %d\n", IC, DC);

	for ( i = 0; i < endLine; i++)
	{
		fprintf(fp, "%.4d	", line);
		temp1 = insWord[i];

			for ( j = 0; j <= MSBIT; j++)
			{
				temp2 = temp1 & offset;

				if (temp2 != 0)
				{
					fprintf(fp, "/");
				}
				else
				{
					fprintf(fp, ".");
				}

				temp1 = temp1 << 1;
			}

		line++;
		fprintf(fp, "\n");


	}
	

		fclose(fp);
}






void write_entry_label(List* lst,char* fileName)
{
	Node* node;
	Label* new_label;
	FILE* fp;
	char temp[MAX_NAME_FILE];

	renameFile(temp,fileName,ENTRY_FILE_EXTENSION);
	fp = fopen(temp, "w");

	if (fp == NULL)
	{
		ErrorFile(cantOpenToWrite, fileName);
	}

	

	for (node = lst->head;node != NULL; node = node->next)
	{

		new_label = (Label*)node->data;
		if (new_label->type == ENTRY_TYPE)
		{
			fprintf(fp, "%s\t%d\n", new_label->name, new_label->mem_adr);
		}
	}
	
	fclose(fp);
	return;

	}
	


