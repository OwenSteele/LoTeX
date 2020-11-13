#include "SrcFiles.h"

bool CheckPathExists(const std::filesystem::path& dirPath, std::filesystem::file_status status = std::filesystem::file_status{})
    {
        if(std::filesystem::status_known(status) ? std::filesystem::exists(status) : std::filesystem::exists(dirPath)) return true;
        return false;
    }    
inline std::string CorrectPathName(std::string& fullPath)
{
    if(fullPath.substr(0,1)!="/" && fullPath.find(":") == std::string::npos) return "/" + fullPath;
    return fullPath;
}
void CreateFile(std::string&& fullPath)
{
    std::ofstream outFile (fullPath, std::ios_base::app);
    outFile.close();
}
inline static std::unordered_map<std::string, std::vector<std::string>> builtinInlineStatements {
        {"link", {"<a href=\"","\">","</a>"}},
        {"image", {"<img src=\"","\" alt=\"","\">"}}
};

class LFile
{
    public:
    std::vector<std::string> content;
    std::string name;
    std::string path;

    private:
    static std::vector<std::string> GetFileContent (std::string& fullPath)
    {
        std::vector<std::string> content;
        std::string currentLine;
        std::ifstream exFile;
        bool emptyCommentLine;
        bool stylesRegionEnded = false;

	    exFile.open (fullPath, std::ios_base::app);
        while (getline(exFile, currentLine))
        {
            emptyCommentLine = false;
            if (currentLine.find("*||") != std::string::npos)
                 currentLine.replace(currentLine.find("*||"),3,"<i>'Note: ") += "'</i>"; //adds note around comment
            if (currentLine.find("||") != std::string::npos) {
                currentLine = currentLine.substr(0, currentLine.find("||"));
            }
            if (currentLine.find("#endstyles") != std::string::npos) stylesRegionEnded = true;
            if (currentLine.empty() && !stylesRegionEnded) emptyCommentLine = true;
            if(!emptyCommentLine) content.emplace_back(currentLine);
        }
	    exFile.close();

        return content;
    }
    const std::string HTMLSection(int section)
    {
        if(section == 0)
        {
            std::string p1 = "<!DOCTYPE html>\n<html>\n<head>\n<title>";
            std::string p2 = "</title>\n<style>\n";
            Styles sBody = GetStyleByName("body");
            return p1 + name.substr(0,name.rfind(".")) + p2 + (!sBody.name.empty() ? sBody.ReturnCSS() : "");
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
    static std::string HTMLCleanup(std::vector<std::string> text)
    {
        std::string html;
        std::string currentTag;
        std::string lastTag;
        for (int n = 0; n < text.size(); n++)
        {
            if (text[n].empty())
            {
                text.erase(text.begin() +n);
                break;
            }
            if (int pos = text[n].find("\n\n"); pos != std::string::npos)
            {
                text[n].replace(pos,1, "");
            }
            if (text[n].find(">") == std::string::npos) break;

            lastTag = currentTag;
            currentTag = text[n].substr(0,text[n].find(">"));

            if (currentTag == lastTag && currentTag != "<br>")
            {
                text[n-1] = text[n-1].substr(0, text[n-1].find("<",3)) + "\n<br>";
                text[n] = text[n].substr(text[n].find(">",1)+1,text[n].length());
            }
        }
        for(std::string& line : text) html += line;
        return html;
    }
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
    static inline std::string InlineStatement(const std::string& statement)
    {
        std::string functionName = statement.substr(0, statement.find(" "));
        std::string _content = statement.substr(statement.find(" ")+1,statement.length());
        std::string result;
        auto stmt = builtinInlineStatements.find(functionName);
        if (stmt != builtinInlineStatements.end())
        {
            result = stmt->second[0] +
                _content.substr(_content.find(",")+1,_content.length()) +
                stmt->second[1] +
                _content.substr(0,_content.find(",")) +
                stmt->second[2];
        }
        return result;
    }
    static std::string FormatHTML(std::string& line)
    {
        if (line.empty()) return "\n<br>";

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
            if (!s.background_color.empty())
            {
                pStart += "<span>";
                pEnd = "</span>";
            }
            pContent = line.substr(line.find(" ",1)+1, line.length());
            pEnd += "</" + s.element + ">";
        }
        else if (s.name.empty() && line.substr(0,1) == "/")
        {
            pStart = "\n<p>";
            pContent = line.substr(line.find(" ",1)+1, line.length());
            pEnd = "</p>";
        }
        else if (line.substr(0,1) != "{")
        {
            pStart = "\n<p>";
            pContent = line;
            pEnd = "</p>";
        }
        if(line.find("{") != std::string::npos && line.find("}",1) != std::string::npos)
        {
            int inlineOpenPos = line.find("{");
            int inlineClosePos = line.find("}",1);
            if (inlineOpenPos >= inlineClosePos) return pStart + pContent + pEnd;
            std::string lineBeforeClause = line.substr(0,inlineOpenPos);
            std::string inlineClause = InlineStatement(line.substr(inlineOpenPos+1,inlineClosePos-(inlineOpenPos+1)));
            std::string lineAfterClause = line.substr(inlineClosePos+1,line.length());
            pContent = lineBeforeClause + inlineClause + lineAfterClause;
        }
        return "\n" + pStart + pContent + pEnd;
    }
    public:
    explicit LFile(std::string& fullPath)
    {
        int&& lastSlashPos = fullPath.rfind("/");
        if (lastSlashPos != std::string::npos) {
            path = CorrectPathName(fullPath).substr(0, lastSlashPos);
            name = fullPath.substr(lastSlashPos + 1, fullPath.length() - lastSlashPos); //name = after last '/' in path
            if (CheckPathExists(fullPath)) content = GetFileContent(fullPath);
        }
    }
    void Publish(int styleInclusionType = 0)
    {
        std::string&& publishedFullPath = path +"/"+ name.substr(0,name.rfind(".")) + ".html";
        std::vector<std::string> htmlText;
        std::ofstream pubFile;

        if(!CheckPathExists(publishedFullPath))
        {
             CreateFile(std::forward<std::string&&>(publishedFullPath));
             SysMsg("Creating " + name.substr(0,name.rfind(".")) + "'.html' file");
        }
        else
        {
            SysMsg("Existing file found - overwriting.");
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
        pubFile.open (publishedFullPath, std::ios_base::app);
        pubFile << HTMLSection(0);
        for (std::string line : content) pubFile << FormatCSS(line);
        pubFile << HTMLSection(1);
        for (std::string line : content) htmlText.emplace_back(FormatHTML(line));
        pubFile << HTMLCleanup(std::move(htmlText));
        pubFile << HTMLSection(2);
        pubFile.close();
        SysMsg("html file published successfully - Location: '" + publishedFullPath + "'. FINISHED");
    }
};