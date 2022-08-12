#include <iostream>
#include <fstream>
#include <limits.h>
#include <opencv2/opencv.hpp>
#include "Input_Parser.h"

template <typename S>
std::ostream& operator<<(std::ostream& os, const std::vector<S>& v)
{
    // Printing all the elements using <<
    for (auto element : v) {
        os << element << " ";
    }
    return os;
}

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
    cv::integral(img, img_integral, CV_32F);

    int img_integral_height = img_integral.rows;
    int img_integral_width = img_integral.cols;

    int img_height = img.rows;
    int img_width = img.cols;
    
    // float t1 = img_integral.at<float>(img_integral_height-1, img_integral_width-1);
    // float t2 = img.at<float>(img_height-1, img_width-1);
    // std::cout << t1 << " " << t2 << " " << img_integral_height << " " << img_integral_width << std::endl;

    int w_min = (int)round(img_width * 0.10);
    int w_max = (int)round(img_width * 0.95);

    int h_min = (int)round(img_height * 0.10);
    int h_max = (int)round(img_height * 0.95);

    int x1_min = (int)round(img_width * 0.05);
    int x1_max = (int)round(img_width * 0.5);
    int x2_min = (int)round(img_width * 0.5);
    int x2_max = (int)round(img_width * 0.95);

    int y1_min = (int)round(img_height * 0.05);
    int y1_max = (int)round(img_height * 0.5);
    int y2_min = (int)round(img_height * 0.5);
    int y2_max = (int)round(img_height * 0.95);

    int step =1;

    float score_max = -std::numeric_limits<float>::infinity();

    std::cout << w_min << ", " << w_max << std::endl;

    x1_ = -1;
    x2_ = -1;
    // loop through the sampled rectangles
    for (size_t x1 = x1_min; x1 < x2_max; x1+=step)
    {
        for (size_t x2 = x2_min; x2 < x2_max; x2+=step)
        {
            if (x2>=x1 && x2-x1 >= w_min && x2-x1 <= w_max) {
                float s1 = (img_integral.at<float>(img_integral_height-1, x1) + 
                            img_integral.at<float>(0, 0) -
                            img_integral.at<float>(0, x1) - 
                            img_integral.at<float>(img_integral_height-1, 0))/(float)(x1*img_height);
                
                float s2 = (img_integral.at<float>(img_integral_height-1, x2) + 
                            img_integral.at<float>(0, x1) -
                            img_integral.at<float>(0, x2) - 
                            img_integral.at<float>(img_integral_height-1, x1))/(float)((x2-x1)*img_height);

                float s3 = (img_integral.at<float>(img_integral_height-1, img_integral_width-1) + 
                            img_integral.at<float>(0, x2) -
                            img_integral.at<float>(0, img_integral_width-1) - 
                            img_integral.at<float>(img_integral_height-1, x2))/(float)((img_width-x2)*img_height);
                
                float score = s2 - 0.5 * (s1 + s3);
                
                if (score > score_max) {
                    score_max = score;
                    x1_ = x1;
                    x2_ = x2;
                }
            }
        }
    }
    
    score_max = -std::numeric_limits<float>::infinity();
    y1_ = -1;
    y2_ = -1;

    for (size_t y1 = y1_min; y1 < y1_max; y1+=step)
    {
        for (size_t y2 = y2_min; y2 < y2_max; y2+=step)
        {
            if (y2>=y1 && y2-y1 >= h_min && y2-y1 <= h_max) {
                float s1 = (img_integral.at<float>(y1, img_integral_width-1) + 
                            img_integral.at<float>(0, 0) -
                            img_integral.at<float>(y1, 0) - 
                            img_integral.at<float>(0, img_integral_width-1))/(float)(y1*img_width);
                
                float s2 = (img_integral.at<float>(y2, img_integral_width-1) + 
                            img_integral.at<float>(y1, 0) -
                            img_integral.at<float>(y2, 0) - 
                            img_integral.at<float>(y1, img_integral_width-1))/(float)((y2-y1)*img_width);
                
                float s3 = (img_integral.at<float>(img_integral_height-1, img_integral_width-1) + 
                            img_integral.at<float>(y2, 0) -
                            img_integral.at<float>(img_integral_height-1, 0) - 
                            img_integral.at<float>(y2, img_integral_width-1))/(float)((img_height-y2)*img_width);
                
                float score = s2 - 0.5 * (s1 + s3);
                
                if (score > score_max) {
                    score_max = score;
                    y1_ = y1;
                    y2_ = y2;
                }
            }
        }
        
    }
    
}

