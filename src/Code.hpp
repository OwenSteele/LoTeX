#include "SrcFiles.h"

void Menu(std::unordered_map<int, std::pair<std::function<void()>, std::string>> dict, const std::string&& functionName = "")
{
	std::cout << "\n__" << functionName << "() Menu__\n";
	for (auto it : dict) std::cout <<"   " << it.first << " - " << it.second.second << std::endl;
	std::cout << "Press a key and press enter";

	do{		
		auto option = dict.find(MsgInInt());
		if (option != dict.end()) return option->second.first();
		else ErrMsg("Invalid input, please try again");
	} while (true);
}
int SelectStylesType()
{
	const std::vector<std::string> options {"File Styles - Select one of the options:",
			"0: Default Styles",
			"1: Styles included in file",
			"2: Styles listed in separate file"};
	for(auto option : options) std::cout << option << std::endl;

	return MsgInInt(options.size());			
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
			
			//temp debugging
			if (fullPath == "./") fullPath = "/home/owen/Desktop/test.txt";

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

	currentFile.Publish(SelectStylesType());
}
void CodeMain()
{
	while(!exitCalled){
    	std::unordered_map<int, std::pair<std::function<void()>, std::string>> newMenu;
		newMenu.insert(std::make_pair(0, std::make_pair(&Exit, "Exit")));
		newMenu.insert(std::make_pair(1, std::make_pair(&FilePublish, "Publish")));
		Menu(newMenu, __FUNCTION__);
	}
	exit(0);
}  