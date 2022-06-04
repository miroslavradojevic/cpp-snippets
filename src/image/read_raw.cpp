#include <iostream>
#include <filesystem>
#include <SimpleITK.h>
#include <sitkImageFileReader.h>
#include "Input_Parser.h"

namespace sitk = itk::simple;
// namespace fs = std::filesystem;

int main(int argc, char **argv)
{

    if (__cplusplus == 201703L) std::cout << "C++17\n";
    else if (__cplusplus == 201402L) std::cout << "C++14\n";
    else if (__cplusplus == 201103L) std::cout << "C++11\n";
    else if (__cplusplus == 199711L) std::cout << "C++98\n";
    else std::cout << "pre-standard C++\n";

    //std::cout << "read raw image" << std::endl;
    InputParser ip(argc, argv);
    if (ip.cmdOptionExists("-f"))
    {
        const std::string &filename = ip.getCmdOption("-f");
        if (!filename.empty())
        {
            std::cout << "opening file: " << filename << std::endl;
            // fs::path fpath{filename};

            // std::cout << fpath.c_str() << std::endl;
            // std::cout << fpath.parent_path() << std::endl;

            // TODO: check if it exists

            // read raw file from path
            sitk::ImageFileReader reader;
            std::vector<std::string> image_ios = reader.GetRegisteredImageIOs();
            std::cout << image_ios.size() <<  "ImageIOs:" << std::endl;
            for (auto & iio : image_ios) std::cout << iio << " ";
            std::cout << std::endl;

            // Read the image file
            reader.SetImageIO ("");// "PNGImageIO" 
            reader.SetFileName (filename);

            try
            {
                sitk::Image image = reader.Execute();
                std::cout << "Read image: " << filename << std::endl;

                std::vector<unsigned int> size = image.GetSize();
                std::cout << "Image size: " << size[0] << " " << size[1] << std::endl;
            }
            catch (std::exception &e)
            {
                std::cout << "Read failed: " << e.what() << std::endl;
            }

            sitk::Image im = sitk::ReadImage("/home/miro/ev_battery_ct/img/image.mhd");
            std::cout << im.GetWidth() << "," << im.GetHeight() << "," << im.GetDepth() << "," << im.GetNumberOfPixels() << std::endl; 
            // std::vector<unsigned int> idx{ 0, 0, 0 };
            // std::vector<float> im_arr = im.GetPixelAsVectorFloat32(idx);
            float* im_arr = im.GetBufferAsFloat();
            std::cout << im_arr[0] << "|" << im_arr[1] << "|" << im_arr[3] << std::endl;
        }
        return 0;
    }
    else {
        std::cerr << "Argument -f missing: raw image path" << std::endl;
        return 1;
    }
    
}