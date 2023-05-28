#ifndef _WRITE_FILES_
#define _WRITE_FILES_

#include "list.h"






#define POINT "."


void write_files(List* lst, short* insWord, int IC, int DC, char* fileName);
void write_ob_file(short* insWord, int IC, int DC, char* fileName);
void write_entry_label(List* lst, char* fileName);





#endif