std::vector<int> detect_cells(cv::Mat img, int gap_width=7, int nr_cells=18) {
    
    std::vector<int> x_out;

    if ((gap_width % 2) == 0 || (nr_cells % 2) == 1) {
        std::cerr << "Gap width (" << gap_width << ") must be odd number and number of cells (" << nr_cells << ") must be even number" << std::endl;
        return x_out;
    }

    cv::Mat img_integral;
    cv::integral(img, img_integral, CV_32F);

    int img_integral_height = img_integral.rows;
    int img_integral_width = img_integral.cols;

    int img_height = img.rows;
    int img_width = img.cols;

    int step = 1;

    int nhood = (int)(round(img_width/nr_cells)*0.25);


    // central division
    float score_optim = -std::numeric_limits<float>::infinity();
    int x_optim = -1;

    // std::cout << "nhood = " << nhood << " | " << step << score_optim << std::endl;
    for (int dx = -nhood; dx < nhood; dx+=step)
    {
        // std::cout << "dx=" << dx << std::endl;
        int x0 = img_width/(int)2 + dx;
        int x1 = x0 - gap_width/(int)2;
        int x2 = x0 + gap_width/(int)2;
        int x11 = x1 - gap_width;
        int x22 = x2 + gap_width;

        float s0 = (img_integral.at<float>(img_integral_height-1, x2) + 
                    img_integral.at<float>(0, x1) -
                    img_integral.at<float>(0, x2) - 
                    img_integral.at<float>(img_integral_height-1, x1))/(float)(gap_width*img_height);

        float s1 = (img_integral.at<float>(img_integral_height-1, x1) + 
                    img_integral.at<float>(0, x11) -
                    img_integral.at<float>(0, x1) - 
                    img_integral.at<float>(img_integral_height-1, x11))/(float)(gap_width*img_height);
        
        float s2 = (img_integral.at<float>(img_integral_height-1, x22) + 
                    img_integral.at<float>(0, x2) -
                    img_integral.at<float>(0, x22) - 
                    img_integral.at<float>(img_integral_height-1, x2))/(float)(gap_width*img_height);
        
        float score = 0.5*(s1+s2)-s0;

        // std::cout << score << std::endl;
        
        if (score > score_optim){
            score_optim = score;
            x_optim = x0;
        }
    }

    x_out.push_back(x_optim);

    int x_left = x_optim, x_right = x_optim;

    // expand from center towards right
    for (int i = 0; i < nr_cells/(int)2; i++)
    {
        score_optim = -std::numeric_limits<float>::infinity();
        x_optim = -1;
        for (int dx = -nhood; dx < nhood; dx+=step)
        {
            int x0 = x_right + (int)round(img_width/nr_cells) + dx;
            int x1 = x0 - gap_width/(int)2;
            int x2 = x0 + gap_width/(int)2;
            int x11 = x1 - gap_width;
            int x22 = x2 + gap_width;

            if (x22 <= img_width) {
                float s0 = (img_integral.at<float>(img_integral_height-1, x2) + 
                            img_integral.at<float>(0, x1) -
                            img_integral.at<float>(0, x2) - 
                            img_integral.at<float>(img_integral_height-1, x1))/float(gap_width*img_height);
                
                float s1 = (img_integral.at<float>(img_integral_height-1, x1) + 
                            img_integral.at<float>(0, x11) -
                            img_integral.at<float>(0, x1) - 
                            img_integral.at<float>(img_integral_height-1, x11))/float(gap_width*img_height);
                
                float s2 = (img_integral.at<float>(img_integral_height-1, x22) + 
                            img_integral.at<float>(0, x2) -
                            img_integral.at<float>(0, x22) - 
                            img_integral.at<float>(img_integral_height-1, x2)) /float(gap_width*img_height);
                
                float score = 0.5*(s1+s2)-s0;

                if (score > score_optim){
                    score_optim = score;
                    x_optim = x0;
                }
                    
            }
        }

        if (x_optim != -1)
        {
            x_out.push_back(x_optim);
            x_right = x_optim;
        }
        else
        {
            break; // stop further with for-loop first time score was not found
        }
    }

    // expand from center towards left
    for (int i = 0; i < nr_cells/(int)2; i++){
        score_optim = -std::numeric_limits<float>::infinity();
        x_optim = -1;
        for (int dx = -nhood; dx < nhood; dx+=step){
            int x0 = x_left - (int)round(img_width/nr_cells) + dx;
            int x1 = x0 - gap_width/(int)2;
            int x2 = x0 + gap_width/(int)2;
            int x11 = x1 - gap_width;
            int x22 = x2 + gap_width;

            if (x11 >= 0) {
                float s0 = (img_integral.at<float>(img_integral_height-1, x2) + 
                            img_integral.at<float>(0, x1) -
                            img_integral.at<float>(0, x2) - 
                            img_integral.at<float>(img_integral_height-1, x1))/float(gap_width*img_height);
                
                float s1 = (img_integral.at<float>(img_integral_height-1, x1) + 
                            img_integral.at<float>(0, x11) -
                            img_integral.at<float>(0, x1) - 
                            img_integral.at<float>(img_integral_height-1, x11))/float(gap_width*img_height);
                
                float s2 = (img_integral.at<float>(img_integral_height-1, x22) + 
                            img_integral.at<float>(0, x2) -
                            img_integral.at<float>(0, x22) - 
                            img_integral.at<float>(img_integral_height-1, x2)) /float(gap_width*img_height);
                
                float score = 0.5*(s1+s2)-s0;

                if (score > score_optim){
                    score_optim = score;
                    x_optim = x0;
                }
            }
        }
        if (x_optim != -1)
        {
            x_out.push_back(x_optim);
            x_left = x_optim;
        }
        else
        {
            break; // stop further with for-loop first time score was not found
        }
    }

    sort(x_out.begin(), x_out.end());

    return x_out;
        
}

