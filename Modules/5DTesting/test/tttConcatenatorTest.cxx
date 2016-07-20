/*
 * tttConcatenatorTest.cxx
 *
 *  Created on: May 27, 2016
 *      Author: morgan
 */



#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkNumericSeriesFileNames.h"

int main(int argc,char ** argv){

	typedef itk::Image<float,4> ImageType;

	typedef itk::ImageSeriesReader< ImageType >  ReaderType;
	typedef itk::ImageFileWriter<   ImageType >  WriterType;

	typedef itk::NumericSeriesFileNames NameGeneratorType;

	NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();

	nameGenerator->SetSeriesFormat(argv[1]);

	nameGenerator->SetStartIndex( atoi(argv[2] ));
	nameGenerator->SetEndIndex( atoi(argv[3] ));
	nameGenerator->SetIncrementIndex( 1 );

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileNames(nameGenerator->GetFileNames());

	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(reader->GetOutput());
	writer->SetFileName(argv[4]);
	writer->Update();
}
