Project (TTTWatershedSegmentation)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

  
include_directories(./include/)

add_executable(tttWatershedSegmentationTest test/tttWatershedSegmentationTest.cpp)


target_link_libraries(tttWatershedSegmentationTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
add_executable(tttWatershed2DSegmentationTest test/tttWatershed2DSegmentationTest.cpp)


target_link_libraries(tttWatershed2DSegmentationTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
					