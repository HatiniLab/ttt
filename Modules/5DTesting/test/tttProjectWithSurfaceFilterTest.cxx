
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkMultiplyImageFilter.h>
#include <itkAccumulateImageFilter.h>
#include <itkSCIFIOImageIO.h>
#include <itkExtractImageFilter.h>
//#include "tttProjectWithSurfaceImageFilter.h"

int main(int argc, char ** argv){


	typedef itk::Image<float,4> SurfaceImageType;
	typedef itk::Image<float,4> InputImageType;
	typedef itk::Image<float,3> OutputImageType;

	typedef itk::ImageFileReader<SurfaceImageType> SurfaceReader;
	typedef itk::ImageFileReader<InputImageType> InputReader;

	typedef itk::ImageFileWriter<OutputImageType> OutputWriter;

	typename InputReader::Pointer inputReader = InputReader::New();
	inputReader->SetFileName(argv[1]);
	//inputReader->Update();
	//std::cout << "Input" <<inputReader->GetOutput()->GetLargestPossibleRegion() << std::endl;
	typename SurfaceReader::Pointer surfaceReader = SurfaceReader::New();
	surfaceReader->SetFileName(argv[2]);
	//surfaceReader->Update();
	//std::cout << "Surface" <<surfaceReader->GetOutput()->GetLargestPossibleRegion() << std::endl;

	typedef itk::MultiplyImageFilter<InputImageType,SurfaceImageType,InputImageType> MultiplierType;
	typename MultiplierType::Pointer multiplier = MultiplierType::New();
	multiplier->SetInput1(inputReader->GetOutput());
	multiplier->SetInput2(surfaceReader->GetOutput());
	//multiplier->Update();
	//std::cout << "Multiplier" <<multiplier->GetOutput()->GetLargestPossibleRegion() << std::endl;

	typedef itk::AccumulateImageFilter<InputImageType,InputImageType> AccumulatorType;
	AccumulatorType::Pointer accumulator = AccumulatorType::New();
	accumulator->SetAccumulateDimension(2);
	accumulator->SetInput(multiplier->GetOutput());

	accumulator->UpdateOutputInformation();

	typedef itk::ExtractImageFilter<InputImageType,OutputImageType> ExtractorType;
	ExtractorType::Pointer extractor = ExtractorType::New();

	InputImageType::RegionType largestRegion = accumulator->GetOutput()->GetLargestPossibleRegion();

	InputImageType::RegionType extractionRegion;
	extractionRegion.SetIndex(0,largestRegion.GetIndex(0));
	extractionRegion.SetIndex(1,largestRegion.GetIndex(1));
	extractionRegion.SetIndex(2,largestRegion.GetIndex(2));
	extractionRegion.SetIndex(3,largestRegion.GetIndex(3));

	extractionRegion.SetSize(0,largestRegion.GetSize(0));
	extractionRegion.SetSize(1,largestRegion.GetSize(1));
	extractionRegion.SetSize(2,0);
	extractionRegion.SetSize(3,largestRegion.GetSize(3));


	extractor->SetExtractionRegion(extractionRegion);
	extractor->SetDirectionCollapseToIdentity();
	extractor->SetInput(accumulator->GetOutput());

	typename OutputWriter::Pointer outputWriter = OutputWriter::New();
	outputWriter->SetFileName(argv[3]);
	outputWriter->SetInput(extractor->GetOutput());
	outputWriter->SetNumberOfStreamDivisions(33);
	outputWriter->Update();

}