void get_cell_locations(const cv::Mat& img, std::vector<std::vector<float>> &cell_centroids, std::vector<float> &cell_widths, int nr_cells=18, int gap_width=5) {

    int x1, x2, y1, y2;
    detect_battery_rectangle(img, x1, x2, y1, y2);
    std::cout << x1 << ", " << x2 << ", " << y1 << ", " << y2 << std::endl;

    cv::Mat img_crop = img(cv::Range(y1,y2), cv::Range(x1,x2));

    std::vector<int> x_vert = detect_cells(img_crop, gap_width, nr_cells);
    std::cout << "x_vert=[" << x_vert << "]" << std::endl;

    // extract cell centroids
    for (auto i : cell_centroids)
        i.clear();

    cell_centroids.clear();

    cell_widths.clear();
    
    for (int i = 1; i < x_vert.size(); i++)
    {
        int x_r = x_vert[i];
        int x_l = x_vert[i-1];
        std::vector<float> cc{ x1+0.5f*(x_l+x_r), 0.5f*(y1+y2) };
        cell_centroids.push_back(cc);
        cell_widths.push_back(x_r - x_l);
    }
    


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

            cv::Mat img_avg = get_z_mean(img, w, h, l);

            int nr_cells = 18;
            int gap_width = 5;

            std::vector<std::vector<float>> cell_centroids;
            std::vector<float> cell_widths;

            get_cell_locations(img_avg, cell_centroids, cell_widths, nr_cells, gap_width);

            std::cout << "cell centroids:" << std::endl;
            for (auto cc : cell_centroids) 
                std::cout << "(" << cc[0] << ", " << cc[1] << "), ";
            std::cout << std::endl;

            std::cout << "cell widths: [" << cell_widths << "]" << std::endl;

            // save img_avg
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