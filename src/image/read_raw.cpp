#include <iostream>
#include <filesystem>
#include <SimpleITK.h>
#include <sitkImageFileReader.h>
#include "Input_Parser.h"

namespace sitk = itk::simple;
// namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    InputParser ip(argc, argv);
    if (ip.cmdOptionExists("-f"))
    {
        const std::string &filename = ip.getCmdOption("-f");
        if (!filename.empty())
        {
            std::filesystem::path fpath{filename};

//             std::cout << "parent=" << fpath.parent_path().c_str() << std::endl;

            if (std::filesystem::exists(fpath)) {
                
                std::cout << fpath.extension() << std::endl;
                
                if (std::strcmp(fpath.extension().c_str(), ".raw")==0) {
                    // read raw file from path
                    sitk::ImageFileReader reader;

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
                }
                else {
                    
                }
                    
                // alternative
                sitk::Image im = sitk::ReadImage("/home/miro/ev_battery_ct/img/image.mhd");
                std::cout << im.GetWidth() << "," << im.GetHeight() << "," << im.GetDepth() << "," << im.GetNumberOfPixels() << std::endl; 
                // std::vector<unsigned int> idx{ 0, 0, 0 };
                // std::vector<float> im_arr = im.GetPixelAsVectorFloat32(idx);
                float* im_arr = im.GetBufferAsFloat();
                std::cout << im_arr[0] << "|" << im_arr[1] << "|" << im_arr[3] << std::endl;
            }
            else {
                std::cerr << "File " << fpath.c_str() << " does not exist" << std::endl;
            }
        }
        return 0;
    }
    else {
        std::cerr << "Argument -f missing: raw image path" << std::endl;
        return 1;
    }
    
}