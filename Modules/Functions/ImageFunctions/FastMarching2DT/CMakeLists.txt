Project(FastMarching2DT)

include_directories(./include)

add_executable(tttFastMarching2DTImageFilterTest test/tttFastMarching2DTImageFilterTest.cxx)

target_link_libraries(tttFastMarching2DTImageFilterTest ${ITK_LIBRARIES})