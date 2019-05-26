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

struct Userdaten getUserDataFromFile(FILE *f, char Username[41], char Password[41])
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
            return *User;
        }
    }
    User->Username[0] = '\0';
    User->Password[0] = '\0';
    User->token[0] = '\0';
    return *User;
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

int getUser(char Username[41], char Password[41])
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
        struct Userdaten User = getUserDataFromFile(f, Username, Password);
        fclose(f);
        // cout << User.Username[0] << endl;
        if (User.Username[0] != '\0')
        {
            if (strcmp(User.Password, Password) == 0)
            {
                // cout << "[SERVER]: Password stimmt überein"<<endl;
                // cout << "[SERVER]: Token ist: " << User.token << endl<< endl;
                return 2;
            }
            else
            {
                // cout << "[SERVER]: Password nicht correct, ------- hier ist noch WIP ------- "<<endl;
                return 1;
            }
        }
        else
        {
            // cout << "[SERVER]: User nicht vorhanden" << endl;
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

int makeUser(char Username[41], char Password[41])
{
    FILE *f;
    int User = getUser(Username, Password);
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
            fwrite("", 0, 0, f);

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

    char *test = getUserTxt(token);
    FILE *f = fopen(test, "a");
    if (f == NULL)
    {
        // cout << "Failed, didn't find the User" << endl;
        return -1;
    }
    fwrite(termin->Datename, sizeof(termin->Datename), 1, f);
    fclose(f);
    // cout << "Dateiverweiß beim User hinzugefügt" << endl;
    f = fopen(getDateTxt(termin->Datename), "w+");
    if (f == NULL)
    {
        // cout << "Failed, coun't create the Date-File" <<endl;
        return -2;
    }

    fwrite(termin, sizeof(Date), 1, f);
    fclose(f);
    // cout << "Termin erstellt, mit den Daten : " << termin->DateDescription << endl;
    return 1;
}
int getDate(char Datename[51])
{
    FILE *f = fopen(getDateTxt(Datename), "r");
    if (f == NULL)
    {
        return -1;
    }
    Date *termin = new Date;
    fread(termin, sizeof(Date), 1, f);
    // cout << "termin: " << termin->Datename << endl;
    // cout << "Beschreibung: " << termin->DateDescription << endl;
    // cout << "Zeit: " << termin->Time << endl;
}
int getDates(char token[201])
{
    FILE *f = fopen(getUserTxt(token), "r");
    if (f == NULL)
    {
        return -1;
    }
    // fseek(f, sizeof(Userdaten), SEEK_SET);
    // Fehler, in den UserTxt Dateien werden ja nur die wirklichen Termine gespeichert
    char *Datename = new char[51];
    while (!feof(f))
    {
        fread(Datename, sizeof(char), 51, f);
        getDate(Datename);
    }
}

void outPutStart(int StatusCode, char Desc[201])
{
    // cout << "Content-Type: application/json\r\n";
    // cout << "Status:" << StatusCode << "\r\n\r\n";
    cout << "{ \"Desc\" : \"" << Desc << "\" }";
}

int main()
{
    cout << "Content-Type: plain/text\r\n\r\n"; //DELETE
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

            switch (makeUser(Username, Password))
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
                    char Error[] = "User created";
                    outPutStart(201, Error);
                    break;
                }
            }
        }
        else if (strcmp(Befehl, "GetToken") == 0)
        {
            switch (getUser(Username, Password)){
                case -1:{
                    char Error[] = "Could not use the User.txt";
                    outPutStart(599,Error);
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
                    FILE * f = fopen("c:/temp/apache/cgi-bin/User.txt", "r+");
                    if(f==NULL){
                        return 0;
                    }
                    Userdaten User = getUserDataFromFile(f, Username, Password); //HIER WIP. Ich muss die ausgabe dynamischer machen
                    outPutStart(200, User.token);
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
                if (Pos >= 0)
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
                            // cout << "Termin überschrieben" << endl;
                            strcpy(Termin->DateDescription, Desc);
                            strcpy(Termin->Time, Time); // ICH KANN VON ANFANGAN DEN VERDAMMTEN STRUCT BENUTZEN, egal, später.
                            makeDate(Token, Termin);
                        }
                        else
                        {
                            // cout << "Dateiname nicht nutzbar" << endl;
                        }
                    }
                    else
                    {
                        // cout << "Keine Zeitangaben gemacht" << endl;
                    }
                }
                else
                {
                    // cout << "Keine Beschreibung gegeben" << endl;
                }
            }
            else
            {
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
                getDates(Token->token);
            }
        }
        else
        {
            // cout << "[SERVER]: Konnte leider mit dem Befehl nichts anfangen, bisher gibt es nur 'CreateUser' und 'GetToken'" << endl;
        }
    }
    else
    {
        // cout << "[SERVER]: Habe leider keinen richtigen Befehl erhalten";
    }

    // cout << endl << "[SERVER]: Done" << endl << endl;
    return 0;
}