#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkExtractImageFilter.h>
#include <tttStreamedTestFilter.h>
#include <itkSCIFIOImageIO.h>
#include <itkPipelineMonitorImageFilter.h>
#include <itkDivideImageFilter.h>
int main(int argc,char ** argv){

	typedef itk::Image<float,5> InputImageType;
	typedef itk::Image<float,4> OutputImageType;

	typedef itk::ImageFileReader<InputImageType> ReaderType;
	typedef itk::ImageFileWriter<OutputImageType> WriterType;



	typedef InputImageType::RegionType RegionType;




	typedef ttt::StreamedTestFilter<OutputImageType,OutputImageType> StreamedFilterType;

	ReaderType::Pointer reader= ReaderType::New();
	reader->SetFileName(argv[1]);
	reader->UpdateOutputInformation();

	RegionType extractionRegion;

	extractionRegion.SetIndex(0,0);
	extractionRegion.SetIndex(1,0);
	extractionRegion.SetIndex(2,0);
	extractionRegion.SetIndex(3,0);
	extractionRegion.SetIndex(4,0);

	extractionRegion.SetSize(0,reader->GetOutput()->GetLargestPossibleRegion().GetSize(0));
	extractionRegion.SetSize(1,reader->GetOutput()->GetLargestPossibleRegion().GetSize(1));
	extractionRegion.SetSize(2,reader->GetOutput()->GetLargestPossibleRegion().GetSize(2));
	extractionRegion.SetSize(3,reader->GetOutput()->GetLargestPossibleRegion().GetSize(3));
	extractionRegion.SetSize(4,0);


	typedef itk::DivideImageFilter<InputImageType,InputImageType,InputImageType> DividerType;

	DividerType::Pointer divider = DividerType::New();
	divider->SetInput1(reader->GetOutput());
	divider->SetConstant2(65536.0);



	typedef itk::ExtractImageFilter<InputImageType,OutputImageType> ExtractorType;


	typename ExtractorType::Pointer extractor =ExtractorType::New();

	extractor->SetInput(divider->GetOutput());

	extractor->SetDirectionCollapseToIdentity();

	extractor->SetExtractionRegion(extractionRegion);

	//typedef itk::PipelineMonitorImageFilter< OutputImageType > MonitorFilterType;
	//MonitorFilterType::Pointer monitorFilter = MonitorFilterType::New();
	//monitorFilter->SetInput( extractor->GetOutput() );
	// If ITK was built with the Debug CMake configuration, the filter
	// automatically outputs status information to the console
	//monitorFilter->DebugOn();

	StreamedFilterType::Pointer streamedFilter = StreamedFilterType::New();
	streamedFilter->SetInput(extractor->GetOutput());
	//streamedFilter->SetNumberOfThreads(1);

	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(argv[2]);
	writer->SetNumberOfStreamDivisions(atoi(argv[3]));
	writer->SetInput(streamedFilter->GetOutput());
	writer->Update();

}




