

#include "pre_asm.h"
#include "list.h"
#include "func.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*Analyzes the macros inside the file and interprets them into a file with the extension .am*/
void Macro_analysis(FILE* as, char* file_name)
{

    List lst;
    Macro_Node* macroNode = NULL;
    char cmdLine[MAX_COM_LINE], temp[MAX_COM_LINE], * ptr = NULL, * s = NULL,tempfilename[MAX_FILE_NAME],temp2[MAX_COM_LINE], templabel[MAX_COM_LINE];
    int flag_macro = 0, i = 0;
    FILE* Amf;
 

    renameFile(tempfilename,file_name,AM_EXTENSION);
    Amf = fopen(tempfilename, "w");
    if (Amf == NULL)
    {
       ErrorFile(cantOpenToRead,file_name);
    }

    start_list(&lst);

    /*Reading lines from the file*/
    while (fgets(cmdLine, MAX_COM_LINE, as))
    {

        cmdLine[strcspn(cmdLine, "\n")] = '\0';
        strcpy(temp, cmdLine);
	strcpy(temp2, cmdLine);
        strcpy(templabel, cmdLine);
        ptr = strtok(cmdLine, "\t \n");

        /*is empty line*/
        if (ptr == NULL)
	{
		fprintf(Amf,"\n");
            continue;
	}
        /*check if can be start of label*/
        if (strstr(temp, ":") != NULL)
        {
	    ptr=strstr(temp, ":");
            ptr++;
            ptr = strtok(ptr, "\t ");
            s = strtok(templabel, ":");
            strcat(s,":");

            /*if after label is empty line*/
            if (ptr == NULL)
            {
                fprintf(Amf, "%s\n", s);
                continue;
            }

            else
            {
		
                /*print label into file Amf*/
                fprintf(Amf, "%s", s);
                /*check if after label is macro*/
                macroNode = (Macro_Node*)get_from_list(&lst, ptr, compare_macro);
                /*ismacro*/
                if (macroNode != NULL)
                {
                    /*print all command line from macro to file*/
                    for (i = 0; i < macroNode->size; i++)
                    {
                        fprintf(Amf, "%s\n", macroNode->c_line[i]);
                    }
                }
                /*is not macro*/
                else
                {
                    s = strstr(temp2, ":");
                    s++;
                    fprintf(Amf, "%s\n", s);
                }
                continue;
            }
        }


        if (flag_macro == 0)
        {

            if (strcmp(ptr, START_MACRO_DEFINITION) == 0)
            {

                macroNode = (Macro_Node*)malloc(sizeof(Macro_Node));

                if (macroNode == NULL)
                {
                    ErrorMemory(memoryAllocate);
                }

                add_macro_name(macroNode, strtok(NULL, "\t "));
                flag_macro = 1;
            }

            /*is not start macro definition*/
            else
            {

                macroNode = (Macro_Node*)get_from_list(&lst, ptr, compare_macro);
                if (macroNode != NULL)
                {

                    for (i = 0; i < macroNode->size; i++)
                    {
                        fprintf(Amf, "%s\n", macroNode->c_line[i]);
                    }
                }
                /*is not macro*/
                else
                {
                    fprintf(Amf, "%s\n", temp);
                }
            }
        }
        /*macro flag = 1*/
        else
        {

            if (strcmp(ptr, END_MACRO_DEFINITION) == 0)
            {
                flag_macro = 0;
                insert_to_list(&lst, macroNode, compare_macro);
            }
            /*is not end macro definition and add the line to the macro*/
            else
            {
                add_line(macroNode, temp);
            }
        }

    }

    free_list(&lst, free_macro);
    fclose(Amf);
}







int compare_macro(void* m1, void* m2)
{
    Macro_Node* mn1 = (Macro_Node*)m1;
    char* mn2 = m2;
    return strcmp(mn1->mn, mn2);
}

void add_line(Macro_Node* macro_node, char* line)
{
    macro_node->c_line = (char**)realloc(macro_node->c_line, sizeof(char*) * (macro_node->size + 1));
    if (macro_node->c_line == NULL)
    {
        ErrorMemory(memoryAllocate);
    }
    macro_node->c_line[macro_node->size] = (char*)malloc(sizeof(char) * (strlen(line) + 1));
    if (macro_node->c_line[macro_node->size] == NULL)
    {
        ErrorMemory(memoryAllocate);
    }
    strcpy(macro_node->c_line[macro_node->size], line);
    macro_node->size++;
}

void add_macro_name(Macro_Node* macro_node, char* macro_name)
{
    macro_node->mn = (char*)malloc(sizeof(char) * (strlen(macro_name) + 1));
    if (macro_node->mn == NULL)
    {
        ErrorMemory(memoryAllocate);
    }
    strcpy(macro_node->mn, macro_name);
    macro_node->size = 0;
    macro_node->c_line = NULL;
}

void free_macro(void* macro)
{
    Macro_Node* macroNode = (Macro_Node*)macro;
    int i;
    for (i = 0; i < macroNode->size; ++i) {
        free(macroNode->c_line[i]);
    }
    free(macroNode->c_line);
    free(macroNode);
}




