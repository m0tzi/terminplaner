#include "Instruction.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
using namespace std;

int ValuePosition(char c[1000], char SearchforString[100])
{
	// char * values = malloc(1,sizeof(char));
	int UsernameCounter = 0;
	for (int ci = 0; c[ci] != '\0'; ci++)
	{
		if (c[ci] == SearchforString[UsernameCounter])
		{
			UsernameCounter++;
		}
		else
		{
			UsernameCounter = 0;
		}
		if (SearchforString[UsernameCounter] == '\0')
		{
			if (c[ci + 1] != '\0' && c[ci + 1] != '\0')
			{
				ci = ci + 1;
				return ci;
			}
			else
			{
				// cout << "[SERVER]: Found Value, but empty"; // DELETE
			}
		}
	}
	return -1;
}

void GetValue(char *Value, int pos, char c[1000], int Maxlength)
{
	int Valueindex = 0;
	for (int i = pos; c[i] != '\0' && c[i] != '&'; i++)
	{
		if (c[i] == '+')
			Value[Valueindex] = ' ';
		else
			Value[Valueindex] = c[i];
		Valueindex++;
	}
	Value[Valueindex] = '\0';
}

void outPutStart(int StatusCode, char *Desc) // int length | ist noch Tempor�r, vielleicht gar nicht n�tig, da es keine Nullerkennung innerhalb des Strings geben sollte.
{
	// cout << "Access-Control-Allow-Origin: http://192.168.0.192\r\n"; // WICHTIG, wenn man nicht �ber localhost geht.
	cout << "Content-Type: application/json\r\n";
	cout << "Status:" << StatusCode << "\r\n\r\n";
	if (Desc[0] != '[')
		cout << "{ \"Server\" : \"" << Desc << "\" }";
	else
		cout << "{ \"Server\" : " << Desc << " }";
}

void cookieSetter(int StatusCode, char * Cookie){
	cout << "Content-Type: application/json\r\n";
	cout << "Status:" << StatusCode << "\r\n";
	cout << "Set-Cookie:CES=" << Cookie << "\r\n\r\n";
	cout << "{ \"Server\" : \"" << "CES " << Cookie << "\" }";
}