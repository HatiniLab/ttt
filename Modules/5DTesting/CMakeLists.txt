Project (TTT5DTesting)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

  
include_directories(./include/)
include_directories(../Functions/ImageFunctions/SurfaceExtraction/include)
include_directories(../Functions/ImageFunctions/AdvectionDiffusion/include)
include_directories(../Functions/ImageFunctions/AJEnhancementDiffusion/include)

add_executable(tttFrameByFrameStreamingTest test/tttFrameByFrameStreamingTest.cxx)

target_link_libraries(tttFrameByFrameStreamingTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
					
					
add_executable(tttProjectWithSurfaceFilterTest test/tttProjectWithSurfaceFilterTest.cxx)

target_link_libraries(tttProjectWithSurfaceFilterTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)

					
add_executable(tttProjectRegistrationFieldWithSurfaceFilterTest test/tttProjectRegistrationFieldWithSurfaceFilterTest.cxx)

target_link_libraries(tttProjectRegistrationFieldWithSurfaceFilterTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
										
										
add_executable(tttCurvatureDiffusionTest test/tttCurvatureDiffusionTest.cxx)

target_link_libraries(tttCurvatureDiffusionTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
					
add_executable(tttWatershedFilterTest test/tttWatershedFilterTest.cxx)

target_link_libraries(tttWatershedFilterTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
add_executable(tttConcatenatorTest test/tttConcatenatorTest.cxx)

target_link_libraries(tttConcatenatorTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
add_executable(tttRegistrationFieldConcatenatorTest test/tttRegistrationFieldConcatenatorTest.cxx)

target_link_libraries(tttRegistrationFieldConcatenatorTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
				
					
add_executable(tttTemporalAdvectionDiffusionTest test/tttTemporalAdvectionDiffusionTest.cxx)

target_link_libraries(tttTemporalAdvectionDiffusionTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
add_executable(tttAJEnhancementDiffusionTest test/tttAJEnhancementDiffusionTest.cxx)

target_link_libraries(tttAJEnhancementDiffusionTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)
					
add_executable(tttSurfaceExtractionImageFilterTest test/tttSurfaceExtractionImageFilterTest.cxx)

target_link_libraries(tttSurfaceExtractionImageFilterTest 
					${ITK_LIBRARIES}
					${GTEST_LIBRARY}		
					)