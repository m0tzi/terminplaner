#include "WriteFile.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "ReadFile.h"
#include "TxtHandler.h"

using namespace std;
int makeUser(char Username[41], char Password[41], char *token)
{
	FILE *f;
	int User = getUser(Username, Password, token);
	if (User == 0)
	{
		f = fopen("c:/temp/apache/cgi-bin/User.txt", "a");
		if (f == NULL)
		{
			// cout << "[SERVER]: FILE Error, at" << endl << "-------  makeUser -------" << endl;
			return -1;
		}
		else
		{
			// HIER WIP, im Grunde kann ich auch beim getUser den struct zur�ckgeben und den return int als Pointer �bergeben, ist aber bisher nicht so.
			Userdaten *User = new Userdaten;
			strcpy(User->Username, Username);
			strcpy(User->Password, Password);
			char *Token = new char[201];
			FILE * exists;
			do{
				Token = TokenGen(200);
				exists = fopen(getUserTxt(Token), "r");
			} while (exists != NULL);

			strcpy(User->token, Token);
			fwrite(User, sizeof(Userdaten), 1, f);
			fclose(f);

			f = fopen(getUserTxt(User->token), "w+");
			if (f == NULL)
			{
				// cout << "[SERVER]: File Error, at" << endl << "------ makeUser ------" <<endl << User->Username<< endl;
				return -2;
			}
			strcpy(token, User->token);

			fclose(f);
			// cout << "[Server]: User " << Username <<" created" << endl;
		}
		fclose(f);
	}
	else if (User == -1)
	{
		return -1; //User.txt nicht benutzbar
	}
	else
	{
		return -2; //User schon vorhanden
	}
	return 0;
}
int makeDate(char token[201], Date *termin)
{
	//cout << "Content-Type: plain/text\r\n";
	//cout << "Status:" << 200 << "\r\n\r\n";
	//cout << "going in";
	FILE *f = fopen(getDateTxt(token), "r");
	if (f == NULL){
		char *txt = getUserTxt(token);
		f = fopen(txt, "a");
		if (f == NULL)
		{
			//cout << "Failed, didn't find the User" << endl;
			return -1;
		}
		char *Token = new char[201];
		FILE * exists;
		do {
			Token = TokenGen(200);
			exists = fopen(getDateTxt(Token), "r");
		} while (exists != NULL);

		fwrite(Token, 201, 1, f);
		fclose(f);
		 //cout << "Dateiverwei� beim User hinzugef�gt" << endl;
		f = fopen(getDateTxt(Token), "w+");
		if (f == NULL)
		{
			 //cout << "Failed, coun't create the Date-File" <<endl;
			return -2;
		}

		fwrite(termin, sizeof(Date), 1, f);
		fclose(f);
		//cout << "Termin erstellt, mit den Daten : " << termin->DateDescription << endl;
		return 0;
	}
	//Datei wurde schon erstellt. Nicht mehr in benutzung, da wir jetzt Tokens auch für Termine benutzen
	else{
		//cout << "Termin bereits erstellt" << endl;
		return -3;
	}
}
int deleteDate(char ID[201]) {
	return remove(getDateTxt(ID));
	
}
int ModifyDate(char token[201], Date *termin) {
	//cout << "Content-Type: plain/text\r\n";
	//cout << "Status:" << 200 << "\r\n\r\n";
	//cout << "going in";
	FILE * f = new FILE;
	f = fopen(getDateTxt(token), "w+");
	if (f == NULL)
	{
		//cout << "Failed, coun't create the Date-File" <<endl;
		return -2;
	}
	fwrite(termin, sizeof(Date), 1, f);
	fclose(f);
	//cout << "Termin erstellt, mit den Daten : " << termin->DateDescription << endl;
	return 0;
}