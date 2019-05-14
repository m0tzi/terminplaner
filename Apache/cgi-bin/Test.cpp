#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

#pragma warning(disable:4996)

int ValuePosition(char c[1000], char SearchforString[100]){
    // char * values = malloc(1,sizeof(char));
    int UsernameCounter = 0;
    for(int ci = 0; c[ci]!='\0';ci++){
        if(c[ci] == SearchforString[UsernameCounter]){
            UsernameCounter++;
        }
        else{
            UsernameCounter = 0;
        }
        if(SearchforString[UsernameCounter]=='\0'){
            if(c[ci+1]!='\0'&&c[ci+1]!='\0'){
                ci = ci + 1;
                return ci;
            }
            else{
                cout << "Found Value, but empty"; // DELETE
            }
        }
    }
    return -1;
}

void GetValue(char * Value, int pos, char c[1000], int Maxlength){
    int Valueindex = 0;
    for(int i = pos; c[i] != '\0' && c[i] != '&'; i++){
        if(c[i] == '+')
            Value[Valueindex] = ' ';
        else
            Value[Valueindex] = c[i];
        Valueindex++;
    }
    Value[Valueindex] = '\0';
}

int main(){
    char c[1000];
    cin>>c;
    cout << "Content-Type: text/html\r\n\r\n";
    cout << "[SERVER]: Erhaltene Daten: " << c << endl;


    //Funktionen
    char SearchforValue[100];
    strcpy(SearchforValue,"Username=");
    int Pos = ValuePosition(c,SearchforValue);
    if(Pos >= 0){
        char * Username = new char[41];
        GetValue(Username, Pos, c, 41);
        cout << "[SERVER]: Username ist: '" << Username << "'"<< endl;
    }

    strcpy(SearchforValue,"Password=");
    Pos = ValuePosition(c,SearchforValue);
    if(Pos >= 0){
        char * Password = new char[41];
        GetValue(Password, Pos, c, 41);
        cout << "[SERVER]: Password ist: '" << Password << "'" << endl;
    }

    return 0;
}