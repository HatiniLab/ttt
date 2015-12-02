Project (TTTDataStorage)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(./include/)

add_executable(tttDataStorageTest test/tttDataStorageTest.cpp)


target_link_libraries(tttDataStorageTest 
					${ITK_LIBRARIES}
					${VTK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
add_test(
    NAME tttDataStorageTest
    COMMAND tttDataStorageTest
)
