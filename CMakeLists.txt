cmake_minimum_required(VERSION 3.10.2) # 3.10.2
# enable c++17 https://stackoverflow.com/questions/45688522/how-to-enable-c17-in-cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_CXX_COMPILER /usr/bin/gcc-8)

project(CppSnippets VERSION 1.0.0)

set(SimpleITK_DIR "$ENV{HOME}/source/SimpleITK-build/SimpleITK-build")
message(STATUS "SimpleITK_DIR: ${SimpleITK_DIR}")

set(ITK_DIR "$ENV{HOME}/source/SimpleITK-build/ITK-build")
message(STATUS "ITK_DIR: ${ITK_DIR}")

find_package(SimpleITK)
message(STATUS "SimpleITK_FOUND=${SimpleITK_FOUND}")
if(SimpleITK_FOUND)
    add_executable(read_raw src/image/read_raw.cpp src/Input_Parser.cpp)
    target_include_directories(read_raw PRIVATE ./src)
    target_link_libraries(read_raw ${SimpleITK_LIBRARIES})
else()
    message(STATUS "SimpleITK not found!")
endif()

find_package(OpenCV)

if (OpenCV_FOUND)
    message(STATUS "OpenCV version: " ${OpenCV_VERSION})
    include_directories(${OpenCV_INCLUDE_DIRS})
    
    add_executable(img_array_to_opencv src/img_array_to_opencv.cpp)
    # https://stackoverflow.com/questions/33149878/experimentalfilesystem-linker-error
    target_link_libraries(img_array_to_opencv ${OpenCV_LIBS} stdc++fs)
    
    add_executable(read_img_opencv src/read_img_opencv.cpp)
    target_link_libraries(read_img_opencv ${OpenCV_LIBS})
else()
    message(STATUS "OpenCV not found!")
endif()

add_executable(general src/general.cpp)
add_executable(timing src/timing.cpp)
add_executable(formatting src/formatting.cpp)
add_executable(write_file src/writeFile.cpp)
add_executable(replicate_txt_file_with_line_nums src/replicateTxtFileWithLineNums.cpp)
add_executable(pos src/pos.cpp)
add_executable(psetorient src/psetorient.cpp)