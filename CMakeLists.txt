cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_CXX_STANDARD 17) #https://stackoverflow.com/questions/45688522/how-to-enable-c17-in-cmake
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(CppSnippets VERSION 1.0.0)

set(SimpleITK_DIR "$ENV{HOME}/SimpleITK-build/SimpleITK-build")
message(STATUS "SimpleITK_DIR: ${SimpleITK_DIR}")

set(ITK_DIR "$ENV{HOME}/SimpleITK-build/ITK-build")
message(STATUS "ITK_DIR: ${ITK_DIR}")

find_package(SimpleITK)
message(STATUS "SimpleITK_FOUND=${SimpleITK_FOUND}")
if(SimpleITK_FOUND)
    message(STATUS "SimpleITK version: " ${SimpleITK_VERSION})
    
    add_executable(sitk_image_ios src/image/sitk_image_ios.cpp)
    target_link_libraries(sitk_image_ios ${SimpleITK_LIBRARIES})
else()
    message(STATUS "SimpleITK not found!")
endif()

set(OpenCV_DIR "$ENV{HOME}/opencv-build")
message(STATUS "OpenCV_DIR: ${OpenCV_DIR}")

find_package(OpenCV)
message(STATUS "OpenCV_FOUND=${OpenCV_FOUND}")
if (OpenCV_FOUND)
    message(STATUS "OpenCV version: " ${OpenCV_VERSION})

    include_directories(${OpenCV_INCLUDE_DIRS})
    
    add_executable(img_array_to_opencv src/img_array_to_opencv.cpp)
    # https://stackoverflow.com/questions/33149878/experimentalfilesystem-linker-error
    target_link_libraries(img_array_to_opencv ${OpenCV_LIBS}) # stdc++fs (if using g++-8)
    
    add_executable(array_to_opencv src/array_to_opencv.cpp)
    target_link_libraries(array_to_opencv ${OpenCV_LIBS})

    add_executable(opencv_to_array src/opencv_to_array.cpp)
    target_link_libraries(opencv_to_array ${OpenCV_LIBS})

    add_executable(read_img_opencv src/read_img_opencv.cpp)
    target_link_libraries(read_img_opencv ${OpenCV_LIBS})
    
else()
    message(STATUS "OpenCV not found!")
endif()

if (SimpleITK_FOUND AND OpenCV_FOUND)
    # message(STATUS "Both are found")
    add_executable(read_raw src/image/read_raw.cpp src/Input_Parser.cpp)
    target_include_directories(read_raw PRIVATE ./src)
    target_link_libraries(read_raw ${OpenCV_LIBS} ${SimpleITK_LIBRARIES}) # stdc++fs (if using g++-8, not guaranteed to link correctly)
    
    add_executable(cpbd src/image/cpbd.cpp)
    target_link_libraries(cpbd ${OpenCV_LIBS})
endif()

add_executable(check_cpp src/check.cpp)
add_executable(general src/general.cpp)
add_executable(timing src/timing.cpp)
add_executable(formatting src/formatting.cpp)
add_executable(write_file src/writeFile.cpp)
add_executable(replicate_txt_file_with_line_nums src/replicateTxtFileWithLineNums.cpp)
add_executable(pos src/pos.cpp)
add_executable(psetorient src/psetorient.cpp)