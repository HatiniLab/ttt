
#include <itkImageFileReader.h>

#include "tttAJSegmentationDijkstraFilter.h"
#include "tttAJGraphFileReader.h"
#include "tttAJGraphFileWriter.h"
#include "tttAJGraph.h"
#include "tttAJVertex.h"
#include "tttAJEdge.h"

int main(){

	typedef itk::Image<float,3> PlatenessImageType;
	typedef itk::Image<float,3> VertexnessImageType;

	typedef itk::ImageFileReader<PlatenessImageType> PlatenessReaderType;
	typedef itk::ImageFileReader<VertexnessImageType> VertexnessReaderType;
	typedef ttt::AJGraph<ttt::AJVertex<3>,ttt::AJEdge> AJGraphType;

	typedef ttt::AJSegmentationDijkstraFilter<AJGraphType,PlatenessImageType,VertexnessImageType,AJGraphType> AJGraphEdgeInitializer;

	typedef ttt::AJGraphFileReader<AJGraphType> AJGraphReaderType;
	typedef ttt::AJGraphFileWriter<AJGraphType> AJGraphWriterType;

	AJGraphReaderType::Pointer ajgraphReader = AJGraphReaderType::New();

	std::string inputAJGraphFileName;
	std::string outputAJGraphFileName;
	std::string platenessFileName;
	std::string vertexnessFileName;

	ajgraphReader->SetFileName(inputAJGraphFileName);

	PlatenessReaderType::Pointer platenessReader = PlatenessReaderType::New();
	platenessReader->SetFileName(platenessFileName);

	VertexnessReaderType::Pointer vertexnessReader = VertexnessReaderType::New();
	vertexnessReader->SetFileName(vertexnessFileName);

	AJGraphEdgeInitializer::Pointer ajgraphEdgeInitializer =  AJGraphEdgeInitializer::New();

	AJGraphWriterType::Pointer ajgraphWriter = AJGraphWriterType::New();
	ajgraphWriter->SetFileName(outputAJGraphFileName);


	ajgraphEdgeInitializer->SetPlatenessImage(platenessReader->GetOutput());
	ajgraphEdgeInitializer->SetVertexnessImage(vertexnessReader->GetOutput());
	ajgraphEdgeInitializer->SetInputAJGraph(ajgraphReader->GetOutput());



	ajgraphWriter->SetInput(ajgraphEdgeInitializer->GetOutput());
	ajgraphWriter->Update();


}
