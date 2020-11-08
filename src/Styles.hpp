#include "SrcFiles.h"

#define GetName(PropertyName) #PropertyName

class Styles
{
    private: 
    std::string _formattedAttributes;
    void FormatAttribute(std::string&& id, std::string value)
    {
        if(!value.empty())
        {
            if (id.find("_")!= std::string::npos) id.replace(id.find("_"),1,"-");
            while (value.substr(0,1) == " ") value = value.substr(1,value.length());
            while (value.substr(value.length()-1,value.length()) == " ") value = value.substr(0,value.length()-1);
            _formattedAttributes += id + ": " + value + ";\n";
        }
    }

    public:
    std::string name;
    std::string element;
    std::string color;
    std::string background_color;
    std::string font_family;
    std::string font_size;
    std::string text_align;

    explicit Styles (
        std::string rname, std::string relement="a",
        std::string rcolor="", std::string rbackground_color="",
        std::string rfont_family="", std::string rfont_size="",
        std::string rtext_align="")
    {
        name = rname;
        element = relement;

        color = rcolor;
        FormatAttribute(GetName(color), color);
        background_color = rbackground_color;
        FormatAttribute(GetName(background_color), background_color);
        font_family = rfont_family;
        FormatAttribute(GetName(font_family), font_family);
        font_size = rfont_size + ((rfont_size.rfind("px") == std::string::npos)? "px": "");
        FormatAttribute(GetName(font_size), font_size);
        text_align = rtext_align;
        FormatAttribute(GetName(text_align), text_align);
    }
    std::string ReturnCSS()
    {
        std::string sName = (name == "body") ? name : "." + name;
        if (_formattedAttributes.empty()) return "";
        return sName + " {\n" + _formattedAttributes + "}\n";
    }
};

inline std::vector<Styles> fileStyles;

std::vector<Styles> GetFileStyles() {return fileStyles;}

void AddStyle(std::vector<std::string> attributes)
{
    while(attributes.size() < 7) attributes.emplace_back("");
    try
    {
        Styles s (attributes[0],attributes[1],attributes[2],attributes[3],attributes[4],attributes[5],attributes[6]);
        fileStyles.emplace_back(s);
    }
    catch(const std::exception& e)
    {   
        ErrMsg("Could not create new style - exception thrown");
        ErrMsg(e.what());
    }
}
void CreateDefaultStyles()
{
    if (fileStyles.empty())
    { 
    Styles normalText ("text","p","black","white","Arial","12");
    Styles title ("title","h1","black","white","Arial","32");
    Styles subtitle ("subtitle","h2","black","white","Arial","20");
    Styles redText ("red","p","red","white","Arial","12");
    Styles highlightedText ("highlight","p","white","red","Arial","12");
    fileStyles.emplace_back(normalText);
    fileStyles.emplace_back(title);
    fileStyles.emplace_back(subtitle);
    fileStyles.emplace_back(redText);
    fileStyles.emplace_back(highlightedText);
    }
}
Styles GetStyleByName(std::string styleName)
{
    Styles emptyStyle ("","","","","","");
    auto it = find_if(fileStyles.begin(), fileStyles.end(), [&styleName](const Styles& obj) {return obj.name == styleName;}); //find if style exists
    if (it != fileStyles.end())
    {
        auto it = find_if(fileStyles.begin(), fileStyles.end(), [&styleName] (const Styles& obj) {return obj.name == styleName;});
        return fileStyles[distance(fileStyles.begin(), it)];
    }
    else return emptyStyle;
}