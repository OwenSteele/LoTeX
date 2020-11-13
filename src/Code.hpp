#include "SrcFiles.h"
int SelectStylesType()
{
	const std::vector<std::string> options {"File Styles - Select one of the options:",
			"0: Default Styles",
			"1: Styles included in file",
			"2: Styles listed in separate file"};
	for(const auto& option : options) std::cout << option << std::endl;

	return MsgInInt(options.size());
}
inline std::string FileInfo(bool pathOnly = false)
{
    std::string fullPath = "./";
    do{
        if (pathOnly) std::cout << "Enter path only";
        else std::cout << "Enter file name and path";
        std::string temp = MsgIn();
        if (temp != "!!BREAK") {
            fullPath = temp;
            //temp debugging

            if (pathOnly) {
                if (fullPath == "./" && steel::IsWindowsOS()) fullPath = "c:/Users/owenf/source/repos/LoTex";
                else if (fullPath == "./" && !steel::IsWindowsOS()) fullPath = "/home/owen/Desktop";

                if (fullPath.rfind("/") > (fullPath.rfind(".") != std::string::npos) ? fullPath.rfind(".") :
                    fullPath.rfind("/") + 1)
                    fullPath.substr(fullPath.rfind("/"));
            } else {
                if (fullPath == "./" && steel::IsWindowsOS())
                    fullPath = "c:/Users/owenf/source/repos/LoTex/Example.txt";
                else if (fullPath == "./" && !steel::IsWindowsOS()) fullPath = "/home/owen/Desktop/test.txt";
            }

            fullPath = CorrectPathName(fullPath);
            if (CheckPathExists(fullPath)) break;
            else ErrMsg("Not found, try again - ");
        }
        else break;

    } while (true);

    return fullPath;
}
void CreateTemplateFile()
{
    std::string fullPath = FileInfo(true);
    std::string content = "||     LoTeX, by Owen Steele 2020.\n\n"
                          "|| comments are made using '||' - these will not be included in the published file.\n"
                          "||      To have comments included in the published file as notes, '*||' will have the comment included as: 'Note: '.\n\n"
                          "|| For custom styles, a styles region must be included at the top of the file, with opening '#styles', and closing '#endstyles'.\n"
                          "||      To define a style in this region, type '/[style name] [html element],[color],[background colour], [font family], [font size], [alignment]'\n"
                          "||      Leaving a field black, e.g. '/myStyle h1,black,,arial...', will rely on the default style.\n"
                          "||      To create the default style type '/default' and the style name, default styles do NOT have an element (first attributes is 'color')..\n"
                          "||      After the '#endstyles' line, the text to be translated to html begins.\n\n"
                          "|| To set a line to a defined style, start the line with '/[defined style name]', '/default' tags will be removed.\n"
                          "||      If the style written is not defined, the tag will be removed and set to the default style instead.\n\n"
                          "|| Inline code can also be defined with '{ }', this will add in elements in the same line.\n"
                          "||      Currently two inline statements exist, for links and images.\n"
                          "||      Syntax: {[defined statement name] [text for link/image description],[path directory or full URL]}.\n"
                          "||      E.g. '{link click here, https://owensteele.co.uk}' and '{image this is me, C:/pictures/photo_of_me.jpg}'.\n\n"
                          "|| Be careful as spaces ' ' and newlines are important for syntax.\n\n"
                          "|| Below is a template to help you get started. Once it is ready, launch LoTeX and select publish!\n\n"
                          "#styles\n"
                          "/default black, white, verdana, 14, left\n"
                          "/title h1,green,black, lucinda console, 24, center\n"
                          "/myStyle p,red,,, 18, justify\n"
                          "#endstyles\n\n"
                          "/title Template\n\n"
                          "This is some sample text, for another example, see 'Example.txt' and published 'Example.html' files.\n"
                          "Comments to the right will not be included in the published file ||you won't see me in the html file!\n"
                          "But noted comments will appear: *|| this is a note for the html file.\n\n"
                          "/myStyle Lines can have both a style and an inline statement, {link like this!, https://owensteele.co.uk}.";

    fullPath +="/Template.txt";
    std::ofstream templateFile (fullPath, std::ios_base::app);
    templateFile << content;
    templateFile.close();

    if(CheckPathExists(fullPath))SysMsg("Template file successfully created, path: \"" + fullPath + "\"");
}
LFile GetFile()
{
	std::string fullPath = FileInfo();
		
	LFile currentFile(fullPath);
	SysMsg("File '" + fullPath + "' found successfully!");

	return currentFile;
}
void FilePublish()
{
	LFile currentFile = GetFile();

	currentFile.Publish(SelectStylesType());
}
void CodeMain()
{
	while(exitCalled){
		steel::Menu({{&Exit,"Exit"},
               {&FilePublish, "Publish Existing File"},
               {&CreateTemplateFile, "Create a template file"}}
               , __FUNCTION__)();
	}
}  