#include <string.h>
#include <iostream>

#include "SrcFiles.h"

using namespace std;

void Exit()
{
    //add message?
    exit;
}

void ErrMsg(string message) 
{
    cout << "\n  --!ERR >> '" << message << "'" << endl;
}
void SysMsg(string message)
{
    cout << "\n  --!SYS >> '" << message << "'" << endl;
}
string MsgIn()
    {
        bool validInput = false;
        string input;

        do{
            cout << " << ";
            cin >> input;

            if(input != "") validInput=true;
            else cout << "ERR, retry << ";
        }while (!validInput);

        return input;
    }