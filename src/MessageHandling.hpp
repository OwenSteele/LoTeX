#include <string.h>
#include <iostream>
#include <vector>

#include "SrcFiles.h"

using namespace std;

const vector<string> knownCommands = {"'###' = Exit program.", "'#h' = Show Help", "'#b' = break out of current area."};

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
            cin >> input; //aim to have only cin in program here

            //check for known cmds
            if (input == "###") Exit(); //always check if program exit is called
            else if (input == "#h")
            {
                SysMsg("___HELP - COMMANDS___");
                for(const string& cmd : knownCommands)
                {
                    cout << "    - " << cmd << endl;
                }
                cout << "_____________________" << endl;
            }
            else if (input == "#b")
            {
                SysMsg("Breakout called.");
                return "!!BREAK"; 
            } 
            else if(input != "") validInput=true;

            else cout << "ERR, retry << ";
        }while (!validInput);

        return input;
    }