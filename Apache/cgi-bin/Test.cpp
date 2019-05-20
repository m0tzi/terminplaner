#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

#pragma warning(disable : 4996)

struct Userdaten
{
    char Username[41];
    char Password[41];
    char token[201];
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
                cout << "[SERVER]: Found Value, but empty"; // DELETE
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
        // cout << "[SERVER]: User : " <<User->Username << " || Password: " << User->Password<< " || Token: " << User->token << endl;
        if(strcmp(User->Username, Username) == 0){
            cout << "[SERVER]: User " << Username <<" Existiert"<<endl;
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
    f = fopen("c:/temp/apache/cgi-bin/User.txt", "r");
    if (f == NULL)
    {
        cout << "[SERVER]: Konnte die User.txt nicht öffnen." << endl;
        fclose(f);
        returnValue = -1;
    }
    else
    {
        struct Userdaten User = getUserDataFromFile(f, Username, Password);
        fclose(f);
        if (User.Username[0]!='\0'){
            if(strcmp(User.Password, Password) == 0){
                cout << "[SERVER]: Password stimmt überein"<<endl;
                cout << "[SERVER]: Token ist: " << User.token << endl<< endl;
                returnValue = 2;
            } else {
                cout << "[SERVER]: Password nicht correct, ------- hier ist noch WIP ------- "<<endl;
                returnValue = 1;
            }

        }
        else {
            cout << "[SERVER]: User nicht vorhanden" << endl;
            return 0;
        }
    }
}

char * TokenGen( int size ){
    char AllChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
    char * ret = new char[size+1];
    for (int i = 0; i < size; i++){
        ret[i] = AllChars[rand() % (sizeof(AllChars) - 1)];
    }
    ret[size] = '\0';

    return ret;
}

int makeUser(char Username[41], char Password[41]){
    FILE *f;
    if(getUser(Username, Password)==0){
        f = fopen("c:/temp/apache/cgi-bin/User.txt", "a+");
        if(f==NULL){
            cout << "[SERVER]: FILE Error, at" << endl << "-------  makeUser -------" << endl;
            return 0;
        }
        else{
            Userdaten * User = new Userdaten;
            strcpy(User->Username,Username);
            strcpy(User->Password,Password);
            strcpy(User->token, TokenGen(200));
            fwrite(User,sizeof(Userdaten),1,f);
            cout << "[Server]: User " << Username <<" created" << endl;
        }
    }
    return 0;
}

int main()
{
    char c[1000];
    cin >> c;
    cout << "Content-Type: text/html\r\n\r\n";
    cout << "[SERVER]: Erhaltene Daten: " << c << endl;

    //Funktionen
    char SearchforValue[100];
    strcpy(SearchforValue, "Username=");
    int Pos = ValuePosition(c, SearchforValue);
    char * Username = new char[41];
    char * Password = new char[41];
    char * Befehl = new char[41];
    if (Pos >= 0)
    {

        GetValue(Username, Pos, c, 41);
        cout << "[SERVER]: Username ist: '" << Username << "'" << endl;
    }

    strcpy(SearchforValue, "Password=");
    Pos = ValuePosition(c, SearchforValue);
    if (Pos >= 0)
    {
        GetValue(Password, Pos, c, 41);
        cout << "[SERVER]: Password ist: '" << Password << "'" << endl;
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
    if (Pos >= 0){
        GetValue(Befehl, Pos, c, 41);
        cout << "[SERVER]: Befehl ist: '" << Befehl << "'" << endl;
        if (strcmp(Befehl,"CreateUser") == 0){
            makeUser(Username,Password);
        } else if (strcmp(Befehl,"GetToken") == 0){
            getUser(Username, Password);
        }
        else
        {
            cout << "[SERVER]: Konnte leider mit dem Befehl nichts anfangen, bisher gibt es nur 'CreateUser' und 'GetToken'" << endl;
        }
    } else {
        cout << "[SERVER]: Habe leider keinen richtigen Befehl erhalten";
    }
    
    cout << endl << "[SERVER]: Done" << endl << endl;
    return 0;
}