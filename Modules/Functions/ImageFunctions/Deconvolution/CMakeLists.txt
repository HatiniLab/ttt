Project (TTTDeconvolution)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

  
# this will run uic on .ui files:


include_directories(./include/)


add_executable(tttL1L2RichardsonLucyDeconvolutionTest test/tttL1L2RichardsonLucyDeconvolutionTest.cpp)


target_link_libraries(tttL1L2RichardsonLucyDeconvolutionTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
add_test(
    NAME tttL1L2RichardsonLucyDeconvolutionTest
    COMMAND tttL1L2RichardsonLucyDeconvolutionTest
)
