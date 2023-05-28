

#include "second_run.h"
#include "func.h"
#include "write_files.h"
#include "list.h"
#include "label.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*The main part of the function is to complete the encoding of the label addresses
This function is the second pass through the file after the labels and errors were discovered in this pass we will go through the file once more and identify the labels defined in the file and insert them into the command set in the appropriate place if they were not defined in the file we will issue an error message
If no errors are detected, it will continue to write the files*/

BoolProcess second_transaction(List* lst, FILE* pf, short* incWord,char* fileName,int DC)
{

	
	int IC = 1, PC = 1, L = 0, ret,is_empty;
	char cmdline[MAX_COM_LINE], temp[MAX_COM_LINE], tempNoLabel[MAX_COM_LINE], tempfN[MAX_COM_LINE];
	int checkcmd = notvalid;
	char* firstWord = NULL , * afterLabel=NULL, * extraword=NULL;
	Label* new_label = NULL;
	FILE* fpext;
	Bool errorCheck;
	Psw psw;
	psw.eror_flag = NO;
	psw.label_flag = NO;
	
	
	/*Opening an external file*/
	renameFile(tempfN,fileName,EXTERN_FILE_EXTENSION);
	fpext = fopen(tempfN, "w+");
	
	if (fpext == NULL)
	{
		ErrorFile(1, fileName);
	}
	
	

	/*Reading a line from the file*/
	while (fgets(cmdline, MAX_COM_LINE, pf))
	{

		psw.label_flag = NO;
		cmdline[strcspn(cmdline, "\n")] = '\0';
		strcpy(temp, cmdline);

		if (empty_line(cmdline) == YES || isPrompt_line(cmdline) == YES)
		{
			PC++;
			continue;
		}
		
		/*if label We will continue to the rest of the line*/
		if (check_if_start_of_label(cmdline) == YES)
		{

			strcpy(tempNoLabel, strstr(temp, ":") + 1);
			firstWord = strtok(temp, "\t :");
			afterLabel = strtok(NULL, "\t ,");

		}
		
		else
		{

			strcpy(tempNoLabel, cmdline);
			afterLabel = strtok(temp, "\t ,");
			

		}

		/*if data or string or external We will continue to the next line */
		if (strcmp(afterLabel, DATA) == 0 || strcmp(afterLabel, STR) == 0 || strcmp(afterLabel, EXT) == 0)
		{

			PC++;
			continue;
		}

		/*if .entry we check if the label exist in the file*/
		if (strcmp(afterLabel, ENT) == 0)
		{
			extraword = strtok(NULL, "\t ,");
			new_label = get_from_list(lst, extraword, compare_label);

			if (new_label != NULL)
			{
				new_label->type = ENTRY_TYPE;    
			}
			else
			{
				printError(labelUndefine, PC);
				psw.eror_flag = YES;
			}

			PC++;
			continue;
		}

		checkcmd = check_command(afterLabel);
		/*temp contains only the operands*/
		strcpy(temp, strstr(tempNoLabel, afterLabel) + strlen(afterLabel) );


		/*If it is an address method 2*/
		if (checkcmd == jmp || checkcmd == bne || checkcmd == jsr)
		{
			errorCheck = Update_label_address_Addressing2(lst, temp, incWord, IC, PC, fpext);
			if (errorCheck == YES)
			{
				psw.eror_flag = YES;
				PC++;
				continue;
			}
		}
		/*If it is a command with 2 operands*/
		else if (getCmdtype(checkcmd) == two_operands)
		{
			errorCheck = Update_label_address_twoOperands(lst, temp, incWord, IC, PC, fpext);
			if (errorCheck == YES)
			{
				psw.eror_flag = YES;
				PC++;
				continue;
			}
		}

		/*If it is a command with 1 operands*/
		else if (getCmdtype(checkcmd) == one_operands)
		{

			errorCheck = Update_label_address_oneOperands(lst, temp, incWord, IC, PC, fpext);
			if (errorCheck == YES)
			{
				psw.eror_flag = YES;
				PC++;
				continue;
			}
		}
		
		
		L = calc_L(tempNoLabel, lst, (Special_cmd)checkcmd);
		IC += L;
		PC++;
		continue;

	} 

	

	/*if no error start to print*/
	if (psw.eror_flag == NO)
	{
		
		fseek(fpext, 0, SEEK_END);
		is_empty = ftell(fpext) == 0;
		fseek(fpext, 0, SEEK_SET);
		
		/*check if the external file is empty */
		if (is_empty)
		{	

			ret = remove(tempfN);
			
			if (ret != 0) 
			{
				ErrorFile(cantRemoveFile, tempfN);
			}

		}

		write_files(lst, incWord, IC-1, DC, fileName);
		fclose(fpext);

	}

	else
	{
		printError(secondRunError, PC);
		return failure;
	}

	printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("THE PROCESS COMPLETED SUCCESSFULLY FOR FILE \"%s\" ",fileName);
	printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
	return success;
	
} 







