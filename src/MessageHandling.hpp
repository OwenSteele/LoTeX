#include "SrcFiles.h"

const std::vector<std::string> knownCommands = {"'###' = Exit program.", "'#h' = Show Help", "'#b' = break out of current area."};

bool exitCalled = false;
void CodeMain();

void Exit() { exitCalled = true; }

std::string GetTime()
{
    time_t rawTime;
    struct tm * timeinfo;
    char buffer[80];
    time(&rawTime);
    timeinfo = localtime(&rawTime);
    strftime(buffer,sizeof(buffer), "%H:%M:%S", timeinfo);
    std::string str(buffer);
    return str;
}

void ErrMsg(std::string&& message) { 
    std::cout << "      --!ERR >> '" << message << "' " << GetTime() << std::endl; }
void SysMsg(std::string&& message) {
    std::cout << "      --!SYS >> '" << message << "' " << GetTime()<< std::endl; }

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
int MsgInInt(std::optional<int>&& options = NULL)
{
    do{
        try{
            int input = std::stoi(MsgIn());
            if (options != NULL)
            {
                if (input < options && input > -1) return input;
                else ErrMsg("Input is outside options range");
            }
            else return input;
        }
        catch(const std::exception& e) {ErrMsg("You must enter an integer");}
    } while (true);
}