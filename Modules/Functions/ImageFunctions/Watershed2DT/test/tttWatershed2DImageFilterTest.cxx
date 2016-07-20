/*
 * FastMarching2DTImageFilterTest.cxx
 *
 *  Created on: Jun 21, 2016
 *      Author: morgan
 */


#include "tttWatershedImageFilter.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkBoundedReciprocalImageFilter.h>
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>
#include <itkSigmoidImageFilter.h>
#include <itkScalarToRGBColormapImageFilter.h>

#include <itkImage.h>

int main(int argc, char ** argv){

	typedef itk::Image<float,2> InputImageType;

	typedef itk::Image<itk::IdentifierType,2> LabelImageType;


	typedef itk::ImageFileReader<InputImageType> SpeedReaderType;



	typename SpeedReaderType::Pointer speedReader = SpeedReaderType::New();

	speedReader->SetFileName(argv[1]);

	typedef ttt::WatershedImageFilter<InputImageType,LabelImageType> WatershedFilterType;

	WatershedFilterType::Pointer watershed = WatershedFilterType::New();
	watershed->SetInput(speedReader->GetOutput());


	typedef itk::RGBPixel<unsigned char>       RGBPixelType;
	typedef itk::Image<RGBPixelType, 2>        RGBImageType;

	typedef itk::ScalarToRGBColormapImageFilter<LabelImageType, RGBImageType> RGBFilterType;
	RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
	colormapImageFilter->SetInput(watershed->GetOutput());
	colormapImageFilter->SetColormap( RGBFilterType::Jet );


	typedef itk::ImageFileWriter<RGBImageType> RGBWriterType;

	RGBWriterType::Pointer rgbWriter = RGBWriterType::New();

	rgbWriter->SetInput(colormapImageFilter->GetOutput());
	rgbWriter->SetFileName(argv[2]);
	rgbWriter->Update();


}
