/*
 * tttSurfaceExtractionImageFilter.cpp
 *
 *  Created on: Jul 20, 2016
 *      Author: morgan
 */

#include <itkSliceBySliceImageFilter.h>
#include <tttTissueSurfaceImageFilter.h>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
int main(int argc, char ** argv){

	if(argc < 3){
		std::cerr << "Usage: " << argv[0] << " <inputFile> <outputFile>" << std::endl;
	}
	typedef itk::Image<float,4> InputMovieType;
	typedef itk::Image<unsigned char,4> OutputMovieType;
	typedef itk::Image<float,3> InputFrameType;
	typedef itk::Image<unsigned char,3> OutputFrameType;

	typedef ttt::TissueSurfaceImageFilter<InputFrameType,OutputFrameType> SurfaceExtractionType;

	typedef itk::SliceBySliceImageFilter<InputMovieType,OutputMovieType,SurfaceExtractionType> FrameByFrameSurfaceExtractionType;


	typedef itk::ImageFileReader<InputMovieType> ReaderType;
	typedef itk::ImageFileWriter<OutputMovieType> WriterType;

	typename ReaderType::Pointer reader =ReaderType::New();
	typename WriterType::Pointer writer = WriterType::New();

	FrameByFrameSurfaceExtractionType::Pointer frameByFrameSurfaceExtractor = FrameByFrameSurfaceExtractionType::New();

	SurfaceExtractionType::Pointer surfaceExtractor = SurfaceExtractionType::New();

	frameByFrameSurfaceExtractor->SetFilter(surfaceExtractor);
	frameByFrameSurfaceExtractor->SetInput(reader->GetOutput());

	writer->SetInput(frameByFrameSurfaceExtractor->GetOutput());
	writer->Update();
}
