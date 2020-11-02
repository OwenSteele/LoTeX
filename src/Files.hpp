#include "SrcFiles.h"

using namespace std;

//prototypes
int LineCount();
#define GetPropertyName(ClassName, PropertyName) #PropertyName

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
    string HTMLSection(int section)
    {
        if(section == 0)
        {
            string p1 = "<!DOCTYPE html>\n<html>\n<head>\n<title>";
            string p2 = "</title>\n<style>";
            return p1 + name.substr(0,name.rfind(".")) + p2;
        }
        else if (section == 1) return "</style></head>\n<body>\n";
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
    vector<Styles> addedStyles;
    string ReplaceUnderscore(string text)
    {
        if (text.find("_")!= string::npos)
        {
            string p1 = text.substr(0,text.find("_"));
            string p2 = text.substr(text.find("_")+1, text.length());
            return p1 + "-" + p2;
        }
        else return text;
    }
    string CreateCSS(Styles s)
    {
        string pStart = "." + s.name + " {\n";

        //automate to generic list of Styles properties
        string pC = GetPropertyName(Styles, s.color);
        pC = ReplaceUnderscore(pC.substr(pC.find(".",0)+1,pC.length())) + ": " + s.color + ";\n";
        string pBC = GetPropertyName(Styles, s.background_color);
        pBC = ReplaceUnderscore(pBC.substr(pBC.find(".",0)+1,pBC.length())) + ": " + s.background_color + ";\n";
        string pFF = GetPropertyName(Styles, s.font_family);
        pFF = ReplaceUnderscore(pFF.substr(pFF.find(".",0)+1,pFF.length())) + ": " + s.font_family + ";\n";
        string pFS = GetPropertyName(Styles, s.font_size);
        pFS = ReplaceUnderscore(pFS.substr(pFS.find(".",0)+1,pFS.length())) + ": " + to_string(s.font_size) + "px" + ";\n";
        
        string pContent = pC + pBC + pFF + pFS;
        string pEnd = "}\n";

        return pStart + pContent + pEnd;
    }
    vector<string> existingStyles;
    string FormatCSS(string line)
    {
        vector<Styles> styles = GetDefaultStyles();
        string formattedLine;
        string styleName;
            
        if (line.substr(0,1) != "/") styleName = "text";
        else styleName = line.substr(1,line.find(" ",1)-1);
                
        Styles s = GetStyleByName(styleName);
        if (s.name != "")
        {
            string currentStyleName = s.name;
            auto it = find_if(
                existingStyles.begin(), existingStyles.end(),
                 [&currentStyleName] (const string& eS) {return eS == currentStyleName;});
            if (it == existingStyles.end())
            {
                existingStyles.push_back(currentStyleName);                        formattedLine = CreateCSS(s);
            }
        }
        else ErrMsg("Line: '", line, "' command: '", styleName, "' not recognised."); //create add new style option here
        return formattedLine;
    }
    string FormatHTML(string line)
    {
        string styleName;
            
        if (line.substr(0,1) != "/") styleName = "text";
        else styleName = line.substr(1,line.find(" ",1)-1);

        Styles s = GetStyleByName(styleName);

        string pStart = "\n<" + s.element + " class=\"" + s.name + "\"" + ">";
        string pContent;
        if (line.substr(0,1) != "/") pContent = line;
        else if (line.find(" ") != string::npos) pContent = line.substr(line.find(" ",1), line.length());
        
        string pEnd = "</" + s.element + ">\n";

        return pStart + pContent + pEnd + ((s.element == "a" ) ? "<br>" : "");
    }
    void Publish()
    {
        CreateDefaultStyles();

        string publishedFullPath = path +"/"+ name.substr(0,name.rfind(".")) + ".html";
                
                if(!CheckPathExists(publishedFullPath)) CreateFile(publishedFullPath);

                ofstream pubFile; 
                pubFile.open (publishedFullPath, ios_base::app);
                pubFile << HTMLSection(0);
                for (string line : content) //CSS
                {
                    pubFile << FormatCSS(line);
                }
                pubFile << HTMLSection(1);
                for (string line : content)//Text
                {
                    pubFile << FormatHTML(line);
                }
                pubFile << HTMLSection(2);
                pubFile.close();

                SysMsg("html file published successfully - Location: '", publishedFullPath, "'.");        
    }
};