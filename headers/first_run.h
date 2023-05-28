#ifndef _FIRST_RUN_
#define _FIRST_RUN_



#include "func.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/* @ Function: first_transaction
   @ Description: This function is the first pass on the assembly program
    The main function is to discover all the existing labels within 
    the program and update them in the list of labels
    In addition, the function updates the opcode bit field of the commands
    and the operands of the commands except an operand which is a label
    and warns of possible errors if any
    If the first pass was successful, we will proceed 
    to the second pass if we do not leave the program.
   */
BoolProcess first_transaction(List* lst, FILE* pf, short* word, short* incWord, char* fileName);
short set_opcode(int opcode, short word);
short set_source(AdrMethod source, short word);
short set_target(AdrMethod target, short word);
short set_parameter1(AdrMethod source, short word);
short set_parameter2(AdrMethod target, short word);
short set_imidiate_bit(char* imid);
short set_reg_source_bit(char* source);
short set_reg_target_bit(char* target);
short set_two_register_bit(char* source, char* target);
Bool hendle_Addressing_command2(List* lst, char* operands, short* incWord, int cmdNum, int IC, int PC);
void update_twoOperands_bit(AdrMethod source, AdrMethod target, char* strTarget, char* strSource, short* insWord, int IC);

Bool hendle_oneOperands_cmd(List* lst, char* operands, short* incWord, int cmdNum, int IC, int PC);
Bool hendle_twoOperands_cmd(List* lst, char* operands, short* incWord, int cmdNum, int IC, int PC);








#endif
