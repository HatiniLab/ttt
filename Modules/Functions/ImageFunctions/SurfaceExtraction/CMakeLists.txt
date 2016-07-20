Project (TTTSurfaceExtraction)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

  
include_directories(./include/)

add_executable(tttTissueSurfaceMeshSourceTest test/tttTissueSurfaceMeshSourceTest.cpp)

target_link_libraries(tttTissueSurfaceMeshSourceTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
add_test(
    NAME tttTissueSurfaceMeshSourceTest
    COMMAND tttTissueSurfaceMeshSourceTest
)

add_executable(tttTissueSurfaceImageFilterTest test/tttTissueSurfaceImageFilterTest.cpp)

target_link_libraries(tttTissueSurfaceImageFilterTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
add_test(
    NAME tttTissueSurfaceImageFilterTest
    COMMAND tttTissueSurfaceImageFilterTest
)

#add_executable(tttPyramidTissueSurfaceMeshSourceTest test/tttPyramidTissueSurfaceMeshSourceTest.cpp)

#target_link_libraries(tttPyramidTissueSurfaceMeshSourceTest 
#					${ITK_LIBRARIES}
#					${GTEST_LIBRARY}		
#					)
#add_test(
#    NAME tttPyramidTissueSurfaceMeshSourceTest
#    COMMAND tttPyramidTissueSurfaceMeshSourceTest
#)

#add_executable(tttBlurThresholdingMarchingCubesTest test/tttBlurThresholdingMarchingCubesTest.cpp)

#target_link_libraries(tttBlurThresholdingMarchingCubesTest 
#					${ITK_LIBRARIES}
#					${GTEST_LIBRARY}		
#					)
#add_test(
#    NAME tttBlurThresholdingMarchingCubesTest
#    COMMAND tttBlurThresholdingMarchingCubesTest
#)