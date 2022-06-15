#include <iostream>
#include <iomanip>
#include <ctime>
#include <filesystem>

#include <opencv2/opencv.hpp>

std::string get_timestamp()
{
    // time/date -> string
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;                     // use std::ostringstream to convert the date to a string
    oss << std::put_time(&tm, "%d%m%Y-%H%M%S"); // stream manipulator
    return oss.str();
}

int main()
{
    std::cout << "Float array -> OpenCv image" << std::endl;

    int Nrow = 16;
    int Ncol = 32;

    float image_arr[Nrow][Ncol];
    
    float** image_arr_ = new float*[Nrow];
    for (size_t i = 0; i<Nrow; ++i)
        image_arr_[i] = new float[Ncol];

    for (size_t i = 0; i < Nrow; i++)
    {
        for (size_t j = 0; j < Ncol; j++)
        {
            image_arr[i][j] = (i+1)*(j+1); //(10.0/((Nrow-1)*(Ncol-1))) * i * j;
            image_arr_[i][j] = (i+1)*(j+1);
        }
    }

    cv::Mat image = cv::Mat(Nrow, Ncol, CV_32FC1, image_arr);
    cv::Mat image_ = cv::Mat(Nrow, Ncol, CV_32FC1, image_arr_);

    std::string homedir = getenv("HOME");
    std::filesystem::path image_path(homedir);
    image_path /= std::string("img_array_to_opencv_") + get_timestamp();
    std::filesystem::create_directories(image_path); // https://stackoverflow.com/questions/30937227/create-directory-in-c
    std::filesystem::path image_path_(image_path);
    image_path /= get_timestamp() + ".tiff";
    image_path_ /= get_timestamp() + "_.tiff";


    cv::imwrite(image_path.string(), image);
    std::cout << image_path.c_str() << std::endl;
    std::cout << image_path.extension().c_str() << std::endl;
    cv::imwrite(image_path_.string(), image_);
    std::cout << image_path_.c_str() << std::endl;

    // free memory
    for (size_t i=0; i<Nrow; ++i)
        delete [] image_arr_[i];
    delete [] image_arr_;
}