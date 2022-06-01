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
