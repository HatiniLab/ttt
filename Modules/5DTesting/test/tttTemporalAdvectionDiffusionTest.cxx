/*
 * tttTemporalAdvectionDiffusionTest.cxx
 *
 *  Created on: Jun 1, 2016
 *      Author: morgan
 */

#include "tttTemporalAdvectionDiffusionImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkVector.h"
#include "itkBoundedReciprocalImageFilter.h"
int main(int argc, char ** argv){

	typedef itk::Vector<float,2> VectorType;

	typedef itk::Image<float,3> ImageType;

	typedef itk::Image<VectorType,3> VectorImageType;

	typedef itk::ImageFileReader<ImageType> ImageReaderType;
	typedef itk::ImageFileReader<VectorImageType> VectorImageReaderType;

	typedef itk::ImageFileWriter<ImageType> WriterType;

	ImageReaderType::Pointer reader = ImageReaderType::New();
	reader->SetFileName(argv[1]);
	reader->UpdateOutputInformation();
	std::cout << reader->GetOutput()->GetLargestPossibleRegion() << std::endl;

	VectorImageReaderType::Pointer forwardVectorReader = VectorImageReaderType::New();
	forwardVectorReader->SetFileName(argv[2]);
	forwardVectorReader->UpdateOutputInformation();
	std::cout << forwardVectorReader->GetOutput()->GetLargestPossibleRegion() << std::endl;

	VectorImageReaderType::Pointer backwardVectorReader = VectorImageReaderType::New();
	backwardVectorReader->SetFileName(argv[3]);
	backwardVectorReader->UpdateOutputInformation();
	std::cout << backwardVectorReader->GetOutput()->GetLargestPossibleRegion() << std::endl;


	typedef itk::BoundedReciprocalImageFilter<ImageType,ImageType> ReciprocalType;
	ReciprocalType::Pointer reciprocal = ReciprocalType::New();
	reciprocal->SetInput(reader->GetOutput());

	typedef ttt::TemporalAdvectionDiffusionImageFilter<ImageType,VectorImageType,ImageType> TemporalAdvectionDiffusionType;

	typename TemporalAdvectionDiffusionType::Pointer temporalAdvectionDiffusion = TemporalAdvectionDiffusionType::New();

	temporalAdvectionDiffusion->SetInputImage(reciprocal->GetOutput());
	temporalAdvectionDiffusion->SetForwardDisplacementField(forwardVectorReader->GetOutput());
	temporalAdvectionDiffusion->SetBackwardDisplacementField(backwardVectorReader->GetOutput());

	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(argv[4]);
	writer->SetInput(reciprocal->GetOutput());
	writer->Update();

	WriterType::Pointer writer2 = WriterType::New();
	writer2->SetFileName(argv[5]);
	writer2->SetInput(temporalAdvectionDiffusion->GetOutput());
	writer2->Update();



}
