Project(AJEnhancementDiffusion)

include_directories(include)
include_directories(../Base/include/)
include_directories(../ObjectnessMeasurementBase/include/)
include_directories(../Plateness/include/)
include_directories(../../Core/include/)

add_executable(tttAJEnhancementDiffusionImageFilterTest test/tttAJEnhancementDiffusionImageFilterTest.cpp)

target_link_libraries(tttAJEnhancementDiffusionImageFilterTest ${ITK_LIBRARIES} )
