#ifndef WebHeader
#define WebHeader
#include <iostream>
int AppendToFile(FILE * f, void * WebStruct){
    fwrite(Webstruct, sizeof(WebStruct), 1, f);
}

#endif