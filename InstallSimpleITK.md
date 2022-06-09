### Build SimpleITK from source (Ubuntu 18.04)
Precondition to use the SimpleITK’s C++ interface

Instructions:
https://simpleitk.readthedocs.io/en/master/building.html
https://itk.org/Wiki/SimpleITK/GettingStarted

Prerequisites:
- Cmake (>=3.16.3) https://cmake.org/download/
Find and download .sh installer for the release. 
Copy the install bash script (cmake-3.23.2-linux-x86_64.sh) to, for example, /opt/ directory, run and add bin directiory to the $PATH, also possible by adding:
```
export PATH="/opt/cmake-3.23.2-linux-x86_64/bin/${PATH:+:${PATH}}"
```
to  ~/.bashrc

1. Download the latest development version using git
```
cd ~/<my_source_dir>
git clone https://github.com/SimpleITK/SimpleITK.git
```
Select the version (tag)
```
git checkout tags/v2.1.1.2
```
2. Exclude wrappers for other languages 
In ~/<my_source_dir>/SuperBuild/SuperBuild.cmake, before `include(sitkLanguageOptions)`, set variables:
```
set(WRAP_LUA FALSE)
set(WRAP_PYTHON FALSE)
set(WRAP_RUBY FALSE)
set(WRAP_JAVA FALSE)
set(WRAP_TCL FALSE)
set(WRAP_CSHARP FALSE)
set(WRAP_R FALSE)
```

3. Build using SuperBuild
```
mkdir ~/SimpleITK-build
cd ~/SimpleITK-build
cmake -Wno-dev ~/<my_source_dir>/SimpleITK/SuperBuild
```

