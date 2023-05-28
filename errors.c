
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void ErrorMemory(memory_fail errnum)
{
    
    printf("\n############################################################################################\n");

    switch (errnum)
    {

    case memoryAllocate:
    {
        fprintf(stderr,"Error!! Memory allocation error ");
        exit(1);
        break;
    }
        

    }

    printf("\n############################################################################################\n");


}


void ErrorFile(errFile errnum,char* fileName)
{
    printf("\n############################################################################################\n");

    switch (errnum)
    {

        case cantOpenToRead:
        {
            fprintf(stderr, "Error!! can't open file \"%s\" to read",fileName);
            break;
        }

        case cantOpenToWrite:
        {
            fprintf(stderr,"Error!! cant open file %s to write", fileName);
printf("\n############################################################################################\n");
            exit(1);
            break;
        }

        case cantRemoveFile:
        {
            printf("Error!! cant remove file %s ", fileName);
            break;
        }
	

	case NoArguments:
        {
            fprintf(stderr,"Error!! There are no arguments in the command line ");
            break;
        }


    }

    printf("\n############################################################################################\n");

}



void printError(err errnum, int PC)
{

    printf( "\n***************************************************************************************\n");

    switch (errnum)
    {
    case emptyAfterLabel:
        printf("Error!! in line:%d  After setting the label is empty ", PC);
        break;

    case firstLabelCharisNotAlpha:
        printf("Error!! in line:%d  The first character of the label is not alphabetic letter", PC);
        break;

    case labelNameLikecmd:
        printf("Error!! in line:%d  The label Cannot be one of the reserved commands", PC);
        break;

    case labelNameLikereg:
        printf("Error!! in line:%d  The label Cannot be one of the reserved register", PC);
        break;

    case labelexist:
        printf("Error!! in line:%d  label already exist", PC);
        break;

    case misingGereshS:
        printf("Error!! in line:%d  missing \" before the string definition", PC);
        break;

    case misingGereshF:
        printf("Error!! in line:%d  missing \" at the end of the string definition", PC);
        break;

    case misingLabelExtern:
        printf("Error!! in line:%d  Missing label after setting extern", PC);
        break;

    case cmdNotExist:
        printf("Error!! in line:%d  The command does not exist in the command system", PC);
        break;

    case spaceAftersol:
        printf("Error!! in line:%d  space after # not legal", PC);
        break;

    case NotDigit:
        printf("Error!! in line:%d  is not a digit", PC);
        break;

    case NolabelAfterCmd:
        printf("Error!! in line:%d  No label operand was found after the command", PC);
        break;

    case afterCmdNotEmpty:
        printf("Error!! in line:%d After the command line there is something that is not an empty line or a prompt line", PC);
        break;

    case numOperandsInvalid:
        printf("Error!! in line:%d The number of operands is invalid for the command", PC);
        break;

    case misingLeftBracket:
        printf("Error!! in line:%d missing '(' before operands", PC);
        break;

    case spaceBetweenOperands:
        printf("Error!! in line:%d space between label and operands is not legal", PC);
        break;

    case misingRightBracket:
        printf("Error!! in line:%d missing ')' end operands", PC);
        break;

    case extraLeftBracket:
        printf("Error!! in line:%d  Extra '(' between operands", PC);
        break;

    case extraRightBracket:
        printf("Error!! in line:%d  Extra ')' between operands", PC);
        break;

    case numOperandsinvalid:
        printf("Error!! in line:%d  The number of operands for the command is invalid", PC);
        break; 

    case targetcantBeImmediate:
        printf("Error!! in line:%d The target operator cannot be immediate in this command type", PC);
        break;

    case commaBefore:
        printf("Error!! in line:%d Comma before operands is invalid", PC);
        break;

    case extraComma:
        printf("Error!! in line:%d Extra commas between the operands", PC);
        break;

    case MisingComma:
        printf("Error!! in line:%d Mising commas between the operands", PC);
        break;

    case sorceCantBeImiOrReg:
        printf("Error!! in line:%d The source operator cannot be immediate or register in this command", PC);
        break;
        
    case targetCantBeImmediate:
        printf("Error!! in line:%d The target operator cannot be immediate in this command type", PC);
        break;


    case ErrorfirstRun:
        printf("WARNING ##################################\nWere Errors in the first passage\n##################################\n");
        break;


    case spaceBetweenLabelColon:
        printf("Error!! in line:%d In the label definition there is a space between ':' and the name of the label", PC);
        break;

    case labelSize:
        printf("Error!! in line:%d The size of the label name is larger than allowed", PC);
        break;

    case labelNameNotAlpha:
        printf("Error!! in line:%d The label name is not an alphabetic letter or a number", PC);
        break;

    case comaAfter:
        printf("Error!! in line:%d Comma after operands is invalid ", PC);
        break;

    case emptyString:
        printf("WARNING in line:%d After the definition .string is empty ", PC);
        break;

    case emptyData:
        printf("WARNING in line:%d After the definition .data is empty ", PC);
        break;
        
    case spaceAfterSol:
        printf("Error in line:%d In the definition immediate after the '#' there is a space ", PC);
        break;

    case operandUndefine:
        printf("Error in line:%d The operand is undefine ", PC);
        break;

    case secondRunError:
        printf("WARNING ##################################\nWere Errors in the second passage\n##################################\n");
        break;

    case labelUndefine:
        printf("Error in line:%d The label is undefine ", PC);
        break;

    
    }
   
    
    printf("\n***************************************************************************************\n");


}

