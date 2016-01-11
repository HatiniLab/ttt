/*
 * tttDataStorageTest.cpp
 *
 *  Created on: May 27, 2015
 *      Author: morgan
 */

#include <gtest/gtest.h>
#include <tttDataset.h>


TEST(tttAbstractDataRepositoryTest,ReadTest){

	ttt::Dataset::Pointer repository = ttt::Dataset::New();

	repository->SetPath("/home/morgan/testproject2015.04");
	repository->Load();
	auto frame=repository->GetFrame(0,"RawImage");
	typedef itk::Image<double,3> ImageType;
	typedef itk::ImageFileReader<ImageType > SourceType;
	typedef itk::ImageFileWriter<ImageType > SinkType;
	typename SourceType::Pointer frameSource= frame.GetFrameSource<SourceType>();
	typename SinkType::Pointer frameSink= frame.GetFrameSink<SinkType>();

#if 0

	auto sequence=repository->GetLayer("RawImage");
	assert(sequence);
	ASSERT_EQ(sequence->GetLargestPossibleTemporalRegion().GetFrameDuration(),101);

	for(int t=0;t<sequence->GetLargestPossibleTemporalRegion().GetFrameDuration();t++){
		auto frame = sequence->GetFrame(t);
		assert(frame);
	}
#endif
};

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
