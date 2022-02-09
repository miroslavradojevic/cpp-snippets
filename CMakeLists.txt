cmake_minimum_required(VERSION 3.10.2)
# enable c++17 https://stackoverflow.com/questions/45688522/how-to-enable-c17-in-cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(CppSnippets VERSION 1.0.0)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

message(${CMAKE_CXX_STANDARD})

add_executable(general src/general.cpp)
add_executable(timing src/timing.cpp)
add_executable(formatting src/formatting.cpp)
add_executable(write_file src/writeFile.cpp)
add_executable(replicate_txt_file_with_line_nums src/replicateTxtFileWithLineNums.cpp)
add_executable(psetorient src/psetorient.cpp)

if (${OpenCV_FOUND})
    message("OpenCV version: " ${OpenCV_VERSION})
    
    add_executable(img_array_to_opencv src/img_array_to_opencv.cpp)
    target_link_libraries(img_array_to_opencv ${OpenCV_LIBS} stdc++fs) # https://stackoverflow.com/questions/33149878/experimentalfilesystem-linker-error
    
    add_executable(read_img_opencv src/read_img_opencv.cpp)
    target_link_libraries(read_img_opencv ${OpenCV_LIBS})
else()
    message("Could not find OpenCV")
endif()