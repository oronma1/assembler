#ifndef _PRE_ASM_
#define _PRE_ASM_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_COM_LINE 81
#define MAX_FILE_NAME 100
#define START_MACRO_DEFINITION "mcr"
#define END_MACRO_DEFINITION "endmcr"

typedef struct macro_node {
    char* mn;
    char** c_line;
    int size;
}Macro_Node;


/* @ Function: Macro_analysis
   @ Arguments: This function get FILE* file, char* file_name
   @ Description: The function receives a file with the assembly language program in it 
   and searches for all the macro startups and their names in it and continues to search
   for the macro names in the program and instead of the macro names writes all the macro 
   lines from the startups into a file with the .am extension.
    And all the lines that are not a macro or an initialization of a macro will be 
    written as they are to the .am file
   */
void Macro_analysis(FILE* file, char* file_name);

/* @ Function: compare_macro
   @ Arguments: This function gets void* m1, void* m2
   @ Description: The function compares the two received parameters
    and returns 0 if they are equal 
    otherwise if they are different
*/
int compare_macro(void* m1, void* m2);

/* @ Function: add_line
   @ Arguments: This function gets Macro_Node* macro_node, char* line
   @ Description: The function enters into macro_node
    the resulting line
*/
void add_line(Macro_Node* macro_node, char* line);

/* @ Function: add_macro_name
   @ Arguments: This function gets List* list,char* macro_name
   @ Description: The function enters into macro_node
    the resulting macro_name
   */
void add_macro_name(Macro_Node* macro_node, char* macro_name);

/* @ Function: free_macro
   @ Arguments: This function gets void* m
   @ Description: The function frees m from memory
   */
void free_macro(void* m);





#endif
