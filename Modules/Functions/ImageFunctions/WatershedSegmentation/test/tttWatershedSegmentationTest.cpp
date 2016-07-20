/*
 * tttWatershedSegmentationTest.cpp
 *
 *  Created on: May 16, 2016
 *      Author: morgan
 */


#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkWatershedImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkGradientMagnitudeImageFilter.h>

int main(int argc,char ** argv){

	typedef itk::Image<float, 3> ImageType;
	typedef itk::Image<unsigned short,3> LabelImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	typedef itk::ImageFileWriter<LabelImageType> WriterType;

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);

	typedef itk::GradientMagnitudeImageFilter<ImageType,ImageType> GradientMagnitudeType;
	GradientMagnitudeType::Pointer gradientMagnitude = GradientMagnitudeType::New();
	gradientMagnitude->SetInput(reader->GetOutput());

	typedef itk::WatershedImageFilter<ImageType> WatershedFilterType;


	WatershedFilterType::Pointer watershedFilter = WatershedFilterType::New();
	watershedFilter->SetInput(gradientMagnitude->GetOutput());
	watershedFilter->SetThreshold(0.01);
	watershedFilter->SetLevel(1);


	typedef itk::CastImageFilter<WatershedFilterType::OutputImageType, LabelImageType> CasterType;
	CasterType::Pointer caster = CasterType::New();
	caster->SetInput(watershedFilter->GetOutput());


	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(caster->GetOutput());
	writer->SetFileName(argv[2]);
	writer->Update();


}
