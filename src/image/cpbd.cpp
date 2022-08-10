#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

void sobel1(){
    std::cout << "sobel1" << std::endl;
}

int main(int argc, char **argv)
{
    // std::cout << "cpbd" << std::endl;
//    for (int i = 0; i < argc; ++i)
//        std::cout << argv[i] << std::endl;
  
    if (argc!=2){
        std::cout << "Path to image needs to be given as first argument" << std::endl;
        return 1;
    }
    
    std::filesystem::path fpath{argv[1]};

    if (!std::filesystem::exists(fpath)) {
        std::cout << fpath.c_str() << " does not exist" << std::endl;
        return 1;
    }

    cv::Mat image = imread(argv[1], cv::IMREAD_GRAYSCALE);

    cv::Mat detected_edges;
    int lowThreshold = 0;
    const int max_lowThreshold = 100;
    const int ratio = 3;
    const int kernel_size = 3;

    // do blur first?
    cv::Canny(image, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

    // show
    // cv::namedWindow("Edge", cv::WINDOW_AUTOSIZE);
    // cv::imshow("image", detected_edges);
    // cv::waitKey(30);

    cv::imwrite("/Users/miroslav/source/cpp-snippets/data/Recon14_1800x500x80_02_028_canny.png", detected_edges);
    std::cout << detected_edges.type() << std::endl; // CV_MAKETYPE(detected_edges.type(), 1)
    
    cv::Mat grad_x;
    int ddepth = CV_16S;
    int ksize = 3;
    int scale = 1;
    int delta = 0;
    cv::Sobel(image, grad_x, ddepth, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);

    cv::imwrite("/Users/miroslav/source/cpp-snippets/data/Recon14_1800x500x80_02_028_sobelx.png", grad_x);
    std::cout << grad_x.type() << std::endl;

    

    return 0;
}