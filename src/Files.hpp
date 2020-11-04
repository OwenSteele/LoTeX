#include "SrcFiles.h"

//prototypes
int LineCount();
#define GetPropertyName(ClassName, PropertyName) #PropertyName

bool CheckPathExists(const std::filesystem::path& dirPath, std::filesystem::file_status status = std::filesystem::file_status{})
    {
        if(std::filesystem::status_known(status) ? std::filesystem::exists(status) : std::filesystem::exists(dirPath)) return true;
        return false;
    }    
inline std::string CorrectPathName(std::string& fullPath)
{
    if(fullPath.substr(0,1)!="/") return "/" + fullPath;
    return fullPath;
}
void CreateFile(std::string&& fullPath)
{
    std::ofstream outFile (fullPath, std::ios_base::app);
    outFile.close();
}

class LFile
{
    public:
    std::vector<std::string> content;
    std::string name;
    std::string path;

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
    static std::vector<std::string> GetFileContent (std::string& fullPath)
    {
        std::vector<std::string> content;
        std::string currentLine;
        std::ifstream exFile;

	    exFile.open (fullPath, std::ios_base::app);
        while (getline(exFile, currentLine)) content.emplace_back(currentLine);
	    exFile.close();

        return content;
    }
    std::string HTMLSection(int section)
    {
        if(section == 0)
        {
            std::string p1 = "<!DOCTYPE html>\n<html>\n<head>\n<title>";
            std::string p2 = "</title>\n<style>\n";
            return p1 + name.substr(0,name.rfind(".")) + p2;
        }
        else if (section == 1) return "</style>\n</head>\n<body>\n";
        return "\n</body>\n</html>";
    }
    
    public:
    explicit LFile(std::string& fullPath, bool newFile = false)
    {
        int&& lastSlashPos = fullPath.rfind("/");
        path = CorrectPathName(fullPath).substr(0,lastSlashPos);
        name = fullPath.substr(lastSlashPos+1,fullPath.length()-lastSlashPos); //name = after last '/' in path
        if(!newFile) content = GetFileContent(fullPath);
    }
    std::vector<Styles> addedStyles;
    std::string ReplaceUnderscore(std::string text)
    {
        if (text.find("_")!= std::string::npos)
        {
            std::string p1 = text.substr(0,text.find("_"));
            std::string p2 = text.substr(text.find("_")+1, text.length());
            return p1 + "-" + p2;
        }
        else return text;
    }
    std::string CreateCSS(Styles s)
    {
        std::string pStart = "." + s.name + " {\n";

        //automate to generic list of Styles properties
        std::string pC = GetPropertyName(Styles, s.color);
        pC = ReplaceUnderscore(pC.substr(pC.find(".",0)+1,pC.length())) + ": " + s.color + ";\n";
        std::string pBC = GetPropertyName(Styles, s.background_color);
        pBC = ReplaceUnderscore(pBC.substr(pBC.find(".",0)+1,pBC.length())) + ": " + s.background_color + ";\n";
        std::string pFF = GetPropertyName(Styles, s.font_family);
        pFF = ReplaceUnderscore(pFF.substr(pFF.find(".",0)+1,pFF.length())) + ": " + s.font_family + ";\n";
        std::string pFS = GetPropertyName(Styles, s.font_size);
        pFS = ReplaceUnderscore(pFS.substr(pFS.find(".",0)+1,pFS.length())) + ": " + std::to_string(s.font_size) + "px" + ";\n";
        
        std::string pContent = pC + pBC + pFF + pFS;
        std::string pEnd = "}\n";

        return pStart + pContent + pEnd;
    }
    std::vector<std::string> existingStyles;
    std::string FormatCSS(std::string& line)
    {
        std::shared_ptr<std::vector<Styles>> styles = std::make_shared<std::vector<Styles>>(GetDefaultStyles());
        std::string formattedLine;
        std::string styleName;
            
        if (line.substr(0,1) != "/") styleName = "text";
        else styleName = line.substr(1,line.find(" ",1)-1);
                
        Styles s = GetStyleByName(styleName);
        if (!s.name.empty())
        {
            std::string currentStyleName = s.name;
            auto it = find_if(
                existingStyles.begin(), existingStyles.end(),
                 [&currentStyleName] (const std::string& eS) {return eS == currentStyleName;});
            if (it == existingStyles.end())
            {
                existingStyles.emplace_back(currentStyleName);                        formattedLine = CreateCSS(s);
            }
        }
        else ErrMsg("Line: '" + line + "' command: '" + styleName + "' not recognised."); //create add new style option here
        return formattedLine;
    }
    std::string FormatHTML(std::string& line)
    {
        if (line.empty()) return "</br>";

        std::string styleName;
            
        if (line.substr(0,1) != "/") styleName = "text";
        else styleName = line.substr(1,line.find(" ",1)-1);

        Styles s = GetStyleByName(styleName);

        std::string pStart = "\n<" + s.element + " class=\"" + s.name + "\"" + ">";
        std::string pContent;
        if (line.substr(0,1) != "/") pContent = line;
        else if (line.find(" ") != std::string::npos) pContent = line.substr(line.find(" ",1)+1, line.length());
        
        std::string pEnd = "</" + s.element + ">";

        return pStart + pContent + pEnd;
    }
    void Publish()
    {
        CreateDefaultStyles();

        std::string&& publishedFullPath = path +"/"+ name.substr(0,name.rfind(".")) + ".html";
                 
                if(!CheckPathExists(publishedFullPath)) CreateFile(std::forward<std::string&&>(publishedFullPath));

                std::ofstream pubFile; 
                pubFile.open (publishedFullPath, std::ios_base::app);
                pubFile << HTMLSection(0);
                for (std::string line : content) //CSS
                {
                    pubFile << FormatCSS(line);
                }
                pubFile << HTMLSection(1);
                for (std::string line : content)//Text
                {
                    pubFile << FormatHTML(line);
                }
                pubFile << HTMLSection(2);
                pubFile.close();

                SysMsg("html file published successfully - Location: '" + publishedFullPath + "'. FINISHED");        
    }
};