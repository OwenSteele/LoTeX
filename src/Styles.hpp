#include "SrcFiles.h"

using namespace std;

class Styles
{
    public:
    string name;
    string element;
    string color;
    string background_color;
    string font_family;
    int font_size;

    explicit Styles (string _name, string _element, string _color, string _background_color, string _font_family, int _font_size)
    {
        name = _name;
        element = _element;
        color = _color;
        background_color = _background_color;
        font_family = _font_family;
        font_size = _font_size;
    }
};

inline vector<Styles> defaultStyles;

vector<Styles> GetDefaultStyles() {return defaultStyles;}

inline Styles *normalText = new Styles("text","p","black","white","Arial",12);

void CreateDefaultStyles()
{
    if (defaultStyles.size() == 0)
    { 
    defaultStyles.push_back(*normalText);
    Styles *title = new Styles("title","h1","black","white","Arial",32);
    defaultStyles.push_back(*title);
    Styles *subtitle = new Styles("subtitle","h2","black","white","Arial",20);
    defaultStyles.push_back(*subtitle);
    Styles *redText = new Styles("red","p","red","white","Arial",12);
    defaultStyles.push_back(*redText);
    Styles *highlightedText = new Styles("highlight","p","white","red","Arial",12);
    defaultStyles.push_back(*highlightedText);
    }
}

bool StyleExists(string styleName)
{
    auto it = find_if(defaultStyles.begin(), defaultStyles.end(), [&styleName](const Styles& obj) {return obj.name == styleName;}); //find if style exists
    if (it != defaultStyles.end())
    {
        return true;
    }
    else return false;                                 
}
Styles GetStyleByName(string styleName)
{
    if (StyleExists(styleName))
    {
        auto it = find_if(defaultStyles.begin(), defaultStyles.end(), [&styleName] (const Styles& obj) {return obj.name == styleName;});
        return defaultStyles[distance(defaultStyles.begin(), it)];
    }
    else return *normalText;
}