

#include "first_run.h"
#include "second_run.h"
#include "func.h"
#include "list.h"
#include "label.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*This is the first pass over the assembly code program*/
BoolProcess first_transaction(List* lst, FILE* pf, short* dataWord, short* incWord,char* fileName)

{
	int IC = 0, DC = 0 ,PC = 1, L = 1, checkcmd;
	char cmdline[MAX_COM_LINE], temp[MAX_COM_LINE], tempNoLabel[MAX_COM_LINE];
	char* firstWord = NULL, * afterLabel=NULL, * extraWord=NULL;
	Label* new_label = NULL;
	Node* node;
	Bool errorCheck;
	Psw psw;
	psw.eror_flag = NO;
	psw.label_flag = NO;
	
	
	
	/*Reading a line from the file*/
	while (fgets(cmdline, MAX_COM_LINE, pf))
	{

		psw.label_flag = NO;
		cmdline[strcspn(cmdline, "\n")] = '\0';
		strcpy(temp, cmdline);
		strcpy(tempNoLabel, cmdline);

		/*Checking if an empty line or an prompt line*/
		if (empty_line(cmdline) == YES || isPrompt_line(cmdline) == YES)
		{
			PC++;
			continue;
		}

		/*checking if label*/
		if (strstr(cmdline,":") != NULL)
		{

	

			if (check_if_start_of_label(cmdline) == NO)
			{
				psw.eror_flag = YES;
				printError(spaceBetweenLabelColon, PC);
				PC++;
				continue;
			}

			/*tempNolabel points after the label*/
			strcpy(tempNoLabel, strstr(temp, ":") + 1);

			/*Checks if there is anything after the label*/
			if (empty_line(tempNoLabel) == YES || isPrompt_line(tempNoLabel) == YES)
			{
				psw.eror_flag = YES;
				printError(emptyAfterLabel, PC);
				PC++;
				continue;

			}

			/*firstWord contains the label name*/
			firstWord = strtok(temp, "\t :\n");

			if (legal_label(firstWord,PC) == NO)
			{
				psw.eror_flag = YES;
				PC++;
				continue;
			}
			/*firstWord is legal label name */
			psw.label_flag = YES;
			new_label = get_from_list(lst, firstWord, compare_label);
			/*afterLabel contains the word after label*/
			afterLabel = strtok(NULL, "\t ,\n");

			if (new_label != NULL)
			{
				psw.eror_flag = YES;
				psw.label_flag = NO;
				printError(labelexist, PC);
				PC++;
				continue;
			}
		}

		/*is not a start of label or label*/
		else
		{
			/*afterlabel contains the first word */
			afterLabel = strtok(temp, "\t \n");
		}


		if (strcmp(afterLabel, DATA) == 0 || strcmp(afterLabel, STR) == 0)
		{
			if (psw.label_flag == YES)
			{
				new_label = create_label(firstWord, DC, DATA_TYPE);
				insert_to_list(lst, new_label, compare_label);

			}

			if (strcmp(afterLabel, DATA) == 0)
			{
					
				extraWord = strtok(NULL, "\t, \n");

				if (extraWord == NULL)
				{
					printError(emptyData, PC);
					PC++;
					continue;
				}

				/*We will scan all the numbers after the .data one by one */
				while (extraWord!=NULL) 
				{
					

					if (isValidNum(extraWord,PC) == NO)
					{
						psw.eror_flag = YES;
						extraWord = strtok(NULL, "\t, \n");
						continue;
					}

				
					/*If the number is valid we will insert it into the data table */
					dataWord[DC] = (short)atoi(extraWord);
					DC++;
					extraWord = strtok(NULL, "\t, \n");
				}

					extraWord = tempNoLabel;
					removeSpace(&extraWord);
					extraWord = extraWord + strlen(DATA);

					
					errorCheck = comma_syntax(extraWord, PC);
					if (errorCheck == YES)
					{
						psw.eror_flag = YES;
						PC++;
						continue;
					}


			}

			/*is start of .string*/
			else
			{
				extraWord = strtok(NULL, "\t \n");
				if (extraWord == NULL)
				{
					printError(emptyString, PC);
					PC++;
					IC++;
					continue;
				}

				if (*extraWord != GERSHEIM)		     								
				{
					psw.eror_flag = YES;
					psw.label_flag = NO;
					printError(misingGereshS, PC);
					PC++;
					continue;

				}

				extraWord++;

				while (*extraWord != GERSHEIM && *extraWord != '\0')
				{
					dataWord[DC] = (short)*extraWord; 
					extraWord++;
					DC++;
				}

				if (*extraWord != GERSHEIM)                              
				{
					psw.eror_flag = YES;
					psw.label_flag = NO;
					printError(misingGereshF, PC);
					PC++;

					continue;

				}

				dataWord[DC] = 0;
				DC++;
			}


			PC++;
			continue;
		}

		/*This is not the beginning of DATA or STRING*/
		else if (strcmp(afterLabel, ENT) == 0 || strcmp(afterLabel, EXT) == 0)
		{
			if (strcmp(afterLabel, EXT) == 0)
			{
				extraWord = strtok(NULL, "\t \n");
				if (extraWord == NULL )
				{
					psw.eror_flag = YES;
					psw.label_flag = NO;
					printError(misingLabelExtern, PC);
					PC++;
					continue;
				}

				if (legal_label(extraWord,PC) == NO)
				{
					psw.eror_flag = YES;
					psw.label_flag = NO;
					printError(labelexist, PC);
					PC++;
					continue;
				}

				/*check if alrty exist*/
				new_label = get_from_list(lst, extraWord, compare_label);
				if (new_label != NULL)
				{
					psw.eror_flag = YES;
					psw.label_flag = NO;
					printError(labelexist, PC);
					PC++;
					continue;
				}

				new_label = create_label(extraWord, -1, EXTERNAL_TYPE);
				insert_to_list(lst, new_label, compare_label);

			}

			PC++;
			continue;
		}

		/*if the label is code type*/
		else if (psw.label_flag == YES)
		{

			new_label = create_label(firstWord, IC, CODE_TYPE);
			insert_to_list(lst, new_label, compare_label);
			psw.label_flag = NO;

		}

		
		/*fChecking the name of the command and if it exists */
		checkcmd = check_command(afterLabel);
		if (checkcmd == notvalid)
		{
			psw.eror_flag = YES;
			printError(cmdNotExist, PC);
			PC++;
			continue;
		}

		/*exists, we will update the bits of the opcode*/
		incWord[IC] = set_opcode(checkcmd, incWord[IC]);

		/*temp contains only the operands*/
		strcpy(temp, strstr(tempNoLabel, afterLabel) + strlen(afterLabel) );



		/*If it is an address method 2*/
		if (checkcmd == jmp || checkcmd == bne || checkcmd == jsr)
		{
			
			errorCheck = hendle_Addressing_command2(lst, temp, incWord, checkcmd, IC, PC);
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
			errorCheck = hendle_twoOperands_cmd(lst, temp, incWord, checkcmd, IC, PC);
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

			errorCheck = hendle_oneOperands_cmd(lst, temp, incWord, checkcmd, IC, PC);
			if (errorCheck == YES)
			{
				psw.eror_flag = YES;
				PC++;
				continue;
			}
		}

		/*If it is a command with 0 operands*/
		else if (getCmdtype(checkcmd) == zero_operands)
		{
			errorCheck = hendle_zeroOperands_cmd(lst, incWord, IC, PC);
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

		
		/*Updates the addresses of the DATA type labels*/
		for (node = lst->head;node != NULL; node = node->next)
		{
			new_label = (Label*)node->data;
			if (new_label->type == DATA_TYPE)
			{
				new_label->mem_adr += IC;
			}


		}

		merger(dataWord, incWord, IC, DC);
		rewind(pf);


		if (psw.eror_flag == NO)
		{
			second_transaction(lst, pf, incWord, fileName, DC);
		}
		else
		{
			
			printError(ErrorfirstRun,PC);
			return failure;
		}

		
		
		return success;


	}




/*This function handles commands from addressing method 2 and updates the opcode field accordingly
In addition, it also checks for errors corresponding to the types of the command, if there are any, it will return YES, otherwise it will return NO*/
Bool hendle_Addressing_command2(List* lst, char* operands, short* insWord, int cmdNum, int IC, int PC)
{
	int ICOPCODE;
	char* label, * strSource, * strTarget, * extra, * ptr;
	char temp[MAX_COM_LINE];
	AdrMethod target, source;
	

	ICOPCODE = IC;
	IC++;

	strcpy(temp, operands);
	label = strtok(temp, "\t ()\n");

	if (label == NULL)
	{
		printError(NolabelAfterCmd, PC);
		return YES;
	}

	if (legal_label(label, PC) == NO)
	{
		return YES;
	}

	insWord[ICOPCODE] = set_target(direct, insWord[ICOPCODE]);
	IC++;

	ptr = strstr(operands, label);
	ptr = ptr + strlen(label);

	strSource = strtok(NULL, ", ()\n");
	strTarget = strtok(NULL, "()\n");
	extra = strtok(NULL, "");

	if (extra != NULL)
	{
		if (empty_line(extra) == NO)
		{
			printError(afterCmdNotEmpty, PC);
			return YES;
		}
	}
	
	/*check if only label */
	if (strSource == NULL)
	{
		return NO;
	}
	
	/*check if operand mising*/
	if ((strSource == NULL && strTarget != NULL) || (strSource != NULL && strTarget == NULL))
	{
		printError(numOperandsInvalid, PC);
		return YES;

	}

	if (jmp_syntax(ptr, PC) == YES)
	{
		return YES;
	}
	
	
	else if (strSource != NULL && strTarget != NULL)
	{
		insWord[ICOPCODE] = set_parameter1(operands_Analysis(strSource, lst, PC), insWord[ICOPCODE]);
		insWord[ICOPCODE] = set_parameter2(operands_Analysis(strTarget, lst, PC), insWord[ICOPCODE]);
	}

	source = operands_Analysis(strSource, lst, PC);
	target = operands_Analysis(strTarget, lst, PC);


	if (source == notvalid || target == notvalid)
	{
		return YES;
	}

	update_twoOperands_bit(source, target, strTarget, strSource, insWord, IC);

	return NO;

}





/*This function handles commands with 2 operands and updates the opcode field accordingly
In addition, it also checks for errors corresponding to the types of the command, if there are any, it will return YES, otherwise it will return NO*/
Bool hendle_twoOperands_cmd(List* lst, char* operands, short* insWord, int cmdNum, int IC, int PC)
{
	char* strTarget, * strSource;
	char temp[MAX_COM_LINE];
	AdrMethod target, source;
	int ICOPCODE;


	ICOPCODE = IC;
	IC++;
	strcpy(temp, operands);

	strSource = strtok(temp, "\t ,");
	strTarget = strtok(NULL, "\t ,");

	if (strSource == NULL || strTarget == NULL)
	{
		printError(numOperandsinvalid, PC);
		return YES;
	}

	if ((strSource == NULL && strTarget != NULL) || (strSource != NULL && strTarget == NULL))
	{
		printError(numOperandsinvalid, PC);
		return YES;

	}



	else if (strSource != NULL && strTarget != NULL)
	{
		insWord[ICOPCODE] = set_source(source=operands_Analysis(strSource, lst,PC), insWord[ICOPCODE]);
		insWord[ICOPCODE] = set_target(target=operands_Analysis(strTarget, lst,PC), insWord[ICOPCODE]);
	}

	if (comma_syntax(operands,PC) == YES)
	{
		return YES;
	}

	

	if (source == notvalid || target == notvalid)
	{
		return YES;
	}
	
	if (source != direct && cmdNum == lea)
	{
		printError(sorceCantBeImiOrReg, PC);
		return YES;
	}
	if (target == imidiate && cmdNum != cmp)
	{
		printError(targetCantBeImmediate, PC);
		return YES;
	}

	update_twoOperands_bit(source, target, strTarget, strSource, insWord, IC);

	return NO;

}

/*This function updates the operand words of commands with 2 operands in the 
appropriate place in the data array */
void update_twoOperands_bit(AdrMethod source, AdrMethod target,char* strTarget,char *strSource,short* insWord,int IC)
{

	if ((source == register_d) && (target == register_d))
	{

		insWord[IC] = set_two_register_bit(strSource, strTarget);
		IC++;

	}
	else if (source == register_d)
	{
		insWord[IC] = set_reg_source_bit(strSource);
		IC++;

		if (target == imidiate)
		{
			insWord[IC] = set_imidiate_bit(strTarget);
			IC++;
		}

	}
	else if (target == register_d)
	{
		if (source == imidiate)
		{
			insWord[IC] = set_imidiate_bit(strSource);
			IC++;
		}

		else
		{
			IC++;
		}

		insWord[IC] = set_reg_target_bit(strTarget);
		IC++;
	}
	else
	{
		if (source == imidiate)
		{
			insWord[IC] = set_imidiate_bit(strSource);
			IC++;
		}
		else
		{

			IC++;

		}

		if (target == imidiate)
		{
			insWord[IC] = set_imidiate_bit(strTarget);
			IC++;
		}


	}
}


/*This function handles commands with 1 operands and updates the opcode field accordingly
In addition, it also checks for errors corresponding to the types of the command, if there are any, it will return YES, otherwise it will return NO*/
Bool hendle_oneOperands_cmd(List* lst, char* operands, short* incWord, int cmdNum, int IC, int PC)
{

	AdrMethod target;
	char temp[MAX_COM_LINE];
	char* strTarget, * extra;
	int ICOPCODE;


	ICOPCODE = IC;
	IC++;
	strcpy(temp, operands);


	if (operands == NULL || empty_line(operands) == YES)
	{
		printError(numOperandsinvalid, PC);
		return YES;

	}

	strTarget = strtok(operands, " \t\n");
	extra = strtok(NULL, " \t\n");
	target = operands_Analysis(strTarget, lst, PC);


	if (target == notvalid)
	{
		return YES;
	}


	if (extra != NULL)
	{
		if (empty_line(extra) == NO)
		{
			printError(numOperandsinvalid, PC);
			return YES;
		}
	}
	

	if ((cmdNum != prn) && (target == imidiate))
	{
		printError(targetcantBeImmediate, PC);
		return YES;
	}

	incWord[ICOPCODE] = set_target(target, incWord[ICOPCODE]);

	if (target == register_d)
	{
		incWord[IC] = set_reg_target_bit(strTarget);
	}
	else if (target == imidiate)
	{
		incWord[IC] = set_imidiate_bit(strTarget);
	}


	return NO;
}











short set_opcode(int opcode, short word)
{
	opcode = opcode << MOVE_TO_OPCODE_BIT;
	word = word | opcode;
	return word;
}
short set_source(AdrMethod source, short word)
{
	source = source << MOVE_TO_SOURCE_BIT;
	word = word | source;
	return word;
}

short set_target(AdrMethod target, short word)
{
	target = target << MOVE_TO_TARGET_BIT;
	word = word | target;
	return word;
}
short set_parameter1(AdrMethod source, short word)
{
	source = source << MOVE_TO_PARAMETER1_BIT;
	word = word | source;
	return word;
}
short set_parameter2(AdrMethod target, short word)
{
	target = target << MOVE_TO_PARAMETER2_BIT;
	word = word | target;
	return word;
}








short set_reg_source_bit(char* source)
{
	short word = 0;
	short src;

	src = check_reg(source);
	src = src << 8;

	word = word | src;

	return word;
}
short set_reg_target_bit(char* target)
{
	short word = 0;
	short tar;

	tar = check_reg(target);
	tar = tar << 2;

	word = word | tar;

	return word;
}

short set_two_register_bit(char* source, char* target)
{
	short word = 0;
	short src, tar;

	src = check_reg(source);
	tar = check_reg(target);

	src = src << 8;
	tar = tar << 2;

	word = word | src;
	word = word | tar;

	return word;

}



