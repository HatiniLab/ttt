Project (TTTAdvectiveDiffusion)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

  
include_directories(./include/)

#add_executable(tttAdvectiveDiffusionImageFilterTest test/tttAdvectiveDiffusionImageFilterTest.cpp)
#add_executable(tttCellnessTest test/tttCellnessTest.cpp)

#target_link_libraries(tttAdvectiveDiffusionImageFilterTest 
#					${ITK_LIBRARIES}
#					${GTEST_LIBRARY}		
#					)
					
add_executable(tttAdvectiveDiffusion2DIterationImageFilterTest test/tttAdvectiveDiffusion2DIterationImageFilterTest.cpp)
target_link_libraries(tttAdvectiveDiffusion2DIterationImageFilterTest ${ITK_LIBRARIES} ${VTK_LIBRARIES})