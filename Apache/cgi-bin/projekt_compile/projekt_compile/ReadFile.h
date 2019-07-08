#ifndef ReadFile_H
#define ReadFile_H


#include "structs.h"

Userdaten *getUserDataFromFile(FILE *f, char Username[41], char Password[41]);
int RowsInFile(FILE *f, int ResetToPosition, int SizeofData);
int getUser(char Username[41], char Password[41], char* token);
int getDate(char Datename[51], char *out);
int getDates(char token[201], char** outPut);
#endif