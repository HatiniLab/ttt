/*
 * TissueSurfaceMeshSourceTest.cpp
 *
 *  Created on: Jul 8, 2015
 *      Author: morgan
 */

#include "itkMesh.h"
#include "itkImage.h"
#include "itkGradientImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMeshFileWriter.h"
#include "itkMeshFileReader.h"

#include "../include/tttTissueSurfaceMeshSource.h"
int main(int argc,char **argv){
	if( argc < 4 ) {
		std::cerr << "Usage: "<< std::endl;
	    std::cerr << argv[0];
	    std::cerr << " <InputFileName> <OutputMesh> <OutputSurface> [initialMesh]";
	    std::cerr << std::endl;
	    return EXIT_FAILURE;
	}

	  const char * inputFileName = argv[1];
	  const char * outputMeshFileName = argv[2];
	  const char * outputSurfaceFileName = argv[3];
	  bool useInitialMesh = argc>4;
	  const char * initialMeshFileName;
	  if(useInitialMesh){
		  initialMeshFileName = argv[4];
	  }


	  typedef itk::Image<float,3> ImageType;


	  typedef itk::ImageFileReader< ImageType >  ReaderType;
	  ReaderType::Pointer reader = ReaderType::New();
	  reader->SetFileName( inputFileName );


	  typedef itk::Mesh<float,3> MeshType;
	  typedef ttt::TissueSurfaceMeshSource<ImageType,MeshType> FilterType;




	  FilterType::Pointer meshFilter = FilterType::New();
	  meshFilter->SetInput(reader->GetOutput());

	  if(useInitialMesh){
		  typedef itk::MeshFileReader<typename FilterType::InputMeshType> MeshReaderType;
		  typename MeshReaderType::Pointer meshReader= MeshReaderType::New();
		  meshReader->SetFileName(initialMeshFileName);
		  meshReader->Update();
		  typename FilterType::InputMeshType::Pointer initialMesh = meshReader->GetOutput();
		  initialMesh->DisconnectPipeline();
		  meshFilter->SetInputMesh(initialMesh);
		  meshFilter->SetUseInitialMesh(true);
	  }




	  typedef itk::MeshFileWriter< MeshType > WriterType;

	  WriterType::Pointer writer = WriterType::New();
	  writer->SetFileName( outputMeshFileName );

	  writer->SetInput( meshFilter->GetOutput());
	  writer->Update();

	  MeshType::Pointer outputMesh = meshFilter->GetOutput();


	  ImageType::Pointer outputImage = ImageType::New();

	  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
	  outputImage->CopyInformation(reader->GetOutput());
	  outputImage->Allocate();
	  outputImage->FillBuffer(0.0);
	  // Access points
	  typedef typename MeshType::PointsContainer::Iterator     PointsIterator;

	  PointsIterator  pointIterator = outputMesh->GetPoints()->Begin();

	 PointsIterator end = outputMesh->GetPoints()->End();


	 while( pointIterator != end ){
		 typename MeshType::PointType p = pointIterator.Value();  // access the point

		 typename ImageType::IndexType index;
		 reader->GetOutput()->TransformPhysicalPointToIndex(p,index);
		 //outputImage->SetPixel(index,reader->GetOutput()->GetPixel(index));
		 outputImage->SetPixel(index,1.0);

    	++pointIterator;                                // advance to next point
	 }

	 typedef itk::ImageFileWriter< ImageType >  ImageWriterType;
	 ImageWriterType::Pointer imageWriter = ImageWriterType::New();
	 imageWriter->SetFileName( outputSurfaceFileName );
	 imageWriter->SetInput(outputImage);
	 imageWriter->Update();

}

