#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <SimpleITK.h>
#include <opencv2/opencv.hpp>
#include "Input_Parser.h"

namespace sitk = itk::simple;

std::vector<cv::Mat> read_raw(std::string raw_img_path, int width, int height, int depth, bool is_big_endian, std::string type){
    std::filesystem::path fpath = std::filesystem::canonical(raw_img_path); // get absolute path
    std::vector<cv::Mat> out;
    return out;
};

int main(int argc, char **argv)
{
    InputParser ip(argc, argv);
    if (ip.cmdOptionExists("-f"))
    {
        const std::string &filename = ip.getCmdOption("-f");
        if (!filename.empty())
        {
            std::filesystem::path fpath{filename};

            if (std::filesystem::exists(fpath)) {
                
                std::cout << "extension: " << fpath.extension().c_str() << std::endl;
                
                if (std::strcmp(fpath.extension().c_str(), ".raw")==0) {

                    if (false) {
                        // read raw file from path
                        sitk::ImageFileReader reader;
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
                            std::cerr << "Read failed: " << e.what() << std::endl;
                            return 1;
                        }
                    }
                    else {
                        // create .mhd file with read parameters
                        std::cout << "Reading .raw file..." << std::endl;
                        std::cout << "Parent:\n" << fpath.parent_path().c_str() << std::endl;
                        std::cout << "Filename:\n" << fpath.filename().c_str() << std::endl;
                        fpath = std::filesystem::canonical(fpath.c_str()); // get absolute path
                        
                        std::filesystem::path fpath_mhd{fpath};
                        fpath_mhd.replace_extension(".mhd");
                        
                        // form new path in the same parent directory, under the same name and .mhd extension
                        std::cout << "MHD:\n" << fpath_mhd.c_str() << std::endl;
                        std::cout << "Original:\n" << fpath.c_str() << std::endl;
                        
                        std::ofstream outfile{fpath_mhd.c_str()};
                        if (!outfile) {
                            std::cerr << "Error opening output file" << std::endl;
                            return 1;
                        }
                        outfile << "ObjectType = Image" << std::endl;
                        outfile << "NDims = 3" << std::endl;
                        outfile << "DimSize = 1800 500 80" << std::endl;
                        outfile << "ElementSpacing = 1 1 1" << std::endl;
                        outfile << "Offset = 0 0 0" << std::endl;
                        outfile << "TransformMatrix = 1 0 0 0 1 0 0 0 1" << std::endl;
                        outfile << "ElementType = MET_FLOAT" << std::endl;
                        outfile << "BinaryData = True" << std::endl;
                        outfile << "BinaryDataByteOrderMSB = False" << std::endl;
                        outfile << "ElementDataFile = " << fpath.c_str() << std::endl; // must be the last entry
                        outfile.close();
                        
                        // std::cout << "Absolute:\n" << fpath.is_absolute() << std::endl;
                        // std::cout << "Has root path:\n" << fpath.has_root_path() << std::endl;
                        
                        sitk::Image im;
                        try{
                            im = sitk::ReadImage(fpath_mhd.c_str());
                        }
                        catch (std::exception &e) {
                            std::cerr << "Read failed: " << e.what() << std::endl;
                            return 1;
                        }
                        
                        std::cout << "Read success" << std::endl;
                        std::cout << "width:\t" << im.GetWidth() << std::endl;
                        std::cout << "height:\t" << im.GetHeight() << std::endl;
                        std::cout << "depth:\t" << im.GetDepth() << std::endl;
                        std::cout << "pixels:\t" << im.GetNumberOfPixels() << std::endl;
                        std::cout << "nrComponents:\t" << im.GetNumberOfComponentsPerPixel() << std::endl;
                        
                        remove(fpath_mhd.c_str()); // delete created file
                        
                        float* im_buf = im.GetBufferAsFloat();
                        
                        float im_arr2d[im.GetHeight()][im.GetWidth()]; // static alloc necessary, dynamic alloc (throws fault)
                        
                        int z = 21;
                        int c = 0;
                        // store average along z-dimension into a 2D array, compute iteratively
                        float pixval = 0;
                        for (size_t z = 0; z < im.GetDepth(); ++z)
                        {
                            for (size_t row = 0; row < im.GetHeight(); ++row) {
                                for (size_t col = 0; col < im.GetWidth(); ++col) {
                                    pixval = im_buf[c + im.GetNumberOfComponentsPerPixel()*(z*(im.GetWidth()*im.GetHeight()) + col + im.GetWidth()*row)];
                                    // crop
                                    if (pixval>0.05) pixval = 0.05;
                                    if (pixval<0.00) pixval = 0.00;
                                    // compute iterative mean for each pixel along z-axis
                                    // https://www.heikohoffmann.de/htmlthesis/node134.html
                                    if (z == 0) 
                                        im_arr2d[row][col] = pixval;
                                    else 
                                        im_arr2d[row][col] = ((float)z/(z+1)) * im_arr2d[row][col] + pixval/(z+1);
                                }
                            }
                        }
                        
                        std::filesystem::path fpath_out{fpath};
                        fpath_out.replace_extension(".tiff");
                        cv::Mat img = cv::Mat(im.GetHeight(), im.GetWidth(), CV_32F, im_arr2d);

                        // write to disk
                        std::vector<int> compression_params;
                        compression_params.push_back(cv::IMWRITE_TIFF_COMPRESSION);
                        compression_params.push_back(1);
                        cv::imwrite(fpath_out.c_str(), img, compression_params);
                        std::cout << fpath_out.c_str() << std::endl;
                        
                    }
                }   
                else {
                    std::cerr << "File " << fpath.c_str() << " does not have .raw extension" << std::endl;
                }
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