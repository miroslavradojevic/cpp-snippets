### Install OpenCv from source (Ubuntu 20.04)
1. Install build tools
```
sudo apt install build-essential cmake git pkg-config libgtk-3-dev \
libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
gfortran openexr libatlas-base-dev python3-dev python3-numpy \
libtbb2 libtbb-dev libdc1394-22-dev libopenexr-dev \
libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev
```
2. Clone OpenCV's repositories
```
mkdir ~/opencv_build && cd ~/opencv_build
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git
```
3. Setup OpenCV build
```
cd ~/opencv_build/opencv
mkdir -p build && cd build
```
Set OpenCV build with CMake
```
cmake -D CMAKE_BUILD_TYPE=RELEASE \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D INSTALL_C_EXAMPLES=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D OPENCV_EXTRA_MODULES_PATH=~/opencv_build/opencv_contrib/modules \
-D BUILD_EXAMPLES=ON ..
```
4. Start compilation (use `nproc` to check the number of processor cores)
```
make -j4
```
5. Install OpenCV
```
sudo make install
```
Verify the OpenCV installation:
- C++ binding
```
pkg-config --modversion opencv4
```
- Python binding
```
python3 -c "import cv2; print(cv2.__version__)"
```
### Install OpenCV from source (MacOS)
[Installation in MacOS](https://docs.opencv.org/4.x/d0/db2/tutorial_macos_install.html)
Prerequisites:
- Cmake (>=3.9) https://cmake.org/download/

1. Get OpenCV source code from the Git repository
```
cd ~/<my_source_dir>
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git
```
Select a version tag for each repository using
```
cd ~/<my_source_dir>/opencv
git checkout tags/4.6.0
cd ~/<my_source_dir>/opencv_contrib
git checkout tags/4.6.0
```

2. Build from source code using CMake
Create build directory where build output will be placed
```
mkdir ~/opencv-build
cd ~/opencv-build
```

3. Configure 
```
cmake -DCMAKE_BUILD_TYPE=Release 
      -DOPENCV_EXTRA_MODULES_PATH=~/<my_source_dir>/opencv_contrib/modules 
      -DBUILD_EXAMPLES=ON 
      -DBUILD_DOCS=ON ~/<my_source_dir>/opencv
```

4. Build
```
make -j4
```

5. To use OpenCV in CMake-based projects, specify variable ```OpenCV_DIR``` to ```<path_to_build_or_install_directory>```
```
set(OpenCV_DIR "$ENV{HOME}/opencv-build")
```
and load OpenCV package 
```
find_package(OpenCV)
```
