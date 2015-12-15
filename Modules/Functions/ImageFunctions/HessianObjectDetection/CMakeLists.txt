Project (TTTHessianObjectDetection)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

  
include_directories(./include/)

add_executable(tttPlatenessMeasurementFunctionTest test/tttPlatenessMeasurementFunctionTest.cpp)

target_link_libraries(tttPlatenessMeasurementFunctionTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
add_test(
    NAME tttPlatenessMeasurementFunctionTest
    COMMAND tttPlatenessMeasurementFunctionTest
)
					
add_executable(tttVertexnessMeasurementFunctionTest test/tttVertexnessMeasurementFunctionTest.cpp)
target_link_libraries(tttVertexnessMeasurementFunctionTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
add_test(
    NAME tttVertexnessMeasurementFunctionTest
    COMMAND tttVertexnessMeasurementFunctionTest
)
