#include <opencv2/opencv.hpp>
#include <filesystem>

int main(int argc, char **argv) {

    // std::cout << "Mat to array" << std::endl;

    if (argc!=2){
        std::cout << "Convert Mat to Array/Vector in OpenCV\nUsage: ./opencv_to_array file_path" << std::endl;
        return 1;
    }

    std::filesystem::path fpath{argv[1]};
    
}