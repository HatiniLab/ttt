Project (TTTWatershed2DT)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

  
include_directories(./include/)

add_executable(tttWatershed2DTImageFilterTest test/tttWatershed2DTImageFilterTest.cxx)


target_link_libraries(tttWatershed2DTImageFilterTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)

add_executable(tttWatershed2DImageFilterTest test/tttWatershed2DImageFilterTest.cxx)


target_link_libraries(tttWatershed2DImageFilterTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					