/*
 * tttTissueSurfaceImageFilterTest.cpp
 *
 *  Created on: May 25, 2016
 *      Author: morgan
 */




#include "tttTissueSurfaceImageFilter.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
int main(int argc,char ** argv){
	typedef float InputPixelType;
	typedef unsigned char OutputPixelType;


	typedef itk::Image<InputPixelType,3> InputImageType;
	typedef itk::Image<OutputPixelType,3> OutputImageType;

	typedef itk::ImageFileReader<InputImageType> ReaderType;

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);

	typedef itk::ImageFileWriter<OutputImageType> WriterType;

	WriterType::Pointer writer = WriterType::New();

	typedef ttt::TissueSurfaceImageFilter<InputImageType,OutputImageType> TissueSurfaceFilterType;

	TissueSurfaceFilterType::Pointer tissueSurface = TissueSurfaceFilterType::New();

	tissueSurface->SetInput(reader->GetOutput());
	writer->SetInput(tissueSurface->GetOutput());
	writer->SetFileName(argv[2]);
	writer->Update();
}
