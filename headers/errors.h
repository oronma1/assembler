#ifndef _ERRORS_
#define _ERRORS_




typedef enum
{
	emptyAfterLabel,
	firstLabelCharisNotAlpha,
	labelNameLikecmd,
	labelNameLikereg,
	labelexist,
	misingGereshS,
	misingGereshF,
	misingLabelExtern,
	cmdNotExist,
	spaceAftersol,
	NotDigit,
	NolabelAfterCmd,
	afterCmdNotEmpty,
	numOperandsInvalid,
	misingLeftBracket,
	spaceBetweenOperands,
	misingRightBracket,
	extraLeftBracket,
	extraRightBracket,
	numOperandsinvalid,
	targetcantBeImmediate,
	commaBefore,
	extraComma,
	MisingComma,
	sorceCantBeImiOrReg,
	targetCantBeImmediate,
	ErrorfirstRun,
	spaceBetweenLabelColon,
	labelSize,
	labelNameNotAlpha,
	comaAfter,
	emptyString,
	emptyData,
	spaceAfterSol,
	operandUndefine,
	secondRunError,
	labelUndefine
	
	


}err;

typedef enum
{

	cantOpenToRead,
	cantOpenToWrite,
	cantRemoveFile,
	NoArguments

}errFile;

typedef enum
{

	memoryAllocate


}memory_fail;

void ErrorFile(errFile errnum, char* fileName);
void printError(err errnum, int PC);
void ErrorMemory(memory_fail errnum);



#endif
