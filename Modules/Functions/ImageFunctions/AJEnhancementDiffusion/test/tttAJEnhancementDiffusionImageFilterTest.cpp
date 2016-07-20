#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkRescaleIntensityImageFilter.h"
#include "itkAbsoluteValueDifferenceImageFilter.h"
#include <tttAJEnhancementDiffusionImageFilter.h>
#include <tttPlatenessMeasurementFunction.h>
int main(int argc,char ** argv){

	typedef itk::Image<float,3> InputImageType;

	typedef itk::ImageFileReader<InputImageType> ImageFileReaderType;

	ImageFileReaderType::Pointer reader = ImageFileReaderType::New();

	reader->SetFileName(argv[1]);

	typedef ttt::AJEnhancementDiffusionImageFilter<InputImageType,InputImageType> AJEnhancementDiffusionType;

	AJEnhancementDiffusionType::Pointer ajEnhancementDiffusion = AJEnhancementDiffusionType::New();

	ajEnhancementDiffusion->SetInput(reader->GetOutput());

	typedef ttt::PlatenessMeasurementFunction PlatenessMeasurementFunctionType;

	PlatenessMeasurementFunctionType::Pointer function=PlatenessMeasurementFunctionType::New();

	ajEnhancementDiffusion->SetPlatenessMeasurementFunction(function);
	ajEnhancementDiffusion->SetSigmaMin(0.05);
	ajEnhancementDiffusion->SetSigmaMax(0.05);
	ajEnhancementDiffusion->SetNumberOfSigmaSteps(1);
	ajEnhancementDiffusion->SetNumberOfIterations(1000);
	ajEnhancementDiffusion->SetWStrength(5);

	typedef itk::ImageFileWriter<InputImageType> WriterType;

	WriterType::Pointer writer = WriterType::New();


	writer->SetInput(ajEnhancementDiffusion->GetOutput());
	writer->SetFileName(argv[2]);
	writer->Update();


}

