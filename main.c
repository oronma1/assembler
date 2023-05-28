

#include "first_run.h"
#include "second_run.h"
#include "list.h"
#include "label.h"
#include "func.h"
#include "pre_asm.h"
#include "errors.h"
#include <ctype.h>
#define MAX_NAME_FILE 100

/*This is the main program where the assembler code is handled
Below is the main program process:

Stage 1 involves parsing the source code and replacing macros with their respective contents using a hash table.

In Stage 2, the assembler counts the size of the data and instruction image, adds symbols to the symbol table, and checks for any errors in the code. If any errors occur, the assembler outputs error/warning messages to stderr and error.log, and stops at this stage.

In Stage 3, the assembler updates the addresses of data symbols in the symbol table, allocates memory, and prepares for the second run.

In Stage 4, the assembler writes the words of each line of code in binary format, inserts them into the memory image, and adds external operands to the correct spot in the external operands table. If there is a label operand not in the symbol table and not external, the assembler outputs an error message and continues to discover and report any more such errors.

In Stage 5, if the second run finishes without any errors, the assembler generates all the required outputs. Finally, the assembler moves on to the next source file and repeats the five stages until the last one, when the program ends.
*/


int main(int argc,char* argv[])
{
	List lst;
	FILE* fp;
	int arg=1;
	short word[MAX_LINES] = { 0 } ,data[MAX_LINES] = { 0 };
	char tempfile[MAX_NAME_FILE];
	
	

	/*Checking whether the number of arguments is correct*/
	if( argc < 2)	
	{
		ErrorFile(NoArguments,NULL);
		exit(1); 
	}
	
	
	/*As long as there are arguments*/
	while(arg<argc)
	{
		
		fp = fopen(argv[arg], "r");

	/*If the file does not open or does not exist, we will continue to the next file*/
		if (fp == NULL)
		{
			ErrorFile(cantOpenToRead, argv[arg]);
			arg++;
			continue;
		}

		/*start pre assembler process*/
		start_list(&lst);
		Macro_analysis(fp,argv[arg]);
		fclose(fp);

		renameFile(tempfile,argv[arg],AM_EXTENSION);
		fp = fopen(tempfile, "r");
      
		if (fp == NULL)
		{
			ErrorFile(cantOpenToRead, tempfile);
		}
		
		/*the assembler process*/
		first_transaction(&lst, fp, data, word,tempfile);

		free_list(&lst,free_label);
		fclose(fp);
		arg++;
	}

	return 1;
}
