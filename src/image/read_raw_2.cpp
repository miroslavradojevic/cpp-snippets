#include <iostream>
#include <fstream>
#include <limits.h>
#include <opencv2/opencv.hpp>
#include "Input_Parser.h"

std::vector<float> read_raw(const std::string &fpath, int width, int height, int length){
    // open the file
    std::streampos fsize;
    std::ifstream file(fpath.c_str(), std::ios::binary);
    
    // get its size
    file.seekg(0, std::ios::end);
    fsize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // read the data
    std::vector<float> fdata(fsize);
    file.read((char*) &fdata[0], fsize);

    // retain first width*height*length elements
    int sz = width * height * length;
    if (sz < fsize) 
        fdata.erase(fdata.begin()+sz, fdata.end());
    
    return fdata;
}

std::vector<cv::Mat> get_layers(std::vector<float> img, int w, int h, int l, int c = 0, float img_min=0.0, float img_max=0.05) {
    std::vector<cv::Mat> layers;
    float img2d[h][w];
    layers.push_back(cv::Mat(h, w, CV_32F, img2d));
    return layers;
}

cv::Mat get_z_mean(std::vector<float> img, int w, int h, int l, int c=0, float img_min=0.0, float img_max=0.05) {
    // store average along z-dimension into an array
    float img_arr[h*w]= {0};

    float pixval = 0;
    for (size_t z = 0; z < l; ++z)
    {
        for (size_t row = 0; row < h; ++row)
        {
            for (size_t col = 0; col < w; ++col)
            {
                pixval = img[c + (z * (w * h) + col + w * row)];
                // crop
                if (pixval > img_max) pixval = img_max;
                if (pixval < img_min) pixval = img_min;
                // compute iterative mean for each pixel along z-axis
                // https://www.heikohoffmann.de/htmlthesis/node134.html
                if (z == 0)
                    img_arr[col + w * row] = pixval;
                else
                    img_arr[col + w * row] = ((float)z / (z + 1)) * img_arr[col + w * row] + pixval / (z + 1);

            }
        }
    }

    cv::Mat img_avg = cv::Mat(h, w, CV_32F, img_arr);
    return img_avg.clone();
}

void detect_battery_rectangle(const cv::Mat& img, int &x1_, int &x2_, int &y1_, int &y2_){

    cv::Mat img_integral;
    cv::integral(img, img_integral);
    
    int img_height = img.rows;
    int img_width = img.cols;

    int w_min = (int)round(img_width * 0.10);
    int w_max = (int)round(img_width * 0.95);

    int h_min = (int)round(img_height * 0.10);
    int h_max = (int)round(img_height * 0.95);

    int x1_min = (int)round(img_width * 0.05);

    std::cout << w_min << ", " << w_max << std::endl;

    x1_ = -1;
    x2_ = -1;


    y1_ = -1;
    y2_ = -1;
}

void get_cell_locations(const cv::Mat& img, int nr_cells=18, int gap_width=5) {
    std::vector<std::vector<float>> cell_centroids;

}

int main(int argc, char **argv)
{
    if (sizeof(float) * CHAR_BIT != 32)
    {
        std::cerr << "float must be 32 bit on this machine" << std::endl;
        return 1;
    }

    InputParser ip(argc, argv);

    if (ip.cmdOptionExists("-f") && 
        ip.cmdOptionExists("-w") && 
        ip.cmdOptionExists("-h") && 
        ip.cmdOptionExists("-l"))
    {
        const std::string &filename = ip.getCmdOption("-f");
        const int w = stoi(ip.getCmdOption("-w"));
        const int h = stoi(ip.getCmdOption("-h"));
        const int l = stoi(ip.getCmdOption("-l"));

        if (!filename.empty())
        {

            std::vector<float> img = read_raw(filename, w, h, l);

            cv::Mat img_avg = get_z_mean(img, w, h, l); //cv::Mat(h, w, CV_32F);

            // int nr_cells = 18;
            // int gap_width = 7;
            int x1, x2, y1, y2;
            detect_battery_rectangle(img_avg, x1, x2, y1, y2);

            std::cout << x1 << ", " << x2 << ", " << y1 << ", " << y2 << std::endl;

            // get_cell_locations(, );

            // save
            std::string fpath_out=filename.substr(0,filename.find_last_of('.'))+"_meanZ.tif";
            std::vector<int> compression_params;
            compression_params.push_back(cv::IMWRITE_TIFF_COMPRESSION);
            compression_params.push_back(1); // None 
            cv::imwrite(fpath_out.c_str(), img_avg, compression_params);
            std::cout << fpath_out.c_str() << std::endl;

        }
        return 0;
    }
    else {
        std::cerr << "Argument missing:\nUsage:\nread_raw_2 -w W -h H -l L -f path" << std::endl;
        return 1;
    }
}