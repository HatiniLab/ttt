Project(ttt)

cmake_minimum_required(VERSION 2.8.8)
project(TTT2005.04)
#add_definitions("-std=c++11 -fpermissive -g")
SET(CMAKE_BUILD_TYPE Debug)
set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)


find_package(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_subdirectory(./Modules/Functions/ImageFunctions/Deconvolution)
