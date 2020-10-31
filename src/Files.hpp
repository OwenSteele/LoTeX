#include <functional>
#include "SrcFiles.h"

using namespace std;

//prototypes
inline string CorrectPathName(string& fullPath);

class LFile
{
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
    vector<string> GetFileContent (string& fullPath)
    {
        vector<string> content;
        ifstream exFile;
	    exFile.open (fullPath, ios_base::app);

	    exFile.close();

        return content;
    }

    public:
    vector<string> content;
    string name;
    string path;

    LFile(string fullPath, bool newFile = false)
    {
        int lastSlashPos = fullPath.rfind("/");
        path = CorrectPathName(fullPath);
        name = path.substr(lastSlashPos,path.length()-lastSlashPos); //name = after last '/' in path
        if(!newFile) content = GetFileContent(path);
    }
    int lineCount()
    {
        return content.size();
    }

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
};

bool CheckPathExists(const filesystem::path& dirPath, filesystem::file_status status = filesystem::file_status{})
    {
        if(filesystem::status_known(status) ? filesystem::exists(status) : filesystem::exists(dirPath)) return true;
        return false;
    }    
inline string CorrectPathName(string& fullPath)
{
    if(fullPath.substr(0,1)!="/") return "/" + fullPath;
    return fullPath;
}
void CreateFile(string& fullPath)
{
    fullPath = CorrectPathName(fullPath);
    ofstream outFile (fullPath);
    outFile << "test";
    outFile.close();
}
inline bool FileExists (const string& fullPath) 
{
    string path = fullPath;
    path = CorrectPathName(path);
    struct stat buffer;   
    return (stat (path.c_str(), &buffer) == 0); 
}