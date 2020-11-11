#include "SrcFiles.h"

const std::vector<std::string> knownCommands = {"'###' = Exit program.", "'#h' = Show Help", "'#b' = break out of current area."};

bool exitCalled = false;
void CodeMain();

void Exit() { exitCalled = true; }

static void ErrMsg(std::string&& message) {
    std::cout << "      --!ERR >> '" << message << "' " << steel::GetTime() << std::endl; }
static void SysMsg(std::string&& message) {
    std::cout << "      --!SYS >> '" << message << "' " << steel::GetTime()<< std::endl; }

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
            else if(!input.empty()) validInput=true;

            else std::cout << "ERR, retry << ";
        }while (!validInput);

        return input;
    }
int MsgInInt(int options = -1)
{
    do{
        try{
            if (int input = std::stoi(MsgIn()); options != -1)
            {
                if (input < options && input > -1) return input;
                else ErrMsg("Input is outside options range");
            }
            else return input;
        }
        catch(const std::exception& e) {ErrMsg("You must enter an integer");}
    } while (true);
}