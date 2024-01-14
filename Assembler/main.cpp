#include <iostream>
#include "Assembler.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc == 1){
        cout<<"Erroe: require file name: \n./Assembler [file name]"<<endl;
        exit(1);
    }
    string fileName = argv[argc - 1];
    Assembler ab;
    ab.assemble(fileName);
    return 0;
}

