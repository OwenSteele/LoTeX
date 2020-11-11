//
// Created by owen steele on 11/11/2020.
//

#ifndef LOTEX_STEEL_H
#define LOTEX_STEEL_H
struct steel {
    static void Trim(std::string &str) {
        while (str.substr(0, 1) == " ") str = str.substr(1, str.length());
        while (str.substr(str.length() - 1, str.length()) == " ") str = str.substr(0, str.length() - 1);
    }
    static constexpr bool IsWindowsOS() {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        return true;
    //#elif __linux__
    #else return false;
    #endif
    }
    static std::string MsgIn(std::string&& typeDef= nullptr)
    {
        std::string input;
        do{
            std::cout << " << ";
            std::cin >> input;
            if (input == "###") return "";
            else if(!input.empty()) return input;
            else std::cout << "ERR, retry << ";
        }while (true);
    }
    static int MsgIn(int options = -1)
    {
        do{
            try{
                if (int input = std::stoi(MsgIn("")); options != -1)
                {
                    if (input < options && input > -1) return input;
                    else std::cout << "ERROR: Input is outside options range";
                }
                else return input;
            }
            catch(const std::exception& e) {std::cout << "ERROR: You must enter an integer";}
        } while (true);
    }
    static std::function<void()> Menu(std::vector<std::pair<std::function<void()>,std::string>>&& opts, std::string fName){
        std::cout << "\n__" << fName << "() Menu__\n";
        for (int&& i = 0; i < opts.size(); i++) std::cout <<"   " << i << " - " << opts[i].second << std::endl;
        std::cout << "Press a key and press enter";
        int&& x = MsgIn(opts.size());
        opts[x].first;
        return opts[x].first;
    }
    static const std::string GetTime()
    {
        time_t rawTime;
        struct tm * timeinfo;
        char buffer[80];
        time(&rawTime);
        timeinfo = localtime(&rawTime);
        strftime(buffer,sizeof(buffer), "%H:%M:%S", timeinfo);
        std::string str(buffer);
        return str;
    }
};


#endif //LOTEX_STEEL_H
