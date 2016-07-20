/*
 * tttAJEnhancementDiffusionTest.cxx
 *
 *  Created on: Jul 5, 2016
 *      Author: morgan
 */

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImage.h>
#include <itkSliceBySliceImageFilter.h>
#include <tttAJEnhancementDiffusionImageFilter.h>
int main(int argc,char ** argv){

	typedef itk::Image<float,4> InputImageType;
	typedef itk::Image<float,3> SliceImageType;
	typedef itk::Image<float,4> OutputImageType;



	typedef itk::ImageFileReader<InputImageType> InputReaderType;

	InputReaderType::Pointer inputReader = InputReaderType::New();

	inputReader->SetFileName(argv[1]);
	inputReader->UpdateOutputInformation();


	typedef ttt::AJEnhancementDiffusionImageFilter<SliceImageType,SliceImageType> AJEnhancementDiffusionType;

	AJEnhancementDiffusionType::Pointer ajEnhancementDiffusion = AJEnhancementDiffusionType::New();

	typedef ttt::PlatenessMeasurementFunction PlatenessMeasurementFunctionType;

	PlatenessMeasurementFunctionType::Pointer function=PlatenessMeasurementFunctionType::New();

	ajEnhancementDiffusion->SetPlatenessMeasurementFunction(function);
	ajEnhancementDiffusion->SetSigmaMin(0.05);
	ajEnhancementDiffusion->SetSigmaMax(0.05);
	ajEnhancementDiffusion->SetNumberOfSigmaSteps(1);
	ajEnhancementDiffusion->SetNumberOfIterations(1000);
	ajEnhancementDiffusion->SetWStrength(5);


	typedef itk::SliceBySliceImageFilter<InputImageType,OutputImageType,AJEnhancementDiffusionType> SliceDiffusionType;


	SliceDiffusionType::Pointer sliceDiffusion = SliceDiffusionType::New();
	sliceDiffusion->SetInput(inputReader->GetOutput());
	sliceDiffusion->SetFilter(ajEnhancementDiffusion);


	typedef itk::ImageFileWriter<OutputImageType> OutputWriterType;

	OutputWriterType::Pointer outputWriter = OutputWriterType::New();

	outputWriter->SetInput(sliceDiffusion->GetOutput());

	outputWriter->SetNumberOfStreamDivisions(inputReader->GetOutput()->GetLargestPossibleRegion().GetSize(4));
	outputWriter->SetFileName(argv[2]);
	outputWriter->Update();


}
