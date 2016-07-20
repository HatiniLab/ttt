Project (TTTDataStorage)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(./include/)
include_directories(../Core/include/)

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


add_executable(tttTissueReaderTest test/tttTissueReaderTest.cpp)


target_link_libraries(tttTissueReaderTest 
					${ITK_LIBRARIES}
					${VTK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
add_test(
    NAME tttTissueReaderTest
    COMMAND tttTissueReaderTest
)

add_executable(tttTissueWriterTest test/tttTissueWriterTest.cpp)


target_link_libraries(tttTissueWriterTest 
					${ITK_LIBRARIES}
					${VTK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
add_test(
    NAME tttTissueWriterTest
    COMMAND tttTissueWriterTest
)