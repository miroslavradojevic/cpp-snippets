#include <iostream>
#include <iomanip>
#include <opencv2/opencv.hpp>

int main()
{
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
            image_arr[i][j] = (i+1)*(j+1);
            image_arr_[i][j] = (i+1)*(j+1);
        }
    }

    cv::Mat image = cv::Mat(Nrow, Ncol, CV_32FC1, image_arr);
    cv::Mat image_ = cv::Mat(Nrow, Ncol, CV_32FC1, image_arr_);

    cv::imwrite("img_alloc_static.tiff", image);
    cv::imwrite("img_alloc_dynamic.tiff", image_);

    // free memory
    for (size_t i=0; i<Nrow; ++i)
        delete [] image_arr_[i];
    delete [] image_arr_;
}