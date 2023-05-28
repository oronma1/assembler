#ifndef _SECOND_RUN_
#define _SECOND_RUN_

#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


BoolProcess second_transaction(List* lst, FILE* pf, short* incWord, char* fileName, int DC);
Bool Update_label_address_Addressing2(List* lst, char* operands, short* incWord, int IC, int PC, FILE* fpext);
short set_label_bit(List* lst, char* labelName,FILE* fpext, int IC,int PC);


Bool Update_label_address_oneOperands(List* lst, char* operands, short* incWord, int IC, int PC, FILE* fpext);
Bool Update_label_address_twoOperands(List* lst, char* operands, short* incWord, int IC, int PC, FILE* fpext);



#endif
















































































