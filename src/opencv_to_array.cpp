#include <opencv2/opencv.hpp>
#include <filesystem>

int main(int argc, char **argv) {

    // std::cout << "Mat to array" << std::endl;
    // https://stackoverflow.com/questions/26681713/convert-mat-to-array-vector-in-opencv

    if (argc!=2){
        std::cout << "Convert Mat to Array/Vector in OpenCV\nUsage: ./opencv_to_array file_path" << std::endl;
        return 1;
    }

    std::filesystem::path fpath{argv[1]};
    
}