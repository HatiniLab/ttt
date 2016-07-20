Project(ttt)

cmake_minimum_required(VERSION 2.8.8)
project(TTT2005.04)
#add_definitions("-std=c++11 -fpermissive -g ")
#add_definitions("-O2")



set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_BUILD_TYPE Debug)

IF(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-fpermissive)
ENDIF() 

find_package(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

find_package(VTK)
#find_package(VTK COMPONENTS
#  vtkCommonCore
#  vtkFiltersCore
#  vtkInfovisCore
#  vtkInteractionStyle
#  #vtkRenderingFreeType${VTK_RENDERING_BACKEND}
#  vtkRenderingVolume${VTK_RENDERING_BACKEND}
#  vtkViewsQt
#  vtkjsoncpp
#  vtkChartsCore
#  vtkRenderingContext${VTK_RENDERING_BACKEND}
#  vtkViewsContext2D
#  vtkIOFFMPEG
#)


include(${VTK_USE_FILE})

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_subdirectory(./Modules/5DTesting)
add_subdirectory(./Modules/DataStorage)
#add_subdirectory(./Modules/Functions/ImageFunctions/Deconvolution)
#add_subdirectory(./Modules/Functions/ImageFunctions/HessianObjectDetection)
add_subdirectory(./Modules/Functions/ImageFunctions/SurfaceExtraction)
#add_subdirectory(./Modules/Functions/ImageFunctions/PotentialFieldTransform)
#add_subdirectory(./Modules/Functions/ImageFunctions/CellSegmentation)
#add_subdirectory(./Modules/Functions/ImageFunctions/TemporalSmoothing)
#add_subdirectory(./Modules/Functions/ImageFunctions/PlaneFittingSurfaceExtraction)
add_subdirectory(./Modules/Functions/ImageFunctions/AdvectionDiffusion)
#add_subdirectory(./Modules/Functions/ImageFunctions/AnisotropicDiffusion)
add_subdirectory(./Modules/Functions/ImageFunctions/WatershedSegmentation)
add_subdirectory(./Modules/Functions/ImageFunctions/FastMarching2DT)
add_subdirectory(./Modules/Functions/TissueFunctions/FastMarchingCellSegmentationAndTracking)
add_subdirectory(./Modules/Functions/ImageFunctions/AJEnhancementDiffusion)
add_subdirectory(./Modules/Functions/ImageFunctions/HessianExpectation)
#add_subdirectory(./Modules/Functions/TissueFunctions/LocalMaximaAJGraphSource)
#add_subdirectory(./Modules/Functions/TissueFunctions/AJEdgeInference)
#add_subdirectory(./Modules/Functions/TissueFunctions/CellIdentification)
#add_subdirectory(./Modules/Functions/TissueFunctions/CellCorrespondence)
#add_subdirectory(./Modules/Functions/TissueFunctions/MTTAJGraphInitialization)
#add_subdirectory(./Modules/Functions/TissueFunctions/CellGraphInitialization)
add_subdirectory(./Modules/Functions/ImageFunctions/Watershed2DT)
add_subdirectory(./Modules/Functions/TissueFunctions/HierarchicalTessellation)
