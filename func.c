
#include "func.h"
#include "label.h"
#include "errors.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static const struct 
{
	char* name;
	
}reg[] = {{"r0"},{"r1"},{"r2"},{"r3"},{"r4"},{"r5"},{"r6"},{"r7"}};


static const struct cmd
{
	char* name;
	cWd_type type;


}cWd[] = {	{"mov",two_operands},
		{"cmp",two_operands},
		{"add",two_operands},
		{"sub",two_operands},
		{"not",one_operands},
		{"clr",one_operands},
		{"lea",two_operands},
		{"inc",one_operands},
		{"dec",one_operands},
		{"jmp",one_operands},
		{"bne",one_operands},
		{"red",one_operands},
		{"prn",one_operands},
		{"jsr",one_operands},
		{"rts",zero_operands},
		{"stop",zero_operands}
		
};




Bool empty_line(char* cmdline)
{
	char temp[MAX_COM_LINE], * label;

	strcpy(temp, cmdline);
	label = strtok(temp, "\t ");

	if (label == NULL)
	{
		return YES;
	}

	return NO;
}




Bool isPrompt_line(char* cmdline)
{
	char temp[MAX_COM_LINE], * first;
	strcpy(temp, cmdline);
	first = strtok(temp, "\t ");

	if (first[0] == ';')
	{
		return YES;

	}

	return NO;
}



Bool comma_syntax(char* line, int PC)
{
	char* temp = NULL;
	int numOfComma = 0;
	int numOfOperands = 0;
	temp = line;


	/*check if t*/
	while ((*temp == ',' || isspace(*temp)) && *temp != '\0')
	{
		if (*temp == ',')
		{
			printError(commaBefore, PC);
			return YES;
		}

		temp++;
	}

	while (*temp != '\0')
	{
		if (empty_line(temp) == YES)
		{

			return NO;
		}
		

		while ((*temp != ',' && !isspace(*temp)) && *temp != '\0')
		{
			temp++;
		}

		numOfOperands++;

		if (numOfOperands <= numOfComma)
		{
			printError(extraComma, PC);
			return YES;
		}

		else if (numOfOperands != numOfComma + 1)
		{
			printError(MisingComma, PC);
			return YES;
		}

		while ((*temp == ',' || isspace(*temp)) && *temp != '\0')
		{
			if (*temp == ',')
			{
				numOfComma++;
			}

			temp++;
		}

	}

	

	 if (numOfOperands != numOfComma + 1)
	{
		 printError(comaAfter, PC);
		return YES;
	}

	return NO;
}


Bool isValidNum(char* num, int PC)
{
	char* temp = NULL;
	temp = num;

	if (isspace(*temp) || *temp == '\t')
	{
		printError(spaceAftersol, PC);
		return NO;
	}

	if (*temp == '+' || *temp == '-')
		temp++;

	while (*temp != '\0')
	{
		if (!isdigit(*temp))
		{
			printError(NotDigit, PC);
			return NO;
		}

		temp++;
	}

	return YES;
}


Bool jmp_syntax(char* line, int PC)
{

	char* temp = NULL, * ptr1, * ptr2;
	char tempfi[MAX_COM_LINE];
	
	
	
	temp = line;

	if (*temp != '(')
	{
		if (isspace(*temp) || *temp == '\t')
		{
			temp++;
			if (*temp != '(')
			{
				printError(misingLeftBracket, PC);
				return YES;
			}

			printError(spaceBetweenOperands, PC);
			return YES;
		}

		printError(misingLeftBracket, PC);
		return YES;

	}

	ptr1 = temp;
	ptr2 = strrchr(line, ')');

	if (ptr2 == NULL)
	{
		printError(misingRightBracket, PC);
		return YES;
	}

	temp++;

	while (temp != ptr2)
	{
		if (isspace(*temp))
		{
			printError(spaceBetweenOperands, PC);
			return YES;
		}

		if (*temp == '(')
		{
			printError(extraLeftBracket, PC);
			return YES;
		}

		if (*temp == ')')
		{
			printError(extraRightBracket, PC);
			return YES;
		}

		temp++;
	}

	 strcpy(tempfi, ptr1);
	tempfi[strlen(tempfi)-1] = '\0';
	
	if (comma_syntax(tempfi, PC) == YES)
	{
		return YES;
	}

	return NO;
}

Bool hendle_zeroOperands_cmd(List* lst, short* incWord, int IC, int PC)
{
	char* operands;
	operands = strtok(NULL, "");

	if (operands != NULL)
	{
		printError(numOperandsinvalid, PC);
		return YES;
	}
	return NO;
}







void merger(short* dataWord, short* insWord, int IC, int DC)
{
	int i;

	for ( i = IC; i < IC+DC; i++)
	{
		insWord[i] = dataWord[i - IC];
	}
}





