/*
 * TissueSurfaceMeshSourceTest.cpp
 *
 *  Created on: Jul 8, 2015
 *      Author: morgan
 */

#include "itkMesh.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMeshFileWriter.h"
#include "itkMeshFileReader.h"
#include "itkDiscreteGaussianImageFilter.h"
#include <itkLaplacianImageFilter.h>
#include "itkOtsuThresholdImageFilter.h"
#include "itkBinaryMask3DMeshSource.h"
#include "itkMultiplyImageFilter.h"

int main(int argc,char **argv){
	if( argc < 4 ) {
		std::cerr << "Usage: "<< std::endl;
	    std::cerr << argv[0];
	    std::cerr << " <InputFileName> <OutputMesh> <OutputSurface> ";
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
	  typedef itk::Mesh<double,3> MeshType;

	  typedef itk::ImageFileReader< ImageType >  ReaderType;
	  ReaderType::Pointer reader = ReaderType::New();
	  reader->SetFileName( inputFileName );

	  typedef itk::DiscreteGaussianImageFilter<ImageType,ImageType> BlurFilterType;

	  BlurFilterType::Pointer blurFilter = BlurFilterType::New();
	  blurFilter->SetUseImageSpacingOff();
	  blurFilter->SetInput(reader->GetOutput());
	  blurFilter->SetVariance(10);


	  typedef itk::OtsuThresholdImageFilter<ImageType,ImageType> ThresholdFilterType;

	  ThresholdFilterType::Pointer thresholdFilter = ThresholdFilterType::New();
	  thresholdFilter->SetInput(blurFilter->GetOutput());
	  thresholdFilter->SetInsideValue( 0.0 );
	  thresholdFilter->SetOutsideValue( 1.0 );

	  typedef itk::MultiplyImageFilter<ImageType,ImageType> MultiplyImageType;
	  MultiplyImageType::Pointer multiplyFilter = MultiplyImageType::New();
	  multiplyFilter->SetInput1(thresholdFilter->GetOutput());
	  multiplyFilter->SetInput2(reader->GetOutput());

#if 0

	  typedef itk::LaplacianImageFilter<ImageType,ImageType> EdgeDetectionType;
	  EdgeDetectionType::Pointer edgeDetectionFilter = EdgeDetectionType::New();
	  edgeDetectionFilter->SetInput(thresholdFilter->GetOutput());
#endif


	  typedef itk::ImageFileWriter<ImageType> ImageWriterType;
	  ImageWriterType::Pointer thresholdedImageWriter = ImageWriterType::New();
	  thresholdedImageWriter->SetInput(multiplyFilter->GetOutput());
	  thresholdedImageWriter->SetFileName("thresholded.mha");
	  thresholdedImageWriter->Update();


	  typedef itk::BinaryMask3DMeshSource<ImageType,MeshType> MeshExtractorType;

	  MeshExtractorType::Pointer meshExtractor = MeshExtractorType::New();

	  meshExtractor->SetInput(multiplyFilter->GetOutput());

	  typedef itk::MeshFileWriter< MeshType > WriterType;
	  WriterType::Pointer writer = WriterType::New();
	  writer->SetFileName( outputMeshFileName );
	  writer->SetInput( meshExtractor->GetOutput() );

	  writer->Update();
	  MeshType::Pointer outputMesh = meshExtractor->GetOutput();




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
		 outputImage->SetPixel(index,reader->GetOutput()->GetPixel(index));
		 //outputImage->SetPixel(index,1.0);
    	++pointIterator;                                // advance to next point
	 }

	 typedef itk::ImageFileWriter< ImageType >  ImageWriterType;
	 ImageWriterType::Pointer imageWriter = ImageWriterType::New();
	 imageWriter->SetFileName( outputSurfaceFileName );
	 imageWriter->SetInput(outputImage);
	 imageWriter->Update();

}

