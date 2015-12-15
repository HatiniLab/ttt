/*
 * tttPlatenessMeasurementFunctionTest.cxx
 *
 *  Created on: May 18, 2015
 *      Author: morgan
 */


#include <gtest/gtest.h>
#include "tttVertexnessMeasurementFunction.h"
#include "tttMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter.h"
template <typename TImage>

void CreateImage(const typename TImage::Pointer & image)
{
  // This function creates a 2D image consisting of a black background,
  // a large square of a non-zero pixel value, and a single "erroneous" pixel
  // near the square.
  typename TImage::IndexType corner = {{0,0,0}};

  typename TImage::SizeType size = {{20,20,20}};

  typename TImage::RegionType region(corner, size);


  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
  // Make a square
  for(int r = 4; r < 10; r++){
    for(int c = 4; c < 10; c++){
    	for(int h=4;h < 10;h++){
    		typename TImage::IndexType pixelIndex = {{r,c,h}};
    		image->SetPixel(pixelIndex, 1);
    	}
      }
    }

  //  typename TImage::IndexType pixelIndex = {{102, 102}};

//  image->SetPixel(pixelIndex, 50);
}

//int itkHessian3DToVesselnessMeasureImageFilterTest(int, char* [] )
TEST(VertexnessTest, Regular) {


  // Define the dimension of the images
  const unsigned int myDimension = 3;

  // Declare the types of the images
  typedef itk::Image<float, myDimension>           myImageType;

  // Declare the type of the index to access images
  typedef itk::Index<myDimension>             myIndexType;

  // Declare the type of the size
  typedef itk::Size<myDimension>              mySizeType;

  // Declare the type of the Region
  typedef itk::ImageRegion<myDimension>        myRegionType;

  // Create the image
  myImageType::Pointer inputImage  = myImageType::New();


  // Define their size, and start index
  mySizeType size;
  size[0] = 16;
  size[1] = 16;
  size[2] = 16;

  myIndexType start;
  start.Fill(0);

  myRegionType region;
  region.SetIndex( start );
  region.SetSize( size );

  // Initialize Image A
  inputImage->SetLargestPossibleRegion( region );
  inputImage->SetBufferedRegion( region );
  inputImage->SetRequestedRegion( region );
  inputImage->Allocate();

  // Declare Iterator type for the input image
  typedef itk::ImageRegionIteratorWithIndex<myImageType>  myIteratorType;

  // Create one iterator for the Input Image A (this is a light object)
  myIteratorType it( inputImage, inputImage->GetRequestedRegion() );

  // Initialize the content of Image A
   while( !it.IsAtEnd() )
     {
     it.Set( 0.0 );
     ++it;
     }

   size[0] = 1;
   size[1] = 8;
   size[2] = 1;

   start[0] = 3;
   start[1] = 0;
   start[2] = 3;

   // Create one iterator for an internal region
   region.SetSize( size );
   region.SetIndex( start );
   myIteratorType itb( inputImage, region );

   // Initialize the content the internal region
   while( !itb.IsAtEnd() )
     {
     itb.Set( 100.0 );
     ++itb;
     }

   typedef ttt::VertexnessMeasurementFunction<double> VertexnessFunctionType;
   typedef ttt::MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter<VertexnessFunctionType,myImageType> VertexnessFilterType;

   VertexnessFunctionType::Pointer vertexnessFunction = VertexnessFunctionType::New();

   VertexnessFilterType::Pointer filter = VertexnessFilterType::New();

   filter->SetInput(inputImage);
   filter->SetSigmaMin(0.0001);
   filter->SetSigmaMax(0.5);
   filter->SetNumberOfSigmaSteps(5);
   filter->SetObjectnessMeasurementFunction(vertexnessFunction);
   filter->Update();

   return EXIT_SUCCESS;
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
