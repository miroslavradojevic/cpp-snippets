#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

int main() {
    std::cout << "Input text tile path:\t";
    std::string filePath{};
    std::cin >> filePath;

    std::ifstream inFile{filePath};
    if (!inFile.good()){
        std::cerr << "File at " << filePath << " does not exist" << std::endl;
        return 1;
    }

    if (!inFile.is_open()) {
        std::cerr << "Error opening input file" << std::endl;
        return 1;
    }

    size_t idx = filePath.find_last_of(".");
    std::string filePathOut = filePath.substr(0, idx); 
    filePathOut = filePathOut.append("_out.txt"); 

    std::cout << "output file path:\t" << filePathOut << std::endl;
    std::ofstream outFile{filePathOut};

    std::string line {};
    int line_number {0};
    while (getline(inFile, line)) {
        if (line == "") {
            outFile << std::endl;
        }
        else {
            ++line_number;
            outFile << std::setw(7) << std::left << line_number << line << std::endl;
        }
    }

    std::cout << "\nCopy complete" << std::endl;

    inFile.close();
    outFile.close();

    return 0;
}