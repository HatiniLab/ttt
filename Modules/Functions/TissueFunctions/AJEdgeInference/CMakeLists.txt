Project (TTTAJEdgeInference)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(./include/)
include_directories(../../../Core/include/)
include_directories(../../../DataStorage/include/)
add_executable(tttAJSegmentationTest test/tttAJSegmentationTest.cpp)


target_link_libraries(tttAJSegmentationTest 
					${ITK_LIBRARIES}
					${VTK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
add_test(
    NAME tttAJSegmentationTest
    COMMAND tttAJSegmentationTest
)

