#ifndef WriteFile_H
#define WriteFile_H

#include "structs.h"

int makeUser(char Username[41], char Password[41], char *token);
int makeDate(char token[201], Date *termin);
int deleteDate(char ID[201]);
int ModifyDate(char token[201], Date *termin);

int ModifyPassword(char token[201], char NewPass, char OldPass);
#endif