//#include <sitkImageFileReader.h>
#include <SimpleITK.h>

int main() {
    itk::simple::ImageFileReader reader;
    std::vector<std::string> image_ios = reader.GetRegisteredImageIOs();
    std::cout << image_ios.size() <<  "ImageIOs:" << std::endl;
    for (auto & iio : image_ios) std::cout << iio << " ";
    std::cout << std::endl;
    return 0;
}