void removeSpace(char** c)
{
	while (isspace(**c))
	{
		(*c)++;
	}
}


/*This function checks if the name of the label is valid*/
Bool legal_label(char* label, int PC)
{
	char temp[MAX_COM_LINE], * ptr;
	ptr = strcpy(temp, label);
	

	while (isspace(*ptr))
	{
		ptr++;
	}

	/*check if the size <= 30*/
	if (strlen(ptr) >= MAX_LABEL_SIZE)
	{
		printError(labelSize, PC);
	}
	
	/*check if the first char is not alpha*/
	if (!isalpha(*ptr))
	{
		printError(firstLabelCharisNotAlpha, PC);
			return NO;
	}
	/*check if command*/
	if (check_command(ptr) != notvalid)
	{
		printError(labelNameLikecmd, PC);
		return NO;
	}
	/*check if reg*/
	if (check_reg(ptr) != notvalid)
	{
		printError(labelNameLikereg, PC);
		return NO;
	}

	/*check if all label is not alpha*/
	while (*ptr != '\0')
	{
		if (!isalpha(*ptr) && !isdigit(*ptr))
		{
			printError(labelNameNotAlpha, PC);
			return NO;
		}
		ptr++;
	}



	return YES;
}



Bool check_if_start_of_label(char* cmdline) 
{
	char temp[MAX_COM_LINE], * label;

	strcpy(temp, cmdline);
	
		if ((label = strstr(temp, ":")) != NULL)
		{
			label--;
			if (isspace(*label))
			{
				return NO;
			}

			return YES;
		}
	
	

	return NO;
}





/*This function checks and analyzes the type of the operand, 
whether it is a label register or a number*/
AdrMethod operands_Analysis(char* type, List* lst, int PC)
{
	int i;

	if (type == NULL)
		return notvalid;

	/*check if reg*/
	for ( i = 0; i < NUM_OF_REG; i++)
	{
		if (strcmp(type,reg[i].name) == 0)
			return register_d;
	}

	/*check if label*/
	if (get_from_list(lst, type, compare_label) != NULL)
		return direct;

	/*check if # exist it can be number*/
	else if (strncmp(type, "#", 1) == 0)
	{
		type++;

		if (isspace(*type) || *type == '\0')
		{
			printError(33, PC);
			return notvalid;
		}

		if (isValidNum(type, PC) == NO)
		{
			return notvalid;
		}

		return imidiate;
	}



	return direct;

}


int check_command(char* test_cmd)
{
	int i;

	for ( i = 0; i <= NUM_OF_CMD; i++)
	{
		if (strcmp(cWd[i].name, test_cmd) == 0)
		{
			return i;
		}
	}
	return -1;

}




cWd_type getCmdtype(int num_cmd)
{
	
		if(cWd[num_cmd].type == one_operands)
		{
			return one_operands;
		}

		else if (cWd[num_cmd].type == two_operands )
		{
			return two_operands;
		}

		else 
		{
			return zero_operands;
		}

	
}


void renameFile(char* temp,char* oldName,const char* type)
{
	strcpy(temp, oldName);
	temp[strcspn(temp, ".")] = '\0';
	strcat(temp, type);
}


short check_reg(char* test_reg)
{
	int i;

	for ( i = 0; i < NUM_OF_REG; i++)
	{
		if (strcmp(reg[i].name, test_reg) == 0)
		{
			return i;
		}
	}
	return -1;
}



/*This function calculates how many words the current poda needs and returns it in L*/

int calc_L(char* cmdline, List* lst, Special_cmd checkcmd)
{

	char temp[MAX_COM_LINE], * strSourse=NULL, * strTarget=NULL, * pivot=NULL, * label=NULL;
	Bool source = notvalid, target = notvalid;
	int L = 0;

	strcpy(temp, cmdline);
	pivot = strtok(temp, "\t ");


	if (checkcmd == jmp || checkcmd == bne || checkcmd == jsr)
	{
		L++;
		label = strtok(NULL, "\t ,()\n");
		strSourse = strtok(NULL, ",()\n");
		strTarget = strtok(NULL, ",()\n");

	}

	else
	{
		strSourse = strtok(NULL, ",\t \n");
		strTarget = strtok(NULL, ",\t \n");
	}

	source = operands_Analysis(strSourse, lst, 0);
	target = operands_Analysis(strTarget, lst, 0);

	L = c_calc_L(source, target, L);

	return L + 1;

}

int c_calc_L(int source, int target, int L)
{
	if (source == register_d)
	{
		L++;

		if (target == register_d || target == notvalid)
		{
			return L;
		}
		else if (target == imidiate || target == direct)
		{
			L++;
			return L;
		}
		return L;
	}

	else if (source == imidiate || source == direct)
	{
		L++;

		if (target == notvalid)
		{
			return L;
		}

		L++;
		return L;

	}
	return L;
}




















