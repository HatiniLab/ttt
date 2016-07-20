Project(CellSegmentationTracking2DT)

include_directories(./include)

include_directories(../../../Core/include)
include_directories(../../../DataStorage/include)
include_directories(../../ImageFunctions/FastMarching2DT/include)


add_executable(tttCellSegmentationTracking2DTTest test/tttCellSegmentationTracking2DTTest.cxx)

target_link_libraries(tttCellSegmentationTracking2DTTest ${ITK_LIBRARIES} ${VTK_LIBRARIES})