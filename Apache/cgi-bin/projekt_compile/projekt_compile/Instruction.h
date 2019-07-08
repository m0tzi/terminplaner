#ifndef Instruction_H
#define Instruction_H

#include "structs.h"

int ValuePosition(char c[1000], char SearchforString[100]);
void GetValue(char *Value, int pos, char c[1000], int Maxlength);
void outPutStart(int StatusCode, char *Desc);
void cookieSetter(int StatusCode, char * Cookie);
#endif