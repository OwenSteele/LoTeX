#include "SrcFiles.h"
int SelectStylesType()
{
	const std::vector<std::string> options {"File Styles - Select one of the options:",
			"0: Default Styles",
			"1: Styles included in file",
			"2: Styles listed in separate file"};
	for(const auto& option : options) std::cout << option << std::endl;

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

			if (fullPath == "./" && steel::IsWindowsOS()) fullPath = "c:/Users/owenf/source/repos/LoTex/Example.txt";
            if (fullPath == "./"&& !steel::IsWindowsOS()) fullPath = "/home/owen/Desktop/test.txt";
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
		steel::Menu({{&Exit,"Exit"},{&FilePublish, "Publish Existing File"}}, __FUNCTION__)();
	}
	exit(0);
}  