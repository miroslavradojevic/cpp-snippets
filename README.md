## c++ code snippets

### Compile (Windows)
Install [CMake](https://cmake.org/install), [MinGW-x64 c++ compiler](https://code.visualstudio.com/docs/languages/cpp#_example-install-mingwx64) and [OpenCV](https://github.com/huihut/OpenCV-MinGW-Build).

Add the path to your Mingw-w64 bin folder to the Windows PATH environment variable.

CMake should find OpenCV installation, but [installed OpenCV must be compatible with the used compiler (MinGW-x64)](https://stackoverflow.com/questions/70942644/how-to-fix-warning-found-opencv-windows-pack-but-it-has-no-binaries-compatible)
Original [OpenCV installation](https://opencv.org/releases/) is built with MSVC 15, while CMake uses MinGW as compiler. OpenCV (libraries) needs to be built with the same compiler.
Solution is to build OpenCV using MinGW or use [third-party MinGW binary distribution of OpenCV](https://github.com/huihut/OpenCV-MinGW-Build).

- Set a system variable **OpenCV_DIR** to hold the build directory of the OpenCV library ```\OpenCV-MinGW-Build-4.5.2-x64\x64\mingw```
- Add the OpenCV build directory (```\OpenCV-MinGW-Build-4.5.2-x64```), and bin folder (```\OpenCV-MinGW-Build-4.5.2-x64\x64\mingw\bin```)  to the system **PATH**
```
cd REPO_HOME
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --config Debug
```
#### Alternatively, cmake call can use paths
```
cmake -B [build_dir] -S [src_dir]
```

#### Clear build directory
```
rmdir /s /q ".\build\"
```

#### Run 
```
.\Debug\executable.exe
```

### Compile (Linux)
Install prerequisites: 
- Cmake 
```
sudo apt  install cmake
```
- OpenCV
-- Check if OpenCV is available:
```
pkg-config --modversion opencv
```
or
```
dpkg -l | grep libopencv
```
Use [install OpenCV](InstallOpenCv.md) instructions.
- SimpleITK
Use [install SimpleITK](InstallSimpleITK.md) instructions.

Build repo:
```
mkdir build
cd build
cmake ..
make
```

