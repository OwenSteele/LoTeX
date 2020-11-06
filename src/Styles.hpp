#include "SrcFiles.h"

#define GetPropertyName(PropertyName) #PropertyName

class Styles
{
    public:
    std::string name;
    std::string element;
    std::string color;
    std::string background_color;
    std::string font_family;
    int font_size;

    explicit Styles (std::string rname, std::string relement, std::string rcolor, std::string rbackground_color, std::string rfont_family, int rfont_size)
    {
        name = rname;
        element = relement;
        color = rcolor;
        background_color = rbackground_color;
        font_family = rfont_family;
        font_size = rfont_size;;

    }
    private:
    std::string ReplaceUnderscore(std::string text)
    {
        if (text.find("_")!= std::string::npos) text.replace(text.find("_"),1,"-");
        return text;
    }
    public:
    std::string ReturnCSS()
    {
        std::string tag = "." + name + " {\n";
        std::string css = tag + 
        GetPropertyName(color) + ": " + color + ";\n" +
        ReplaceUnderscore(GetPropertyName(background_color)) + ": " + background_color + ";\n" +
        ReplaceUnderscore(GetPropertyName(font_family)) + ": " + font_family + ";\n" +
        ReplaceUnderscore(GetPropertyName(font_size)) + ": " + std::to_string(font_size) + "px;\n}\n";
        
        return css;
    }
};

inline std::vector<Styles> fileStyles;

std::vector<Styles> GetFileStyles() {return fileStyles;}

void AddStyle(std::vector<std::string> attributes)
{
    try
    {
        Styles s (attributes[0],attributes[1],attributes[2],attributes[3],attributes[4],std::stoi(attributes[5]));
        fileStyles.emplace_back(s);
    }
    catch(const std::exception& e)
    {   
        ErrMsg("Could not create new style - exception thrown");
        ErrMsg(e.what());
    }
}

inline Styles normalText ("text","p","black","white","Arial",12);
void CreateDefaultStyles()
{
    if (fileStyles.size() == 0)
    { 
    Styles title ("title","h1","black","white","Arial",32);
    Styles subtitle ("subtitle","h2","black","white","Arial",20);
    Styles redText ("red","p","red","white","Arial",12);
    Styles highlightedText ("highlight","p","white","red","Arial",12);
    fileStyles.emplace_back(normalText);
    fileStyles.emplace_back(title);
    fileStyles.emplace_back(subtitle);
    fileStyles.emplace_back(redText);
    fileStyles.emplace_back(highlightedText);
    }
}
bool StyleExists(std::string& styleName)
{
    auto it = find_if(fileStyles.begin(), fileStyles.end(), [&styleName](const Styles& obj) {return obj.name == styleName;}); //find if style exists
    if (it != fileStyles.end()) return true;
    else return false;                                 
}
Styles GetStyleByName(std::string& styleName)
{
    if (StyleExists(styleName))
    {
        auto it = find_if(fileStyles.begin(), fileStyles.end(), [&styleName] (const Styles& obj) {return obj.name == styleName;});
        return fileStyles[distance(fileStyles.begin(), it)];
    }
    else return normalText;
}