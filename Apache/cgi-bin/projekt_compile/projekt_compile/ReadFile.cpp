#include "ReadFile.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "TxtHandler.h"
using namespace std;
struct Userdaten *getUserDataFromFile(FILE *f, char Username[41], char Password[41])
{
	int size = sizeof(Userdaten);
	Userdaten *User = new Userdaten;
	// cout << "in getUserDataFromFile: " << Username;
	// cout << "[SERVER]: ------------- FILE CONTAINS ----------------" << endl;
	while (!feof(f))
	{
		fread(User, size, 1, f);
		// cout << "[SERVER]: User : " <<User->Username << endl;// << " || Password: " << User->Password<< " || Token: " << User->token << endl;
		if (strcmp(User->Username, Username) == 0)
		{
			// cout << "[SERVER]: User " << Username <<" Existiert"<<endl;
			return User;
		}
	}
	return NULL;
}

int RowsInFile(FILE *f, int ResetToPosition, int SizeofData)
{
	int MaxRow = 0;
	char *c = new char[SizeofData];
	while (!feof(f))
	{
		fgets(c, SizeofData, f);
		MaxRow++;
	}
	fseek(f, SizeofData, ResetToPosition);
	return MaxRow;
}

int getUser(char Username[41], char Password[41], char* token)
{
	FILE *f;
	int returnValue;
	f = fopen("c:/temp/apache/cgi-bin/User.txt", "r+");

	if (f == NULL)
	{
		f = fopen("c:/temp/apache/cgi-bin/User.txt", "w+");
		if (f == NULL)
		{
			return -1;
		}
		fclose(f);
	}
	else
	{
		// cout << "Username in getUser ist:" << Username << endl;
		Userdaten* User = getUserDataFromFile(f, Username, Password);
		fclose(f);
		// cout << User.Username[0] << endl;
		if (User != NULL){
			if (strcmp(Password, User->Password) == 0){
				strcpy(token, User->token);
				token[203] = '\0';
				return 2;
			}
			else
			{
				return 1;
			}
		}
		else {
			return 0;
		}
	}
}

int getDate(char Datename[51], char *out)
{
	FILE *f = fopen(getDateTxt(Datename), "r");
	//cout << "Content-Type: plain/text\r\n";
	//cout << "Status:" << 200 << "\r\n\r\n";
	//cout << "getDate" << endl << endl;
	if (f == NULL)
	{
		//cout << "getSingleDateFail" << endl << "File: " << getDateTxt(Datename) << endl;
		return -1;
	}
	Date *termin = new Date;
	fread(termin, sizeof(Date), 1, f);
	strcat(out, "{\"termin\":\"");
	strcat(out, termin->Datename);
	strcat(out, "\",\"Beschreibung\":\"");
	strcat(out, termin->DateDescription);
	strcat(out, "\",\"Time\":\"");
	strcat(out, termin->Time);
	strcat(out, "\"}");
}

int getDates(char token[201], char** outPut)
{
	FILE *f = fopen(getUserTxt(token), "r");
	if (f == NULL)
	{
		return -1;
	}
	// fseek(f, sizeof(Userdaten), SEEK_SET);
	// Fehler, in den UserTxt Dateien werden ja nur die wirklichen Termine gespeichert
	char *Datename = new char[51];
	int AllDates = RowsInFile(f, 0, sizeof(Date));
	char* out = new char[AllDates*sizeof(Date) + (AllDates * 36) + 1]; // chars + [] + komma - 1 komma + die anf�hrungszeichen + {}
	strcpy(out, "[");
	fseek(f, 0, SEEK_SET);
	fread(Datename, sizeof(char), 51, f);
	while (!feof(f))
	{
		//cout << "Dateiname gefunden: " << Datename << endl;
		getDate(Datename, out);
		fread(Datename, sizeof(char), 51, f);
		if (!feof(f)){
			strcat(out, ",");
		}

	}
	strcat(out, "]");
	*outPut = out;
	//cout << "FirstPointer: " << endl << out << endl;
	return 0;
}