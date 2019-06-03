#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
// #include "C:\temp\Apache\cgi-bin\header.h"

using namespace std;

#pragma warning(disable : 4996)

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

struct Userdaten *getUserDataFromFile(FILE *f, char Username[41], char Password[41])
{
	int size = sizeof(Userdaten);
	Userdaten *User = new Userdaten;
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
		Userdaten* User = getUserDataFromFile(f, Username, Password);
		fclose(f);
		// cout << User.Username[0] << endl;
		if (User != NULL){
			if (strcmp(Password, User->Username)){
				strcpy(token, User->token);
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
			// HIER WIP, im Grunde kann ich auch beim getUser den struct zur¸ckgeben und den return int als Pointer ¸bergeben, ist aber bisher nicht so.
			Userdaten *User = new Userdaten;
			strcpy(User->Username, Username);
			strcpy(User->Password, Password);
			strcpy(User->token, TokenGen(200));
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

	char *txt = getUserTxt(token);
	FILE *f = fopen(txt, "a");
	if (f == NULL)
	{
		// cout << "Failed, didn't find the User" << endl;
		return -1;
	}
	fwrite(termin->Datename, sizeof(termin->Datename), 1, f);
	fclose(f);
	// cout << "Dateiverweiﬂ beim User hinzugef¸gt" << endl;
	f = fopen(getDateTxt(termin->Datename), "w+");
	if (f == NULL)
	{
		// cout << "Failed, coun't create the Date-File" <<endl;
		return -2;
	}

	fwrite(termin, sizeof(Date), 1, f);
	fclose(f);
	 cout << "Termin erstellt, mit den Daten : " << termin->DateDescription << endl;
	return 0;
}
int getDate(char Datename[51], char *out)
{
	FILE *f = fopen(getDateTxt(Datename), "r");
	if (f == NULL)
	{
		cout << "getSingleDateFail" << endl << "File: " << getDateTxt(Datename)<< endl;
		return -1;
	}
	Date *termin = new Date;
	fread(termin, sizeof(Date), 1, f);
	strcat(out, "{\"termin\":\"");
	strcat(out, termin->Datename);
	strcat(out, "\",Beschreibung\":\"");
	strcat(out, termin->DateDescription);
	strcat(out, "\",Time\":\"");
	strcat(out, termin->Time);
	strcat(out, "\"}");
	cout << "getSingleDate: " << endl << *out << endl;
	//strcat()
	// cout << "termin: " << termin->Datename << endl;
	// cout << "Beschreibung: " << termin->DateDescription << endl;
	// cout << "Zeit: " << termin->Time << endl;
}
int getDates(char token[201], char** outPut)
{
	FILE *f = fopen(getUserTxt(token), "rb");
	if (f == NULL)
	{
		return -1;
	}
	// fseek(f, sizeof(Userdaten), SEEK_SET);
	// Fehler, in den UserTxt Dateien werden ja nur die wirklichen Termine gespeichert
	char *Datename = new char[51];
	int AllDates = RowsInFile(f, 0, sizeof(Date));
	char* out = new char[AllDates*sizeof(Date) + (AllDates*36) + 1]; // chars + [] + komma - 1 komma + die anf¸hrungszeichen + {}
	strcat(out, "[");
	fseek(f, 0, SEEK_SET);
	while (!feof(f))
	{
		fread(Datename, 1, 51, f);
		cout << "Dateiname gefunden: " << Datename << endl;
		getDate(Datename, out);
		if (!feof(f)){
			strcat(out, ",");
		}
	}
	strcat(out, "]");
	*outPut = out;
	cout << "FirstPointer: " << endl << out << endl;
	return 0;
}

void outPutStart(int StatusCode, char *Desc) // int length | ist noch Tempor‰r, vielleicht gar nicht nˆtig, da es keine Nullerkennung innerhalb des Strings geben sollte.
{
	cout << "Content-Type: application/json\r\n";
	cout << "Access-Control-Allow-Origin: http://192.168.0.192\r\n"; // WICHTIG, wenn man nicht ¸ber localhost geht.
	cout << "Status:" << StatusCode << "\r\n\r\n";
	cout << "{ \"Server\" : \"" << Desc << "\" }";

	// for (int i = 0; i < lenght; i++){
	//     cout << Desc[i];
	// }



	// cout << "\" }";
}

int main()
{
	cout << "Content-Type: plain/text\r\n\r\n"; //DELETE - hier nur wenn ich irgendwo cout einf¸ge zum Testen
	char c[1000];
	cin >> c;
	// cout << "[SERVER]: Erhaltene Daten: " << c << endl;

	//Funktionen
	char SearchforValue[100];
	strcpy(SearchforValue, "Username=");
	int Pos = ValuePosition(c, SearchforValue);
	char *Username = new char[41];
	char *Password = new char[41];
	char *Befehl = new char[41];
	if (Pos >= 0)
	{
		GetValue(Username, Pos, c, 41);
	}

	strcpy(SearchforValue, "Password=");
	Pos = ValuePosition(c, SearchforValue);
	if (Pos >= 0)
	{
		GetValue(Password, Pos, c, 41);
	}

	/**** Wichtig ****
	*
	* Es gibt noch keine Errorabfragen, steht alles auf der ToDo Liste.
	* Mich aber dennoch gerne dran erinnnern.
	*
	*
	*/
	strcpy(SearchforValue, "Befehl=");
	Pos = ValuePosition(c, SearchforValue);
	if (Pos >= 0)
	{
		GetValue(Befehl, Pos, c, 41);
		if (strcmp(Befehl, "CreateUser") == 0)
		{
			char *token = new char[201];
			switch (makeUser(Username, Password, token))
			{
			case -1:
			{
				char Error[] = "Could not access user file";
				outPutStart(599, Error);
				break;
			}

			case -2:
			{
				char Error[] = "User exists already";
				outPutStart(599, Error);
				break;
			}
			case 0:
			{
				outPutStart(201, token);
				break;
			}
			}
		}
		else if (strcmp(Befehl, "GetToken") == 0)
		{
			char *token = new char[200];
			switch (getUser(Username, Password, token)){
			case -1:{
				char Error[] = "Could not use the User.txt";
				outPutStart(599, Error);
				break;
			}
			case 0:{
				char Error[] = "User does not exist";
				outPutStart(499, Error);
				break;
			}
			case 1:{
				char Error[] = "Password is incorrect";
				outPutStart(498, Error);
				break;
			}
			case 2:{
				outPutStart(200, token);
			}
			}
		}
		else if (strcmp(Befehl, "CreateDate") == 0)
		{
			char *Token = new char[201];
			strcpy(SearchforValue, "Token=");
			Pos = ValuePosition(c, SearchforValue);
			if (Pos >= 0)
			{
				GetValue(Token, Pos, c, 201);
				strcpy(SearchforValue, "Desc=");
				char *Desc = new char[401];
				Pos = ValuePosition(c, SearchforValue);
				if (Pos >= 0) // WICHTIG : Hier muss noch die abfrage rein, ob es den User wirklich gibt. wof¸r es noch keine Funktion gibt.
				{
					GetValue(Desc, Pos, c, 401);
					strcpy(SearchforValue, "Time=");
					char *Time = new char[51];
					Pos = ValuePosition(c, SearchforValue);
					if (Pos >= 0)
					{
						GetValue(Time, Pos, c, 401);
						strcpy(SearchforValue, "DateName=");
						char *Datename = new char[51];
						Pos = ValuePosition(c, SearchforValue);
						if (Pos >= 0)
						{

							GetValue(Datename, Pos, c, 51);
							Date *Termin = new Date;
							strcpy(Termin->Datename, Datename);
							// cout << "Termin ¸berschrieben" << endl;
							strcpy(Termin->DateDescription, Desc);
							strcpy(Termin->Time, Time); // ICH KANN VON ANFANGAN DEN VERDAMMTEN STRUCT BENUTZEN, egal, sp‰ter.
							switch (makeDate(Token, Termin)){
							case 0:{
								char *Output = new char[22 + sizeof(Termin->Datename)];
								strcpy(Output, "Termin ");
								strcat(Output, Datename);
								strcat(Output, " wurde erstellt");
								outPutStart(201, Output);
								break;
							}
							case -1:
							case -2:{
								char Output[] = "File-Error";
								outPutStart(599, Output);
							}
							}
						}
						else
						{
							char Output[] = "Dateiname nicht ok";
							outPutStart(499, Output);
						}
					}
					else
					{
						char Output[] = "Zeitangabe nicht ok";
						outPutStart(498, Output);
						// cout << "Keine Zeitangaben gemacht" << endl;
					}
				}
				else
				{
					char Output[] = "Beschreibung kam im TS nicht an";
					outPutStart(497, Output);
					// cout << "Keine Beschreibung gegeben" << endl;
				}
			}
			else
			{
				char Output[] = "Token nicht korrekt";
				outPutStart(599, Output);
				// cout << "Token nicht gegeben" << endl;
			}
		}
		else if (strcmp(Befehl, "GetDates") == 0)
		{
			strcpy(SearchforValue, "Token=");
			Pos = ValuePosition(c, SearchforValue);
			if (Pos >= 0)
			{
				Userdaten *Token = new Userdaten;
				GetValue(Token->token, Pos, c, sizeof(Token->token));
				char *outPut = NULL;
				getDates(Token->token, &outPut);
				outPutStart(200, outPut);
			}
		}
		else
		{
			char Output[] = "Befehl nicht bekannt. Guck im Wiki nach, dort stehen alle Befehle.";
			outPutStart(599, Output);
		}
	}
	else
	{
		char Output[] = "Befehl-Parameter nicht gegeben. Es muss wirklich \"Befehl\" heiﬂen.";
		outPutStart(599, Output);
	}
	return 0;
}