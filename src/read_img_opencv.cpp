#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;

int main()
{
    // read image from the given path
    std::string image_path;
    std::cout << "Image path: ";
    std::cin >> image_path;

    Mat img = imread(image_path, IMREAD_COLOR);

    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    
    imshow("Display window", img);
    
    int k = waitKey(0); // Wait for a keystroke in the window
    if(k == 'q')
    {
        imwrite("output.png", img);
    }
    return 0;
}