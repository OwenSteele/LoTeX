#include "SrcFiles.h"

//prototypes
void CodeMain();

int MenuSelection()
{
	std::string input;
	std::stringstream inputSS;
	int inputI =0;

	input =MsgIn();
	inputSS << input.substr(0, 1);
	inputSS >> inputI;

	return inputI;
}

void Menu(std::map<int, std::pair<std::function<void()>, std::string>> dict, const std::string& functionName = "")
{
	bool validMenuSelection = false;
	std::map<int, std::pair<std::function<void()>, std::string>>::iterator option;

	std::cout << "\n__" << functionName << "() Menu__\n";
	for (auto it = dict.cbegin(); it != dict.cend(); ++it)
	{
		std::cout <<"   " << it->first << " - " << it->second.second << std::endl;
	}
	std::cout << "Press a key and press enter";
	do
	{
		option = dict.find(MenuSelection());

		if (option != dict.end()) validMenuSelection = true;
		else std::cout << "INPUT_ERR: Invalid input, please try again";

	} while (!validMenuSelection);

	option->second.first();
}
LFile GetFile()
{
	std::string fullPath = "./";
	do
		{
			std::cout << "Enter file name and path";

			std::string temp = MsgIn();
			if (temp == "!!BREAK") CodeMain();
			else fullPath= temp;

			fullPath = CorrectPathName(fullPath);
			if (CheckPathExists(fullPath)) break;
			else ErrMsg("Not found, try again - ");
		} while (true);
	
	LFile currentFile(fullPath);
	SysMsg("File '" + fullPath + "' found successfully!");

	return currentFile;
}
void FilePublish()
{
	LFile currentFile = GetFile();

	currentFile.Publish();
}
void CodeMain()
{
	while(!exitCalled){
    	std::map<int, std::pair<std::function<void()>, std::string>> mainMenuDict;

		mainMenuDict.insert(std::make_pair(0, std::make_pair(&Exit, "Exit")));
		mainMenuDict.insert(std::make_pair(3, std::make_pair(&FilePublish, "Publish")));

		Menu(mainMenuDict, __FUNCTION__);
	}
	exit(0);
}  