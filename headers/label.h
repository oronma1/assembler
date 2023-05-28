#ifndef _LABEL_
#define _LABEL_


#define MAX_LABEL_SIZE 31

typedef enum
{
	DATA_TYPE,
	CODE_TYPE,
	ENTRY_TYPE,
	EXTERNAL_TYPE

}Label_Type;


typedef struct label
{
	char name[MAX_LABEL_SIZE];
	int mem_adr;
	Label_Type type;

}Label;


/* @ Function: compare_label
   @ Arguments: This function gets void* l1, void* l2
   @ Description: The function compare between two labels and return 0 if Equal
   else if not Equal
   */
int compare_label(void* l1, void* l2);

/* @ Function: create_label
   @ Arguments: This function gets char* label_name, int mem, Label_Type type
   @ Description: The function initializes a place in memory for the path 
	in the label and inserts the received values into the label
	and returns a label type pointer to it
   */
Label* create_label(char* label_name, int mem, Label_Type type);

/* @ Function: free_label
   @ Arguments: This function gets void* label
   @ Description: The function frees the label from memory
   */
void free_label(void* label);




#endif
