/*
 * tttCurvatureDiffusionTest.cxx
 *
 *  Created on: May 27, 2016
 *      Author: morgan
 */

#include <itkImage.h>
#include <itkSliceBySliceImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkCurvatureAnisotropicDiffusionImageFilter.h>
#include <itkBoundedReciprocalImageFilter.h>

int main(int argc,char ** argv){

	typedef itk::Image<float,3> ImageType;
	typedef itk::Image<float,2> InternalImageType;

	typedef itk::ImageFileReader<ImageType> ReaderType;
	typedef itk::ImageFileWriter<ImageType> WriterType;

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);
	reader->UpdateOutputInformation();
	typedef itk::CurvatureAnisotropicDiffusionImageFilter<InternalImageType,InternalImageType> DiffusionType;
	typedef itk::SliceBySliceImageFilter<ImageType,ImageType,DiffusionType> TDiffusionType;

	typedef itk::BoundedReciprocalImageFilter<ImageType,ImageType> ReciprocalType;

	ReciprocalType::Pointer reciprocal = ReciprocalType::New();
	reciprocal->SetInput(reader->GetOutput());



	DiffusionType::Pointer diffusion = DiffusionType::New();
	diffusion->SetTimeStep(5e-7);
	diffusion->SetNumberOfIterations(100);
	diffusion->SetConductanceParameter(5);
	TDiffusionType::Pointer tDiffusion = TDiffusionType::New();
	tDiffusion->SetInput(reciprocal->GetOutput());
	tDiffusion->SetFilter(diffusion);



	WriterType::Pointer writer = WriterType::New();

	writer->SetInput(tDiffusion->GetOutput());
	writer->SetFileName(argv[2]);
	writer->SetNumberOfStreamDivisions(reader->GetOutput()->GetLargestPossibleRegion().GetSize(2));
	writer->Update();

}
