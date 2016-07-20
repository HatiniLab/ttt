/*
 * tttHierarchicalTessellationImageToAJGraphFilter.cpp
 *
 *  Created on: Jul 11, 2016
 *      Author: morgan
 */


#include "tttHierarchicalTessellationImageToAJGraphFilter.h"

#include "tttAJGraph.h"
#include "tttAJVertex.h"
#include "tttAJEdge.h"
#include "tttAJGraphFileWriter.h"
#include <itkImage.h>
#include <itkImageFileReader.h>
int main(int argc, char ** argv){

	typedef itk::Image<float,2> InputImageType;

	typedef itk::ImageFileReader<InputImageType> ReaderType;

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);

	typedef ttt::AJGraph<ttt::AJVertex<2>,ttt::AJEdge> AJGraphType;

	typedef ttt::HierarchicalTessellationImageToAJGraphFilter<InputImageType,AJGraphType> HTImageToAJGraphFilterType;

	HTImageToAJGraphFilterType::Pointer htImageToAJGraphFilter = HTImageToAJGraphFilterType::New();
	htImageToAJGraphFilter->SetInput(reader->GetOutput());


	typedef ttt::AJGraphFileWriter<AJGraphType> WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(argv[2]);
	writer->SetInput(htImageToAJGraphFilter->GetOutput());
	writer->Update();


}
