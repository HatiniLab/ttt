/*
 * tttAdvectiveDiffusion2DIterationImageFilterTest.cpp
 *
 *  Created on: Jun 30, 2016
 *      Author: morgan
 */


#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImage.h>
#include <itkExtractImageFilter.h>
#include <itkImage.h>
#include <itkBoundedReciprocalImageFilter.h>

#include "tttAdvectionDiffusion2DIterationImageFilter.h"

int main(int argc,char ** argv){

	typedef itk::Image<float,3> ImageType;
	typedef itk::Image<float,2> SliceType;

	typedef itk::ImageFileReader<ImageType> ReaderType;
	typedef itk::ImageFileWriter<SliceType> WriterType;

	typedef itk::BoundedReciprocalImageFilter<ImageType,ImageType> BoundedReciprocalType;
	BoundedReciprocalType::Pointer boundedReciprocal = BoundedReciprocalType::New();


	typedef ttt::AdvectiveDiffusion2DIterationImageFilter<SliceType,SliceType> AdvectionDiffusion2DIterationType;

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);
	reader->UpdateOutputInformation();


	typedef itk::ExtractImageFilter<ImageType,SliceType> ExtractorType;

	ExtractorType::Pointer extractor = ExtractorType::New();

	ImageType::RegionType extractionRegion = reader->GetOutput()->GetLargestPossibleRegion();
	extractionRegion.SetSize(2,0);

	boundedReciprocal->SetInput(reader->GetOutput());
	extractor->SetInput(boundedReciprocal->GetOutput());
	extractor->SetExtractionRegion(extractionRegion);
	extractor->SetDirectionCollapseToIdentity();

	AdvectionDiffusion2DIterationType::Pointer advectionDiffusionIteration =AdvectionDiffusion2DIterationType::New();

	advectionDiffusionIteration->SetInput(extractor->GetOutput());
	advectionDiffusionIteration->SetNumberOfThreads(1.0);


	WriterType::Pointer sliceWriter = WriterType::New();
	sliceWriter->SetInput(extractor->GetOutput());
	sliceWriter->SetFileName(argv[2]);
	sliceWriter->Update();


	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(argv[3]);

	writer->SetInput(advectionDiffusionIteration->GetOutput());
	writer->Update();

}
