/*
 * tttDataStorageTest.cpp
 *
 *  Created on: May 27, 2015
 *      Author: morgan
 */

#include <gtest/gtest.h>
#include <tttDataset.h>


TEST(tttAbstractDataRepositoryTest,ReadTest){
#if 0
	ttt::Dataset<3>::Pointer repository = ttt::Dataset<3>::New();

	repository->SetDirectory("/home/morgan/testproject2015.04");
	repository->Open();

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
