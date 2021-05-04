#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

int main()
{
    std::string fname{};
    std::cout << "Enter file name:";
    std::cin >> fname;

    std::ofstream outFile{fname};
    if (!outFile)
    {
        std::cerr << "Error creating file" << std::endl;
    }

    std::vector<float> arr {1, 2, 3, 4, 5};

    outFile << std::setprecision(2) << std::fixed;

    bool first = true;
    for (auto i = arr.begin(); i != arr.end(); ++i)
    {
        outFile << std::setw(4) << std::left << ((first) ? "" : ",") << *i;
        first = false;
    }
    outFile << std::endl;

    outFile.close();
}