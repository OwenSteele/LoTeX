#include "SrcFiles.h"

using namespace std;

//prototypes
void CodeMain();

int MenuSelection()
{
	string input;
	stringstream inputSS;
	int inputI =0;

	input =MsgIn();
	inputSS << input.substr(0, 1);
	inputSS >> inputI;

	return inputI;
}

void Menu(map<int, pair<function<void()>, string>> dict, const string& functionName = "")
{
	bool validMenuSelection = false;
	map<int, pair<function<void()>, string>>::iterator option;

	cout << "\n__" << functionName << "() Menu__\n";
	for (auto it = dict.cbegin(); it != dict.cend(); ++it)
	{
		cout <<"   " << it->first << " - " << it->second.second << endl;
	}
	cout << "Press a key and press enter";
	do
	{
		option = dict.find(MenuSelection());

		if (option != dict.end()) validMenuSelection = true;
		else cout << "INPUT_ERR: Invalid input, please try again";

	} while (!validMenuSelection);

	option->second.first();
}

string fileEditFullPath;
void FileEditing()
{
	string fullPath;
	if (fileEditFullPath.empty())
	{
	do
		{
			cout << "Enter file name and path";

			string temp = MsgIn();
			if (temp == "!!BREAK") CodeMain();
			else fullPath= temp;

			fullPath = CorrectPathName(fullPath);
			if (CheckPathExists(fullPath)) break;
			else ErrMsg("Not found, try again - ");
		} while (true);
	}
	else 
	{
		fullPath = fileEditFullPath;
		fileEditFullPath="";
	}
	
	LFile currentFile(fullPath);

	SysMsg("Reading from file: ~", currentFile.path, " - lines: ", to_string(currentFile.LineCount()));
	
	currentFile.ViewLines();
}
void NewFile()
{
	string msg;
	string nameInput;
	string pathInput;
	string fullPath;

    SysMsg(__FUNCTION__,"()");
    cout << "Please enter a file name, and press enter";
    nameInput = MsgIn();
	SysMsg(filesystem::current_path(), " = '/'");
	cout << "Please enter a path name, and press enter";
	pathInput = MsgIn();
	pathInput = CorrectPathName(pathInput);

    if(CheckPathExists(pathInput))
	{
		if (pathInput.substr(pathInput.length()-1,1)!="/") pathInput.append("/");
		fullPath = pathInput + nameInput + ".txt";
		SysMsg("Creating file: ",fullPath);
		CreateFile(fullPath);
	}
	else SysMsg("Path not found.");

	LFile currentFile(fullPath, true);

	if(CheckPathExists(fullPath))
	{
		SysMsg("File Created Successfully '", currentFile.name,"'");
		cout << "Edit this file now (yes/no)?";
		if (MsgIn().substr(0,1) == "y")
		{
			fileEditFullPath = currentFile.path;
			FileEditing();
		} 
	}
	else ErrMsg("File could not be created.");

}

void CodeMain()
{
	do{
		//init here.
    	map<int, pair<function<void()>, string>> mainMenuDict;

		mainMenuDict.insert(make_pair(0, make_pair(&Exit, "Exit")));
		mainMenuDict.insert(make_pair(1, make_pair(&NewFile, "Create new file")));
		mainMenuDict.insert(make_pair(2, make_pair(&FileEditing, "Open File")));

		Menu(mainMenuDict, __FUNCTION__);
	}while(true);

    cin.get();
}


    