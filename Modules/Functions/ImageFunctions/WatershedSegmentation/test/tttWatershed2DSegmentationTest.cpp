

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkWatershedImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkScalarToRGBColormapImageFilter.h>
int main(int argc,char ** argv){

	typedef itk::Image<float, 2> ImageType;
	typedef itk::Image<itk::IdentifierType,2> LabelImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;


	typedef itk::RGBPixel<unsigned char>       RGBPixelType;
	typedef itk::Image<RGBPixelType, 2>        RGBImageType;
	typedef itk::ImageFileWriter<RGBImageType> WriterType;

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);


	typedef itk::WatershedImageFilter<ImageType> WatershedFilterType;


	WatershedFilterType::Pointer watershedFilter = WatershedFilterType::New();
	watershedFilter->SetInput(reader->GetOutput());
	watershedFilter->SetThreshold(atof(argv[3]));
	watershedFilter->SetLevel(atof(argv[4]));


	typedef itk::ScalarToRGBColormapImageFilter<LabelImageType, RGBImageType> RGBFilterType;
	RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
	colormapImageFilter->SetInput(watershedFilter->GetOutput());
	colormapImageFilter->SetColormap( RGBFilterType::Jet );
	colormapImageFilter->Update();

	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(colormapImageFilter->GetOutput());
	writer->SetFileName(argv[2]);
	writer->Update();

}
