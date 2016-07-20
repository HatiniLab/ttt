/*
 * FastMarching2DTImageFilterTest.cxx
 *
 *  Created on: Jun 21, 2016
 *      Author: morgan
 */


#include "tttWatershed2DTImageFilter.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkBoundedReciprocalImageFilter.h>
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>
#include <itkSigmoidImageFilter.h>
#include <itkScalarToRGBColormapImageFilter.h>

#include <itkImage.h>

int main(int argc, char ** argv){

	typedef itk::Image<float,3> InputImageType;
	typedef itk::Image<itk::Vector<float,2>,3> RegistrationFieldType;
	typedef itk::Image<itk::IdentifierType,3> LabelImageType;


	typedef itk::ImageFileReader<InputImageType> SpeedReaderType;
	typedef itk::ImageFileReader<RegistrationFieldType> RegistrationReaderType;


	typename SpeedReaderType::Pointer speedReader = SpeedReaderType::New();

	typename RegistrationReaderType::Pointer forwardReader = RegistrationReaderType::New();
	typename RegistrationReaderType::Pointer backwardReader = RegistrationReaderType::New();

	speedReader->SetFileName(argv[1]);
	forwardReader->SetFileName(argv[2]);
	backwardReader->SetFileName(argv[3]);

	typedef ttt::Watershed2DTImageFilter<InputImageType,RegistrationFieldType,LabelImageType> Watershed2DFilterType;

	Watershed2DFilterType::Pointer watershed = Watershed2DFilterType::New();
	watershed->SetInput(speedReader->GetOutput());
	watershed->SetForwardDisplacementField(forwardReader->GetOutput());
	watershed->SetBackwardDisplacementField(backwardReader->GetOutput());



	typedef itk::RGBPixel<unsigned char>       RGBPixelType;
	typedef itk::Image<RGBPixelType, 3>        RGBImageType;

	typedef itk::ScalarToRGBColormapImageFilter<LabelImageType, RGBImageType> RGBFilterType;
	RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
	colormapImageFilter->SetInput(watershed->GetOutput());
	colormapImageFilter->SetColormap( RGBFilterType::Jet );
	colormapImageFilter->Update();

	typedef itk::ImageFileWriter<RGBImageType> RGBWriterType;

	RGBWriterType::Pointer rgbWriter = RGBWriterType::New();

	rgbWriter->SetInput(colormapImageFilter->GetOutput());
	rgbWriter->SetFileName(argv[4]);
	rgbWriter->Update();


}
