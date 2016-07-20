
#include <itkSliceBySliceImageFilter.h>

#include "itkGradientMagnitudeImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

int main(int arg, char ** argv){

	typedef itk::Image<float,3> InputImageType;
	typedef itk::RGBPixel<unsigned char>       RGBPixelType;
	typedef itk::Image<RGBPixelType, 3>        RGBImageType;

	typedef itk::Image<float,2> InternalImageType;
	typedef itk::Image<itk::IdentifierType,2> InternalLabelImageType;

	typedef itk::Image<RGBPixelType, 2>        InternalRGBImageType;




	typedef itk::ImageFileReader<InputImageType> ReaderType;

	ReaderType::Pointer reader = ReaderType::New();

	reader->SetFileName(argv[1]);
	reader->UpdateOutputInformation();

	typedef itk::GradientMagnitudeImageFilter<InternalImageType, InternalImageType >  GradientMagnitudeImageFilterType;

	GradientMagnitudeImageFilterType::Pointer gradientMagnitudeImageFilter = GradientMagnitudeImageFilterType::New();



	typedef itk::WatershedImageFilter<InternalImageType> WatershedFilterType;
	WatershedFilterType::Pointer watershed = WatershedFilterType::New();
	watershed->SetThreshold(0.01);
	watershed->SetLevel(0.4);
	watershed->SetInput(gradientMagnitudeImageFilter->GetOutput());

	typedef itk::ScalarToRGBColormapImageFilter<InternalLabelImageType, InternalRGBImageType> RGBFilterType;
	RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
	colormapImageFilter->SetInput(watershed->GetOutput());
	colormapImageFilter->SetColormap( RGBFilterType::Jet );


	typedef itk::SliceBySliceImageFilter<InputImageType,RGBImageType,GradientMagnitudeImageFilterType,RGBFilterType> SliceBySliceProcessor;

	SliceBySliceProcessor::Pointer sliceBySliceProcessor = SliceBySliceProcessor::New();
	sliceBySliceProcessor->SetInput(reader->GetOutput());
	sliceBySliceProcessor->SetInputFilter(gradientMagnitudeImageFilter);
	sliceBySliceProcessor->SetOutputFilter(colormapImageFilter);
	sliceBySliceProcessor->SetDimension(2);

	typedef itk::ImageFileWriter<RGBImageType> WriterType;
	WriterType::Pointer writer = WriterType::New();


	writer->SetFileName(argv[2]);
	writer->SetNumberOfStreamDivisions(reader->GetOutput()->GetLargestPossibleRegion().GetSize(2));

	writer->SetInput(sliceBySliceProcessor->GetOutput());
	writer->Update();

}
