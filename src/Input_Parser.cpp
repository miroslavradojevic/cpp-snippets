#include "Input_Parser.h"

InputParser::InputParser(int &argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
        this->tokens.push_back(std::string(argv[i]));
};

const std::string &InputParser::getCmdOption(const std::string &option) const
{
    std::vector<std::string>::const_iterator itr;
    itr = std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end())
    {
        return *itr;
    }
    static const std::string empty_string("");
    return empty_string;
}

bool InputParser::cmdOptionExists(const std::string &option) const
{
    return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
}

// int main(int argc, char **argv){
//     InputParser input(argc, argv);
//     if(input.cmdOptionExists("-h")){
//         // Do stuff
//     }
//     const std::string &filename = input.getCmdOption("-f");
//     if (!filename.empty()){
//         // Do interesting things ...
//     }
//     return 0;
// }