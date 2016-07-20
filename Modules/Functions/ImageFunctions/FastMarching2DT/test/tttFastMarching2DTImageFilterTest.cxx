/*
 * FastMarching2DTImageFilterTest.cxx
 *
 *  Created on: Jun 21, 2016
 *      Author: morgan
 */


#include "tttFastMarching2DTImageFilter.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkBoundedReciprocalImageFilter.h>
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>
#include <itkSigmoidImageFilter.h>

#include <itkImage.h>

int main(int argc, char ** argv){

	typedef itk::Image<float,3> SpeedImageType;
	typedef itk::Image<float,3> LevelsetImageType;

	typedef itk::Image<itk::Vector<float,2>,3> RegistrationFieldType;

	typedef itk::ImageFileReader<SpeedImageType> SpeedReaderType;
	typedef itk::ImageFileReader<RegistrationFieldType> RegistrationReaderType;
	typedef itk::ImageFileWriter<LevelsetImageType> LevelsetWriterType;

	typename SpeedReaderType::Pointer speedReader = SpeedReaderType::New();

	typename RegistrationReaderType::Pointer forwardReader = RegistrationReaderType::New();
	typename RegistrationReaderType::Pointer backwardReader = RegistrationReaderType::New();

	speedReader->SetFileName(argv[1]);
	forwardReader->SetFileName(argv[2]);
	backwardReader->SetFileName(argv[3]);

	typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<SpeedImageType,SpeedImageType > GradientFilterType;
	typedef itk::SigmoidImageFilter<SpeedImageType,SpeedImageType > SigmoidFilterType;

	GradientFilterType::Pointer gradientMagnitude = GradientFilterType::New();
	SigmoidFilterType::Pointer sigmoid = SigmoidFilterType::New();


	gradientMagnitude->SetInput(speedReader->GetOutput());

	LevelsetWriterType::Pointer gradientMagnitudeWriter = LevelsetWriterType::New();
	gradientMagnitudeWriter->SetInput(gradientMagnitude->GetOutput());
	gradientMagnitudeWriter->SetFileName("gradientMagnitude.mha");
	gradientMagnitudeWriter->Update();

	sigmoid->SetInput(speedReader->GetOutput());
	double K1=0.1;
	double K2=0.005;
	sigmoid->SetAlpha((K2-K1)/6.0);
	sigmoid->SetBeta((K1+K2)/2);
	sigmoid->SetOutputMinimum( 0.0 );
	sigmoid->SetOutputMaximum( 1.0 );

	LevelsetWriterType::Pointer speedWriter = LevelsetWriterType::New();
	speedWriter->SetInput(sigmoid->GetOutput());
	speedWriter->SetFileName("speed.mha");
	speedWriter->Update();




	typedef ttt::FastMarching2DTImageFilter<SpeedImageType,RegistrationFieldType,LevelsetImageType> FastMarchingType;

	FastMarchingType::NodeContainerPointer nodeContainer = FastMarchingType::NodeContainer::New();

#if 0
	FastMarchingType::NodeIndexType index;
	index[0]=400;
	index[1]=115;
	index[2]=0;

	FastMarchingType::NodeType node;


	node.SetIndex(index);
	node.SetValue(0.0);
#endif
	nodeContainer->Initialize();
	//nodeContainer->InsertElement(0,node);

	FastMarchingType::NodeIndexType index2;
	index2[0]=220;
	index2[1]=120;
	index2[2]=0;


	FastMarchingType::NodeType node2;


	node2.SetIndex(index2);
	node2.SetValue(0.0);


	nodeContainer->InsertElement(0,node2);


	FastMarchingType::Pointer fastMarching = FastMarchingType::New();
	fastMarching->SetTrialPoints(nodeContainer);

	fastMarching->SetInput(sigmoid->GetOutput());
	fastMarching->SetForwardDisplacementField(forwardReader->GetOutput());
	fastMarching->SetBackwardDisplacementField(backwardReader->GetOutput());
	fastMarching->Update();


	LevelsetWriterType::Pointer levelsetWriter = LevelsetWriterType::New();

	levelsetWriter->SetInput(fastMarching->GetOutput());
	levelsetWriter->SetFileName(argv[4]);
	levelsetWriter->Update();

}
