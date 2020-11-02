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


vector<Styles> DefaultStyles()
{
    vector<Styles> defaultStyles;

    Styles *title = new Styles("title","h1","black","white","Arial",20);
    defaultStyles.push_back(*title);
    Styles *subtitle = new Styles("subtitle","h2","black","white","Arial",16);
    defaultStyles.push_back(*subtitle);
    Styles *text = new Styles("text","h1","black","white","Arial",12);
    defaultStyles.push_back(*text);
    Styles *redText = new Styles("red","h1","red","white","Arial",12);
    defaultStyles.push_back(*redText);
    Styles *highlightedText = new Styles("highlight","h1","white","red","Arial",12);
    defaultStyles.push_back(*highlightedText);

    return defaultStyles;
}