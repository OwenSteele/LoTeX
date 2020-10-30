#include <iostream>
#include <string.h>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <memory>
#include <numeric>
#include <chrono>
#include <ctime> 

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

void Menu(map<int, pair<function<void()>, string>> dict, string functionName = "")
{
	bool validMenuSelection = false;
	map<int, pair<function<void()>, string>>::iterator option;

	cout << "\n__" << functionName << "() Menu__\n";
	for (auto it = dict.cbegin(); it != dict.cend(); ++it)
	{
		cout <<"   " << it->first << " - " << it->second.second << endl;
	}
	cout << "Press a key and press enter: ";

	do
	{
		option = dict.find(MenuSelection());

		if (option != dict.end()) validMenuSelection = true;
		else cout << "INPUT_ERR: Invalid input, please try again";

	} while (!validMenuSelection);

	option->second.first();
}

void NewFile()
{
	LFile currentFile;
	string msg;
	string pathInput;
	string fullPath;

    SysMsg(__FUNCTION__);

    cout << "Please enter a file name, and press enter";
    currentFile.name = MsgIn();
	cout << "Please enter a path name (typing only '/' will set it as the current program directory), and press enter";
	pathInput = MsgIn();
    if(currentFile.CheckPath(pathInput))
	{
		if (pathInput.substr(pathInput.length()-1,1)!="/") pathInput.append("/");
		fullPath = pathInput + currentFile.name + ".txt";
		SysMsg(fullPath);
		CreateFile(fullPath);
	}

	if(FileExists(fullPath))
	{
		 msg = "File Created Successfully '" + currentFile.name + ".txt'";
		SysMsg(msg);
	}
	else ErrMsg("File could not be created.");
}

void FileEditing()
{
	string fileToOpen;
	do
	{
		cout << "Enter file name";

		string temp = MsgIn();
		if (temp == "!!BREAK") CodeMain();
		else fileToOpen = temp;

		if (FileExists(fileToOpen)) break;
		else ErrMsg("Not found, try again - ");
	} while (true);
	
	ofstream exFile ;
	exFile.open (fileToOpen);
	exFile.close();
	SysMsg("File Closed.");
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


    