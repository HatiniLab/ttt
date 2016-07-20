Project (TTTHierarchicalTessellation)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(./include/)
include_directories(../../../Core/include/)
include_directories(../../../DataStorage/include/)
add_executable(tttHierarchicalTessellationImageToAJGraphFilterTest test/tttHierarchicalTessellationImageToAJGraphFilterTest.cpp)


target_link_libraries(tttHierarchicalTessellationImageToAJGraphFilterTest 
					${ITK_LIBRARIES}
					${VTK_LIBRARIES}
					)
					
