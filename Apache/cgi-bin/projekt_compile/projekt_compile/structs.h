#ifndef Structs_H
#define Structs_H
#pragma warning(disable : 4996)

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct Userdaten
{
	char Username[41];
	char Password[41];
	char token[201];
};
struct Date
{
	char Datename[51];
	char DateDescription[401];
	char Time[51];
};

#endif