#ifndef _FUNC_
#define _FUNC_


#include "list.h" 



#define ENTRY_FILE_EXTENSION ".ent"
#define EXTERN_FILE_EXTENSION ".ext"
#define OBJECT_FILE_EXTENSION ".ob"
#define AM_EXTENSION ".am"
#define DATA ".data"
#define STR ".string"
#define ENT ".entry"
#define EXT ".extern"
#define MAX_LINES 256
#define MAX_LABEL_SIZE 31
#define NUM_OF_REG 8
#define NUM_OF_CMD 15
#define NUM_OF_COMMAND 10
#define SOL '#'
#define MAX_NAME_FILE 100
#define MAX_COM_LINE 81
#define GERSHEIM '\"'
#define MOVE_TO_OPCODE_BIT 6
#define MOVE_TO_SOURCE_BIT 4
#define MOVE_TO_TARGET_BIT 2
#define MOVE_TO_PARAMETER1_BIT 12
#define MOVE_TO_PARAMETER2_BIT 10



typedef enum
{
	absolute,
	external,
	relocatable

}ARE;



typedef enum bool
{
	YES,
	NO
	

}Bool;

typedef enum 
{
	success,
	failure

}BoolProcess;

typedef struct
{
	int label_flag;
	int eror_flag;

}Psw;

typedef enum
{
	zero_operands,
	one_operands,
	two_operands

}cWd_type;



typedef enum
{
	jmp = 9,
	bne = 10,
	jsr = 13,
	lea = 6,
	cmp = 1,
	prn = 12


}Special_cmd;

typedef enum
{
	notvalid = -1,
	imidiate,
	direct,
	jmp_parmeter,
	register_d,
	islabel

}AdrMethod;








/* @ Function: merger
   @ Arguments: This function gets short* dataWord, short* insWord, int IC, int DC
   @ Description: The function accepts two arrays
	and merges them into a single array so that the array
	of commands will be in the smaller values of the array and 
	right after it the values of the data will enter
  */
void merger(short* dataWord, short* insWord, int IC, int DC);

void renameFile(char* temp,char* oldName,const char* type);
cWd_type getCmdtype(int num_cmd);
short check_reg(char* test_reg);
int c_calc_L(int source, int target, int L);
int check_command(char* test_cmd);
AdrMethod operands_Analysis(char* oper_type, List* lst,int PC);
Bool legal_label(char* label,int PC);
int calc_L(char* cmdline, List* lst, Special_cmd checkcmd);
Bool check_if_start_of_label(char* cmdline);
void removeSpace(char** c);
Bool hendle_zeroOperands_cmd(List* lst, short* incWord, int IC, int PC);




/* @ Function: free_label
   @ Arguments: This function gets void* label
   @ Description: The function frees the label from memory
   */
Bool comma_syntax(char* line, int PC);


/* @ Function: free_label
   @ Arguments: This function gets void* label
   @ Description: The function frees the label from memory
   */
Bool empty_line(char* cmdline);

/* @ Function: free_label
   @ Arguments: This function gets void* label
   @ Description: The function frees the label from memory
   */
Bool isPrompt_line(char* cmdline);



Bool jmp_syntax(char* line, int PC);
Bool isValidNum(char* num, int PC);

#endif
