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
    cout << "\n  --!ERR >> '" << message << "'" << endl << endl;
}
void SysMsg(string m, string m2 = "",string m3 = "",string m4 = "",string m5 = "",string m6 = "",string m7 = "") //cleanup
{
    cout << "\n  --!SYS >> '" << m << m2 << m3 << m4 << m5 << m6 << m7 <<"'" << endl << endl;
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