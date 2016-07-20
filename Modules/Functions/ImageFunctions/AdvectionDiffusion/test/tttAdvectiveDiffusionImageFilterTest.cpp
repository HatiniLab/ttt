/*
 * tttAdvectiveDiffusionImageFilterTest.cpp
 *
 *  Created on: May 10, 2016
 *      Author: morgan
 */



#include "../../AdvectionDiffusion/include/tttAdvectiveDiffusionImageFilter.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkBoundedReciprocalImageFilter.h>
int main(int argc, char ** argv){

	typedef itk::Image<double,3> ImageType;
	typedef ttt::AdvectiveDiffusionImageFilter<ImageType,ImageType> AdvectiveDiffusionType;

	typedef itk::ImageFileReader<ImageType> ReaderType;
	typedef itk::ImageFileWriter<ImageType> WriterType;
	ReaderType::Pointer reader= ReaderType::New();

	reader->SetFileName(argv[1]);

	typedef itk::BoundedReciprocalImageFilter<ImageType,ImageType> ReciprocalFilterType;

	typename ReciprocalFilterType::Pointer reciprocal = ReciprocalFilterType::New();

	reciprocal->SetInput(reader->GetOutput());

	typename AdvectiveDiffusionType::Pointer advectiveDiffusionFilter = AdvectiveDiffusionType::New();

	advectiveDiffusionFilter->SetInput(reciprocal->GetOutput());

	WriterType::Pointer writer= WriterType::New();
	writer->SetFileName(argv[2]);
	writer->SetInput(advectiveDiffusionFilter->GetOutput());
	writer->Update();
}
