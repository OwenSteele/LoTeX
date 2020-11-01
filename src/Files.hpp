#include "SrcFiles.h"

using namespace std;

//prototypes
int LineCount();

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
    ofstream outFile (fullPath, ios_base::app);
    outFile.close();
}

class LFile
{
    private:
    bool CheckRange(int lower, int upper = -1)
    {
        if (upper == -1)
        {
            if (lower > 0 && lower < LineCount()) return true;
            else ErrMsg("Value is outside of file line range.");
        }
        else
        {
            if(lower >= 0 && lower < LineCount() && upper > 0 && upper <= LineCount())
            {
                if (lower < upper) return true;
                else ErrMsg("First value must be lower than the second."); 
            }
            else ErrMsg(("A value is outside of file line range."));
        }
        return false;
    }
    static vector<string> GetFileContent (string& fullPath)
    {
        vector<string> content;
        string currentLine;
        ifstream exFile;
	    exFile.open (fullPath, ios_base::app);
        while (getline(exFile, currentLine))
        {
            content.push_back(currentLine);
        }
	    exFile.close();

        return content;
    }
    string HTMLStartEnd(bool start = true)
    {
        if(start)
        {
            string p1 = "<!DOCTYPE html>\n<html>\n<head>\n<title>";
            string p2 = "</title>\n</head>\n<body>\n";
            return p1 + name.substr(0,name.rfind(".")) + p2;
        }
        return "\n</body>\n</html>";
    }

    public:
    vector<string> content;
    string name;
    string path;

    explicit LFile(string fullPath, bool newFile = false)
    {
        int lastSlashPos = fullPath.rfind("/");
        path = CorrectPathName(fullPath).substr(0,lastSlashPos);
        name = fullPath.substr(lastSlashPos+1,fullPath.length()-lastSlashPos); //name = after last '/' in path
        if(!newFile) content = GetFileContent(fullPath);
    }
    int LineCount()
    {
        return content.size();
    }

    string EditLine(int n)
    {
        if (CheckRange(n)) return content[n];
        return "Line not found.";
    }
    void ViewLines(int lower = -1, int upper = -1)
    {
        if(lower == -1 && upper == -1)
        {
           lower = 0;
           upper = LineCount(); 
        }

        if(CheckRange(lower,upper))
        {
            cout << "___Starting ifstream:___" << endl;
            for(int n = lower; n <= upper; n++)
                cout << to_string(n) + ": " + content[n] << endl;

            cout << "___Ending ifstream:___" << endl;
        } 
    }
    void Publish()
    {
        string publishedFullPath = path +"/"+ name.substr(0,name.rfind(".")) + ".html";
                
                if(!CheckPathExists(publishedFullPath)) CreateFile(publishedFullPath);

                ofstream pubFile; 
                pubFile.open (publishedFullPath, ios_base::app);
                pubFile << HTMLStartEnd();
                pubFile << "<a>test</a>";
                for (string line : content)
                {
                    pubFile << line;
                }
                pubFile << HTMLStartEnd(false);
                pubFile.close();

                SysMsg("html file published successfully - Location: '", publishedFullPath, "'.");        
    }
};