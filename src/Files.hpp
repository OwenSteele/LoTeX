#include "SrcFiles.h"

//prototypes
int LineCount();

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
    static std::string RemoveComments(std::string line)
    {
        if (line.find("*||") != std::string::npos) line.replace(line.find("*||"),1,""); //keeps || in line
        else if (line.find("||") != std::string::npos) line = line.substr(0, line.find("||"));
        return line;    
    }
    static std::vector<std::string> GetFileContent (std::string& fullPath)
    {
        std::vector<std::string> content;
        std::string currentLine;
        std::ifstream exFile;

	    exFile.open (fullPath, std::ios_base::app);
        while (getline(exFile, currentLine)) content.emplace_back(RemoveComments(std::move(currentLine)));
	    exFile.close();

        return content;
    }
    std::string HTMLSection(int section)
    {
        if(section == 0)
        {
            std::string p1 = "<!DOCTYPE html>\n<html>\n<head>\n<title>";
            std::string p2 = "</title>\n<style>\n";
            Styles sBody = GetStyleByName("body");
            return p1 + name.substr(0,name.rfind(".")) + p2 + (sBody.name !="" ? sBody.ReturnCSS() : "");
        }
        else if (section == 1) return "</style>\n</head>\n<body>\n";
        return "\n</body>\n</html>";
    }
    void GetCustomStyles(std::vector<std::string> lines)
    {
        int stylesLineStart=-1;
        int stylesLineEnd=-1;
        std::vector<std::string> styleLines;
        for (int n = 0; n < lines.size(); n++)
        {
            if(lines[n].substr(0,7) == "#styles") stylesLineStart = n;
            else if (lines[n].substr(0,10) == "#endstyles") stylesLineEnd = n;
            
            if (stylesLineStart != -1 && n > stylesLineStart && stylesLineEnd == -1) styleLines.emplace_back(lines[n]);
            if (stylesLineEnd > 0) break;
        }
        if (stylesLineStart == -1 && stylesLineEnd == -1)
        { 
            ErrMsg("Styles region not found - using default styles");
            CreateDefaultStyles();
        }
        else
        {            
            for(std::string line : styleLines)
            {
                std::vector<std::string> newStyle;
                if(line.substr(0,1) == "/")
                {
                    std::string styleName = line.substr(1,line.find(" ")-1);
                    if (styleName == "default")
                    {
                        newStyle.emplace_back("body");
                        newStyle.emplace_back("");
                    }
                    else newStyle.emplace_back(styleName);
                    line = line.substr(line.find(" ")+1, line.length());

                    do{
                        int&& comma = line.find(",");
                        newStyle.emplace_back(line.substr(0,comma));
                        line = line.substr(comma+1, line.length());
                    }while (line.find(",") != std::string::npos);
                    newStyle.emplace_back(line);
                }
                else if (!line.empty()) ErrMsg("Invalid style syntax in document: '" + line + "'. Each line and style addition must start with a '/'");

                AddStyle(newStyle);
            }
            SysMsg("User-defined styles found and compiled.");
            //remove styles from content
            for (int n = stylesLineStart; n <= stylesLineEnd; n++) content.erase(content.begin());
        }
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
    std::vector<std::string> existingStyles;
    std::string FormatCSS(std::string& line)
    {
        std::shared_ptr<std::vector<Styles>> styles = std::make_shared<std::vector<Styles>>(GetFileStyles());
        std::string formattedLine;
        std::string styleName;
            
        if (line.substr(0,1) != "/") styleName = "empty";
        else styleName = line.substr(1,line.find(" ",1)-1);
        
        if (styleName == "default") styleName = "empty";        
        Styles s = GetStyleByName(styleName);
        if (!s.name.empty())
        {
            std::string currentStyleName = s.name;
            auto it = find_if(
                existingStyles.begin(), existingStyles.end(),
                 [&currentStyleName] (const std::string& eS) {return eS == currentStyleName;});
            if (it == existingStyles.end())
            {
                existingStyles.emplace_back(currentStyleName);                        
                formattedLine = s.ReturnCSS();
            }
        } //create add new style option here?
        return formattedLine;
    }
    std::string FormatHTML(std::string& line)
    {
        if (line.empty()) return "</br>";

        std::string styleName;
        std::string pStart;
        std::string pContent;
        std::string pEnd;
            
        if (line.substr(0,1) == "/") styleName = line.substr(1,line.find(" ",1)-1);
        else styleName = "empty";
        Styles s = GetStyleByName(styleName);

        if (!s.name.empty())
        {
            pStart = "\n<" + s.element + " class=\"" + s.name + "\"" + ">";
            pContent = line.substr(line.find(" ",1)+1, line.length());
            pEnd = "</" + s.element + ">";
        }
        else if (s.name.empty() && line.substr(0,1) == "/")
        { 
            pStart = "\n<p>";
            pContent = line.substr(line.find(" ",1)+1, line.length());
            pEnd = "</p>";
        }
        else
        {
            pStart = "\n<p>";
            pContent = line;
            pEnd = "</p>";
        }
        
        
        return pStart + pContent + pEnd;
    }
    std::string HTMLCleanup(std::vector<std::string> text)
    {
        std::string html;
        std::string currentTag = "";
        std::string lastTag;
        for (int n = 0; n < text.size(); n++) 
        {
            if (text[n].find(">") == std::string::npos) break;

            lastTag = currentTag;
            currentTag = text[n].substr(0,text[n].find(">"));

            if (currentTag == lastTag && currentTag != "</br>")
            {
                text[n-1] = text[n-1].substr(0, text[n-1].find("<",3)) + " </br>\n";
                text[n] = text[n].substr(text[n].find(">",1)+1,text[n].length());
            }
        }
        for(std::string line : text) html += line;
        return html;
    }
    void Publish(int styleInclusionType)
    {
        std::string&& publishedFullPath = path +"/"+ name.substr(0,name.rfind(".")) + ".html";
                 
        if(!CheckPathExists(publishedFullPath))
        {
             CreateFile(std::forward<std::string&&>(publishedFullPath));
             SysMsg("Creating " + name.substr(0,name.rfind(".")) + "'.html' file");
        }
        else
        {
            SysMsg("Existing file found - overwriting.");
            std::ofstream pubFile; 
            pubFile.open (publishedFullPath,  std::ofstream::out | std::ofstream::trunc);
            pubFile.close();
        }
        

        if (styleInclusionType == 1)
        {
            SysMsg("Locating in-file '#style' region");
            GetCustomStyles(content);
        }
        else if (styleInclusionType == 2)
        {
            std::string input;
            std::string styleFullPath;
            do{
                std::cout << "Enter file name and path containing your styles";
                input = MsgIn();
                styleFullPath = CorrectPathName(input);

                if(CheckPathExists(styleFullPath))
                {
                    SysMsg("Styles file found.");
                    break;
                }
                else ErrMsg("File not found, try again");
            }while (true);

            std::vector<std::string> styleContent = GetFileContent(styleFullPath);
            SysMsg("File contents retrieved.");
            GetCustomStyles(std::move(styleContent));
        } //other file
        else CreateDefaultStyles();

        SysMsg("Writing to '.html' file.");
        std::ofstream pubFile; 
        pubFile.open (publishedFullPath, std::ios_base::app);

        pubFile << HTMLSection(0);

        for (std::string line : content) pubFile << FormatCSS(line);

        pubFile << HTMLSection(1);

        std::vector<std::string> htmlText;

        for (std::string line : content) htmlText.emplace_back(FormatHTML(line));

        pubFile << HTMLCleanup(std::move(htmlText));

        pubFile << HTMLSection(2);

        pubFile.close();

        SysMsg("html file published successfully - Location: '" + publishedFullPath + "'. FINISHED");        
    }
};