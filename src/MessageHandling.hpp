#include "SrcFiles.h"


const std::vector<std::string> knownCommands = {"'###' = Exit program.", "'#h' = Show Help", "'#b' = break out of current area."};

bool exitCalled = false;
void CodeMain();

void Exit()
{
    //add message?
    exitCalled = true;
}

void ErrMsg(std::string&& message) 
{
    std::cout << "\n  --!ERR >> '" << message << "'" << std::endl << std::endl;
}
void SysMsg(std::string&& message) //cleanup
{
    std::cout << "\n  --!SYS >> '" << message << "'" << std::endl << std::endl;
}
std::string MsgIn()
    {
        bool validInput = false;
        std::string input;

        do{
            std::cout << " << ";
            std::cin >> input; //aim to have only std::cin in program here

            //check for known cmds
            if (input == "###") 
            {
                Exit();
                CodeMain();
                } //always check if program exit is called
            else if (input == "#h")
            {
                SysMsg("___HELP - COMMANDS___");
                for(const std::string& cmd : knownCommands)
                {
                    std::cout << "    - " << cmd << std::endl;
                }
                std::cout << "_____________________" << std::endl;
            }
            else if (input == "#b")
            {
                SysMsg("Breakout called.");
                return "!!BREAK"; 
            } 
            else if(input != "") validInput=true;

            else std::cout << "ERR, retry << ";
        }while (!validInput);

        return input;
    }