#ifndef _INPUT_PARSER_H_
#define _INPUT_PARSER_H_

class InputParser{
    private:
    std::vector <std::string> tokens;
    public:
    InputParser(int &argc, char **argv);
    const std::string& getCmdOption(const std::string &option) const;
    bool cmdOptionExists(const std::string &option) const;
};

#endif // _INPUT_PARSER_H_