#include "SrcFiles.h"

class Styles
{
    public:
    std::string name;
    std::string element;
    std::string color;
    std::string background_color;
    std::string font_family;
    int font_size;

    explicit Styles (std::string&& rname, std::string&& relement, std::string&& rcolor, std::string&& rbackground_color, std::string&& rfont_family, int&& rfont_size)
    {
        name = rname;
        element = relement;
        color = rcolor;
        background_color = rbackground_color;
        font_family = rfont_family;
        font_size = rfont_size;
    }
};

inline std::vector<Styles> defaultStyles;

std::vector<Styles> GetDefaultStyles() {return defaultStyles;}

inline Styles normalText ("text","p","black","white","Arial",12);

void CreateDefaultStyles()
{
    if (defaultStyles.size() == 0)
    { 
    Styles title ("title","h1","black","white","Arial",32);
    Styles subtitle ("subtitle","h2","black","white","Arial",20);
    Styles redText ("red","p","red","white","Arial",12);
    Styles highlightedText ("highlight","p","white","red","Arial",12);
    defaultStyles.emplace_back(normalText);
    defaultStyles.emplace_back(title);
    defaultStyles.emplace_back(subtitle);
    defaultStyles.emplace_back(redText);
    defaultStyles.emplace_back(highlightedText);
    }
}
bool StyleExists(std::string& styleName)
{
    auto it = find_if(defaultStyles.begin(), defaultStyles.end(), [&styleName](const Styles& obj) {return obj.name == styleName;}); //find if style exists
    if (it != defaultStyles.end()) return true;
    else return false;                                 
}
Styles GetStyleByName(std::string& styleName)
{
    if (StyleExists(styleName))
    {
        auto it = find_if(defaultStyles.begin(), defaultStyles.end(), [&styleName] (const Styles& obj) {return obj.name == styleName;});
        return defaultStyles[distance(defaultStyles.begin(), it)];
    }
    else return normalText;
}