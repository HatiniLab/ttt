/*
 * tttTemporalAdvectionDiffusionImageFilter.h
 *
 *  Created on: May 31, 2016
 *      Author: morgan
 */

#ifndef MODULES_5DTESTING_INCLUDE_TTTTEMPORALADVECTIONDIFFUSIONIMAGEFILTER_H_
#define MODULES_5DTESTING_INCLUDE_TTTTEMPORALADVECTIONDIFFUSIONIMAGEFILTER_H_

#include <itkImageToImageFilter.h>
#include <itkExtractImageFilter.h>
//#include <itkWarpImageFilter.h>
#include <itkContinuousBorderWarpImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkImageFileWriter.h>

#include "tttAdvectionDiffusion2DIterationImageFilter.h"
namespace ttt{
template<class TInputImage,class TInputRegistration,class TOutputImage> class TemporalAdvectionDiffusionImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>{

public:
	typedef TemporalAdvectionDiffusionImageFilter<TInputImage,TInputRegistration,TOutputImage> Self;
	typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;
	itkNewMacro(Self);

	typedef TInputImage InputImageType;
	typedef typename TInputImage::RegionType InputRegionType;
	typedef typename TInputImage::IndexType InputIndexType;

	typedef TInputRegistration InputRegistrationImageType;


	typedef TOutputImage OutputImageType;
	typedef typename TOutputImage::RegionType OutputRegionType;
	typedef typename TOutputImage::IndexType OutputIndexType;

	typedef itk::Image<typename InputImageType::PixelType,InputImageType::ImageDimension-1> SliceImageType;
	typedef typename SliceImageType::IndexType SliceIndexType;

	typedef itk::Image<typename InputRegistrationImageType::PixelType,InputImageType::ImageDimension-1> SliceRegistrationType;

	virtual void SetInputImage(const TInputImage * input){
		this->SetNthInput(0,const_cast<TInputImage*>(input));
	}
	virtual typename TInputImage::ConstPointer  GetInputImage(){
		return static_cast<const TInputImage*>(this->itk::ProcessObject::GetInput(0));
	}
	virtual void SetForwardDisplacementField(const TInputRegistration * forward){
		this->SetNthInput(1,const_cast<TInputRegistration*>(forward));
	}
	virtual typename TInputRegistration::ConstPointer GetForwardDisplacementField(){
		return static_cast<const TInputRegistration*>(this->itk::ProcessObject::GetInput(1));

	}
	virtual void SetBackwardDisplacementField(const TInputRegistration * backward){
		this->SetNthInput(2,const_cast<TInputRegistration*>(backward));
	}
	virtual typename TInputRegistration::ConstPointer GetBackwardDisplacementField(){
		return static_cast<const TInputRegistration*>(this->itk::ProcessObject::GetInput(2));
	}

protected:
	TemporalAdvectionDiffusionImageFilter(){
		m_NumberOfIterations =50;
		m_TimeStep =0.025;
		this->SetNumberOfRequiredInputs(3);
	}

	virtual ~TemporalAdvectionDiffusionImageFilter(){

	}

	virtual void GenerateInputRequestedRegion() ITK_OVERRIDE{

		Superclass::GenerateInputRequestedRegion();
#if 0
		std::cout << "Input" << this->GetInput()->GetRequestedRegion() << std::endl;
		std::cout << "RequestedOutput" << this->GetOutput()->GetRequestedRegion() << std::endl;

		OutputRegionType outputRegion=this->GetOutput()->GetRequestedRegion();
		OutputRegionType largestOutputRegion =this->GetOutput()->GetRequestedRegion();

		InputRegionType requestedInputRegion = outputRegion;

		if(outputRegion.GetIndex(2)>0){
			requestedInputRegion.SetIndex(2,outputRegion.GetIndex(2)-1);
			requestedInputRegion.SetSize(2,outputRegion.GetSize(2)+1);
		}
		if(outputRegion.GetIndex(2)+outputRegion.GetSize(2)<largestOutputRegion.GetIndex(2)+largestOutputRegion.GetSize(2)){
			requestedInputRegion.SetSize(2,outputRegion.GetSize(2)+1);
		}
#endif
	}

	virtual void EnlargeOutputRequestedRegion(itk::DataObject * output) ITK_OVERRIDE{
#if 0
		TOutputImage * outputPointer = dynamic_cast<TOutputImage*>(output);

		auto requestedRegion=this->GetOutput()->GetRequestedRegion();
		auto largestPossibleRegion = this->GetOutput()->GetLargestPossibleRegion();

		requestedRegion.SetIndex(0,largestPossibleRegion.GetIndex(0));
		requestedRegion.SetIndex(1,largestPossibleRegion.GetIndex(1));

		requestedRegion.SetSize(0,largestPossibleRegion.GetSize(0));
		requestedRegion.SetSize(1,largestPossibleRegion.GetSize(1));

		outputPointer->SetRequestedRegion(requestedRegion);
#endif
	}

	virtual void GenerateData(){


		InputRegionType inputRegion = this->GetInput()->GetRequestedRegion();
		InputRegionType largestPossibleRegion = this->GetInput()->GetLargestPossibleRegion();

		unsigned int t0= inputRegion.GetIndex(2);
		unsigned int t1= inputRegion.GetIndex(2)+inputRegion.GetSize(2);

		typedef itk::ExtractImageFilter<InputImageType,SliceImageType> ExtractorType;
		typedef itk::ExtractImageFilter<InputRegistrationImageType,SliceRegistrationType> RegistrationExtractorType;


		typename ExtractorType::Pointer currentExtractor =ExtractorType::New();
		typename ExtractorType::Pointer previousExtractor =ExtractorType::New();
		typename ExtractorType::Pointer nextExtractor =ExtractorType::New();


		typename RegistrationExtractorType::Pointer forwardExtractor = RegistrationExtractorType::New();
		typename RegistrationExtractorType::Pointer backwardExtractor = RegistrationExtractorType::New();



		forwardExtractor->SetInput(this->GetBackwardDisplacementField());
		backwardExtractor->SetInput(this->GetForwardDisplacementField());

		typedef itk::CastImageFilter<InputImageType,OutputImageType> InputOutputCasterType;
		typename InputOutputCasterType::Pointer inputOutputCaster = InputOutputCasterType::New();
		inputOutputCaster->SetInput(this->GetInput());
		inputOutputCaster->Update();
		typename OutputImageType::Pointer currentEstimate=inputOutputCaster->GetOutput();
		currentEstimate->DisconnectPipeline();


		typename OutputImageType::Pointer nextEstimate;

		for(int it=0;it<m_NumberOfIterations;it++){

//1. Advection-diffusion

			previousExtractor->SetInput(currentEstimate);
			nextExtractor->SetInput(currentEstimate);
			currentExtractor->SetInput(currentEstimate);


			nextEstimate=OutputImageType::New();
			nextEstimate->CopyInformation(currentEstimate);
			nextEstimate->SetBufferedRegion(currentEstimate->GetBufferedRegion());
			nextEstimate->Allocate();

			for(int t=t0;t<t1;t++){
				std::cout << "T: " << t << std::endl;

				InputRegionType extractionRegion=this->GetInput()->GetRequestedRegion();
				extractionRegion.SetIndex(2,t);
				extractionRegion.SetSize(2,0);
				currentExtractor->SetExtractionRegion(extractionRegion);
				currentExtractor->SetDirectionCollapseToIdentity();


				typedef ttt::AdvectiveDiffusion2DIterationImageFilter<SliceImageType,SliceImageType> AdvectionDiffusionType;

				typename AdvectionDiffusionType::Pointer advectionDiffusion = AdvectionDiffusionType::New();
				advectionDiffusion->SetInput(currentExtractor->GetOutput());
				advectionDiffusion->SetTau(this->m_TimeStep);

				advectionDiffusion->Update();
				extractionRegion.SetSize(2,1);
				itk::ImageAlgorithm::Copy<SliceImageType,InputImageType>(advectionDiffusion->GetOutput(),nextEstimate,advectionDiffusion->GetOutput()->GetLargestPossibleRegion(),extractionRegion);


			}

			currentEstimate = nextEstimate;
			currentEstimate->DisconnectPipeline();
#if 1
			previousExtractor->SetInput(currentEstimate);
			nextExtractor->SetInput(currentEstimate);
			currentExtractor->SetInput(currentEstimate);



			for(int t=t0;t<t1;t++){

#ifdef STORE_WARPING
				typedef itk::ImageFileWriter<SliceImageType> WriterType;
				typename WriterType::Pointer writer = WriterType::New();
#endif



				InputRegionType extractionRegion=this->GetInput()->GetRequestedRegion();
				extractionRegion.SetIndex(2,t);
				extractionRegion.SetSize(2,0);
				currentExtractor->SetExtractionRegion(extractionRegion);
				currentExtractor->SetDirectionCollapseToIdentity();
				currentExtractor->Update();
#ifdef STORE_WARPING
				writer->SetFileName("current.mha");
				writer->SetInput(currentExtractor->GetOutput());
				writer->Update();
#endif
				if(t<largestPossibleRegion.GetIndex(2)+largestPossibleRegion.GetSize(2)-1){
					extractionRegion.SetIndex(2,t+1);
					extractionRegion.SetSize(2,0);

				}else{
					extractionRegion.SetIndex(2,t);
					extractionRegion.SetSize(2,0);
				}
				backwardExtractor->SetExtractionRegion(extractionRegion);
				backwardExtractor->SetDirectionCollapseToIdentity();
				nextExtractor->SetExtractionRegion(extractionRegion);
				nextExtractor->SetDirectionCollapseToIdentity();
				if(t>largestPossibleRegion.GetIndex(2)){
					extractionRegion.SetIndex(2,t-1);
					extractionRegion.SetSize(2,0);
				}else{
					extractionRegion.SetIndex(2,t);
					extractionRegion.SetSize(2,0);
				}
				forwardExtractor->SetExtractionRegion(extractionRegion);
				forwardExtractor->SetDirectionCollapseToIdentity();

				previousExtractor->SetExtractionRegion(extractionRegion);
				previousExtractor->SetDirectionCollapseToIdentity();

				typedef itk::ContinuousBorderWarpImageFilter<SliceImageType, SliceImageType, SliceRegistrationType> WarperType;


				//typedef itk::WarpImageFilter<SliceImageType,SliceImageType,SliceRegistrationType> WarperType;

				typename WarperType::Pointer nextWarper = WarperType::New();
				nextWarper->SetInput(nextExtractor->GetOutput());
				nextWarper->SetDisplacementField(backwardExtractor->GetOutput());
				nextWarper->SetOutputParametersFromImage(currentExtractor->GetOutput());
				nextWarper->Update();
#ifdef STORE_WARPING

				writer->SetFileName("next.mha");
				writer->SetInput(nextExtractor->GetOutput());
				writer->Update();

				writer->SetFileName("nextwarped.mha");
				writer->SetInput(nextWarper->GetOutput());
				writer->Update();
#endif
				typename WarperType::Pointer previousWarper = WarperType::New();
				previousWarper->SetInput(previousExtractor->GetOutput());
				previousWarper->SetDisplacementField(forwardExtractor->GetOutput());
				previousWarper->SetOutputParametersFromImage(currentExtractor->GetOutput());
				previousWarper->Update();

#ifdef STORE_WARPING
				writer->SetFileName("previous.mha");
				writer->SetInput(previousExtractor->GetOutput());
				writer->Update();

				writer->SetFileName("previousWarped.mha");
				writer->SetInput(previousWarper->GetOutput());
				writer->Update();

#endif
				typedef itk::ImageRegionIteratorWithIndex<SliceImageType> IteratorType;
				IteratorType iterator(currentExtractor->GetOutput(),currentExtractor->GetOutput()->GetLargestPossibleRegion());

				while(!iterator.IsAtEnd()){
					auto value = iterator.Value();
					SliceIndexType sliceIndex = iterator.GetIndex();

					auto nextValue = nextWarper->GetOutput()->GetPixel(sliceIndex);
					auto previousValue = previousWarper->GetOutput()->GetPixel(sliceIndex);

					auto alpha = this->m_TimeStep;
					auto value1= (1-2*alpha)*value + alpha*(nextValue + previousValue);

					OutputIndexType outputIndex;
					outputIndex[0]=sliceIndex[0];
					outputIndex[1]=sliceIndex[1];
					outputIndex[2]=t;

					nextEstimate->SetPixel(outputIndex,value1);
					++iterator;
				}

			}
			currentEstimate=nextEstimate;
			currentEstimate->DisconnectPipeline();

#endif
		}
		this->GetOutput()->Graft(nextEstimate);
	}

private:
	unsigned m_NumberOfIterations;
	double m_TimeStep;

};

}

#endif /* MODULES_5DTESTING_INCLUDE_TTTTEMPORALADVECTIONDIFFUSIONIMAGEFILTER_H_ */
