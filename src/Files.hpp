#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string.h>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <functional>
#include <map>
#include <memory>

#include "SrcFiles.h"

using namespace std;

class LFile
{
    //properties
    public:
    string name;
    string path;

    private:
    vector<string> content;

    //constructor
    public:


    //functions
    string EditLine(int n)
    {
        if (CheckRange(n))
            return content[n];
    }
    vector<string> ViewLines(int lower, int upper)
    {
        if(CheckRange(lower,upper))
        {
            vector<string> lines;
            for(int n = lower; n <= upper; n++)
            {
                string newLine = n + ": " + content[n];
                lines.push_back(newLine);
            }
            return lines;
        }
    }
    bool CheckPath(string path)
    {
        return true;
    }

    private:
    bool CheckRange(int lower, int upper = -1)
    {
        if (upper == -1)
        {
            if (lower > 0 && lower < sizeof(content)) return true;
            else ErrMsg("Value is outside of file line range.");
        }
        else
        {
            if(lower > 0 && lower < sizeof(content) && upper > 0 && upper < sizeof(content))
            {
                if (lower < upper) return true;
                else ErrMsg("First value must be lower than the second."); 
            }
            else ErrMsg(("A value is outside of file line range."));
        }
        return false;
    }
};
void CreateFile(string& fullPath)
{
    ofstream outFile (fullPath);
    outFile.close();
}
inline bool FileExists (const string& fullPath) {
  struct stat buffer;   
  return (stat (fullPath.c_str(), &buffer) == 0); 
}