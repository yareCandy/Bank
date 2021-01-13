cmake_minimum_required(VERSION 3.0.0)
project(bank VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11) # 添加 c++11 特性

set(CMAKE_CXX_COMPILER "g++")#设置c++编译器

include_directories(${PROJECT_SOURCE_DIR}/deps)#添加头文件的搜索路径

aux_source_directory(./src SrcFiles)#将源文件列表写在变量SrcFiles中

enable_testing()

add_executable(bank ${SrcFiles})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin)#设置可执行文件输出路径
