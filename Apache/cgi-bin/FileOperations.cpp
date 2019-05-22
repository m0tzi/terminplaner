#include <iostream>
#include "C:\temp\Apache\cgi-bin\header.h" //Kann ignoriert werden, denke ich?

int AppendToFile(FILE * f, void * Webstruct){
    fwrite(Webstruct,sizeof(Webstruct),1,f);
}