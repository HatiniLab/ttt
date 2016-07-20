Project (TTTHessianExpectation)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

  
include_directories(./include/)

add_executable(tttHessianExpectation test/tttHessianExpectation.cpp)
target_link_libraries(tttHessianExpectation ${ITK_LIBRARIES})


add_executable(tttHessianEigenvalues test/tttHessianEigenvalues.cpp)
target_link_libraries(tttHessianEigenvalues ${ITK_LIBRARIES})

