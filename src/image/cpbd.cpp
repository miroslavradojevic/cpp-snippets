#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

cv::Mat simple_thinning(cv::Mat strength){
    std::cout << "simple_thinning" << std::endl;
    // use opencv functions cv::compare, cv::warpAffine

}

std::string openCVType2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
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
    std::cout << fpath.parent_path() << std::endl;
    std::cout << fpath.filename() << std::endl;

    if (!std::filesystem::exists(fpath)) {
        std::cout << fpath.c_str() << " does not exist" << std::endl;
        return 1;
    }

    cv::Mat image = imread(argv[1], cv::IMREAD_GRAYSCALE);
    double minVal;     
    double maxVal;          
    cv::minMaxLoc(image, &minVal, &maxVal);
    std::cout << minVal << " -- " << maxVal << std::endl;

    // Canny 
    cv::Mat canny;
    int lowThreshold = 0.1 * 255; // https://scikit-image.org/docs/stable/api/skimage.feature.html#skimage.feature.canny
    const int kernel_size = 7;
    const int highThreshold = 1.0 * 255;
    // std::cout << "lowThreshold = " << lowThreshold << std::endl;
    // std::cout << "highThreshold = " << highThreshold << std::endl;

    // blur filter first?
    cv::Canny(image, canny, lowThreshold, highThreshold, kernel_size);

    //check type
    std::string ty =  openCVType2str(canny.type());
    std::cout << ty << std::endl;

    // save it
    cv::imwrite(fpath.replace_extension("canny.png"), canny);
    std::cout << "canny image type = " << canny.type() << std::endl; // CV_MAKETYPE(canny.type(), 1)
    
    cv::Mat sobel_x;
    int ddepth = CV_16S;
    int ksize = 3;
    int scale = 1;
    int delta = 0;
    cv::Sobel(image, sobel_x, ddepth, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);

    cv::imwrite(fpath.replace_extension("sobelx.png"), sobel_x);
    std::cout << sobel_x.type() << std::endl;

    //    

    return 0;
}