Bool Update_label_address_oneOperands(List* lst, char* operands, short* incWord, int IC, int PC, FILE* fpext)
{

	AdrMethod target;
	char* strTarget;

	strTarget = strtok(operands, " \t\n");
	target = operands_Analysis(strTarget, lst,PC);

	if (target == direct)
	{
		if (set_label_bit(lst, strTarget,fpext,IC,PC) == notvalid)
		{
			return YES;
		}
		else
		{
			incWord[IC] = set_label_bit(lst, strTarget, fpext, IC, PC);
			return NO;
		}

		return NO;

	}
	return NO;
}



Bool Update_label_address_twoOperands(List* lst, char* operands, short* incWord, int IC, int PC, FILE* fpext)
{

	char* strTarget, * strSource;
	AdrMethod target,source;
	

	strSource = strtok(operands, "\t ,");
	strTarget = strtok(NULL, "\t ,");
	source = operands_Analysis(strSource, lst,PC);
	target = operands_Analysis(strTarget, lst,PC);

	
	


	if (source == direct)
	{
		if (set_label_bit(lst, strSource, fpext, IC, PC) == notvalid)
		{
			return YES;
		}

		incWord[IC] = set_label_bit(lst, strSource, fpext, IC, PC);


	}

	IC++;

	if (target == direct)
	{
		if (set_label_bit(lst, strTarget, fpext, IC, PC) == notvalid)
		{
			return YES;
		}
		else
		{
			incWord[IC] = set_label_bit(lst, strTarget, fpext, IC, PC);
			return NO;
		}

	}



	return NO;


}








Bool Update_label_address_Addressing2(List* lst, char* operands, short* incWord, int IC, int PC, FILE* fpext)
{
	char* label, * strSource, * strTarget;
	Label* new_label;
	AdrMethod target, source;
	short temp;
	


	
	
		label = strtok(operands, "\t ,()\n");
		

		new_label = get_from_list(lst, label, compare_label);
		if (new_label == NULL)
		{
			printError(labelUndefine, PC);
			return YES;
		}


		incWord[IC] = set_label_bit(lst, label, fpext, IC, PC);
		IC++;

		strSource = strtok(NULL, ",()\n");
		strTarget = strtok(NULL, ",()\n");

		if (strSource == NULL)
		{
			return NO;
		}

		source = operands_Analysis(strSource, lst,PC);
		target = operands_Analysis(strTarget, lst,PC);

		

		if (source == direct) 
		{
			temp = set_label_bit(lst, strSource, fpext, IC, PC);

			if (temp == notvalid)
			{
				return YES;
			}

			incWord[IC] = temp;
			

		}
		
		IC++;

		if (target == direct)
		{
			temp = set_label_bit(lst, strTarget, fpext, IC, PC);

			if (temp == notvalid)
			{
				return YES;
			}
			else
			{
				incWord[IC] = temp;
				return NO;

			}
			
		}

		return NO;
}

short set_imidiate_bit(char* imid)
{
	short word = 0;

	imid++;
	word = (short)atoi(imid) << 2;
	return word;

}




short set_label_bit(List* lst, char* labelName,FILE* fpext,int IC,int PC)
{
	short word = 0;
	Label* new_label = NULL;
	new_label = get_from_list(lst, labelName, compare_label);

	if (new_label == NULL)
	{
		printError(operandUndefine, PC);
		return -1;
	}

	else if (new_label->type == DATA_TYPE || new_label->type == ENTRY_TYPE)
	{
		
			word = (new_label->mem_adr + 100) << 2;
			return  word | relocatable;
	}

	else if (new_label->type == CODE_TYPE)
	{
		word = (new_label->mem_adr + 100) << 2;
		return  word | relocatable;
	}

	else
	{
		fprintf(fpext, "%s\t%d\n", new_label->name, IC+100);
		return external;
	}
}






