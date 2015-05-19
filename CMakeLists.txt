Project(ttt)

cmake_minimum_required(VERSION 2.8.8)

add_definitions("-std=c++11 -fpermissive -g")

project(TTT2005.04)

find_package(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_subdirectory(./Modules/Functions/ImageFunctions/Deconvolution)
