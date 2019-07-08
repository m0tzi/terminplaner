#include "ReadFile.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char *TokenGen(int size)
{
	char AllChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
	srand(time(NULL));
	char *ret = new char[size + 1];
	for (int i = 0; i < size; i++)
	{
		ret[i] = AllChars[rand() % (sizeof(AllChars) - 1)];
	}
	ret[size] = '\0';

	return ret;
}

char *getUserTxt(char token[201])
{
	char *txtName = new char[205 + 32];
	strcpy(txtName, "C:/temp/apache/cgi-bin/Userdaten/");
	strcat(txtName, token);
	strcat(txtName, ".txt");
	return txtName;
}
char *getDateTxt(char Datename[51])
{
	char *txtName = new char[205 + 32];
	strcpy(txtName, "C:/temp/apache/cgi-bin/Termine/");
	strcat(txtName, Datename);
	strcat(txtName, ".txt");
	return txtName;
}