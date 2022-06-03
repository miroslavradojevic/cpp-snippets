### Build SimpleITK from source (Ubuntu 18.04)
Needed to use the SimpleITK’s C++ interface

Instructions:
https://simpleitk.readthedocs.io/en/master/building.html
https://itk.org/Wiki/SimpleITK/GettingStarted

Prerequisites:
- Cmake (>=3.16.3) https://cmake.org/download/
Find and download .sh installer for the release. 
Copy the install bash script (cmake-3.23.2-linux-x86_64.sh) to, for example, /opt/ directory, run and add bin directiory to the $PATH, possible to do by adding line  ~/.bashrc
```
export PATH="/opt/cmake-3.23.2-linux-x86_64/bin/${PATH:+:${PATH}}"
```

1. Download the latest development version using git
```
git clone https://github.com/SimpleITK/SimpleITK.git
```
2. Build using SuperBuild
