cmake_minimum_required(VERSION 3.1.0)
set (CMAKE_CXX_STANDARD 11)
project(CppSnippets VERSION 1.0.0)

set(SimpleITK_DIR "$ENV{HOME}/source/SimpleITK-build/SimpleITK-build")
set(ITK_DIR "$ENV{HOME}/source/SimpleITK-build/ITK-build")

message(STATUS "SimpleITK_DIR: ${SimpleITK_DIR}")
message(STATUS "ITK_DIR: ${ITK_DIR}")

find_package(SimpleITK)
if(SimpleITK_FOUND)
    add_executable(read_raw src/image/read_raw.cpp)
    target_link_libraries(read_raw ${SimpleITK_LIBRARIES})
else()
    message(STATUS "SimpleITK package not found!")
endif()


add_executable(general src/general.cpp)
add_executable(timing src/timing.cpp)
add_executable(formatting src/formatting.cpp)
add_executable(write_file src/writeFile.cpp)
add_executable(replicate_txt_file_with_line_nums src/replicateTxtFileWithLineNums.cpp)
add_executable(pos src/pos.cpp)