/*
 * tttStreamedTestFilter.h
 *
 *  Created on: May 23, 2016
 *      Author: morgan
 */

#ifndef MODULES_5DTESTING_INCLUDE_TTTSTREAMEDTESTFILTER_H_
#define MODULES_5DTESTING_INCLUDE_TTTSTREAMEDTESTFILTER_H_

#include <itkStreamingImageFilter.h>
#include <itkSimpleFastMutexLock.h>
#include <itkExtractImageFilter.h>
#include <tttTissueSurfaceImageFilter.h>

namespace ttt{

template<class TInputImage,class TOutputImage> class StreamedTestFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>{

public:
	typedef StreamedTestFilter<TInputImage,TOutputImage> Self;
	typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef TInputImage InputImageType;
	typedef TOutputImage OutputImageType;
	typedef typename TInputImage::RegionType InputRegionType;
	typedef typename TInputImage::Pointer InputImagePointer;
	typedef typename TOutputImage::RegionType OutputRegionType;

	typedef itk::Image<typename TInputImage::PixelType,TInputImage::ImageDimension-1> InternalImageType;

	itkNewMacro(Self)
protected:
	StreamedTestFilter(){

	}
	virtual ~StreamedTestFilter(){

	}
#if 1

	virtual void EnlargeOutputRequestedRegion(itk::DataObject * output) ITK_OVERRIDE{
		Superclass::EnlargeOutputRequestedRegion(output);

		TOutputImage * outputPointer = dynamic_cast<TOutputImage*>(output);

		auto requestedRegion=this->GetOutput()->GetRequestedRegion();
		auto largestPossibleRegion = this->GetOutput()->GetLargestPossibleRegion();

		requestedRegion.SetIndex(0,largestPossibleRegion.GetIndex(0));
		requestedRegion.SetIndex(1,largestPossibleRegion.GetIndex(1));
		requestedRegion.SetIndex(2,largestPossibleRegion.GetIndex(2));

		requestedRegion.SetSize(0,largestPossibleRegion.GetSize(0));
		requestedRegion.SetSize(1,largestPossibleRegion.GetSize(1));
		requestedRegion.SetSize(2,largestPossibleRegion.GetSize(2));

		outputPointer->SetRequestedRegion(requestedRegion);

	}

	virtual void GenerateInputRequestedRegion() ITK_OVERRIDE{

		Superclass::GenerateInputRequestedRegion();

		std::cout << "Input" << this->GetInput()->GetRequestedRegion() << std::endl;
		std::cout << "RequestedOutput" << this->GetOutput()->GetRequestedRegion() << std::endl;

	}
#endif
//	virtual void ThreadedGenerateData (const OutputRegionType &outputRegionForThread, itk::ThreadIdType threadId){
	virtual void GenerateData(){

		TOutputImage  * output = this->GetOutput();

		output->SetBufferedRegion(output->GetRequestedRegion());
		output->Allocate();
		std::cout << "Output" << std::endl;
		std::cout << output << std::endl;

		typedef itk::ExtractImageFilter<InputImageType,InternalImageType> ExtractImageFilterType;
		typename ExtractImageFilterType::Pointer extractor = ExtractImageFilterType::New();
		typedef typename InputImageType::RegionType RegionType;

		RegionType extractionRegion;
		RegionType requestedRegion=this->GetOutput()->GetRequestedRegion();

		extractor->SetInput(this->GetInput());

		typedef ttt::TissueSurfaceImageFilter<InternalImageType,InternalImageType> TissueSurfaceFilter;
		typename TissueSurfaceFilter::Pointer tissueSurfaceFilter = TissueSurfaceFilter::New();

		for(int t=requestedRegion.GetIndex(3);t<requestedRegion.GetIndex(3)+requestedRegion.GetSize(3);t++){
			std::cout << "t" << t << std::endl;
			extractionRegion.SetIndex(0,requestedRegion.GetIndex(0));
			extractionRegion.SetIndex(1,requestedRegion.GetIndex(1));
			extractionRegion.SetIndex(2,requestedRegion.GetIndex(2));
			extractionRegion.SetIndex(3,t);

			extractionRegion.SetSize(0,requestedRegion.GetSize(0));
			extractionRegion.SetSize(1,requestedRegion.GetSize(1));
			extractionRegion.SetSize(2,requestedRegion.GetSize(2));
			extractionRegion.SetSize(3,0);

			std::cout << extractionRegion << std::endl;

			extractor->SetExtractionRegion(extractionRegion);
			extractor->SetDirectionCollapseToIdentity();



			tissueSurfaceFilter->SetInput(extractor->GetOutput());
			tissueSurfaceFilter->Update();

			const InternalImageType * extracted = tissueSurfaceFilter->GetOutput();

			itk::ImageAlgorithm::Copy(extracted,output,extracted->GetLargestPossibleRegion(),extractionRegion);

		}

		//this->GetInput()->SetRequestedRegionToLargestPossibleRegion();
		//this->GetInput()->Update();
#if 0
		InputImagePointer inputPtr = const_cast< TInputImage * >( this->GetInput() );

		InputRegionType requestedRegion = outputRegionForThread;
		InputRegionType largestPossibleRegion = this->GetInput()->GetLargestPossibleRegion();

		if(requestedRegion.GetIndex(3)>=1){
			requestedRegion.SetIndex(3,requestedRegion.GetIndex(3)-1);
		}

		if(requestedRegion.GetIndex(3)+requestedRegion.GetSize(3)+1<=largestPossibleRegion.GetSize(3)){
			requestedRegion.SetSize(3,requestedRegion.GetSize(3)+1);
		}
		inputPtr->SetRequestedRegion(requestedRegion);
		inputPtr->Update();
#endif
		//m_Lock.Lock();
		//std::cout << threadId <<  "Input" << this->GetInput()->GetRequestedRegion() << std::endl;
		//std::cout  <<  "Input" << this->GetInput()->GetRequestedRegion() << std::endl;
		//std::cout << threadId << "Output" <<  outputRegionForThread << std::endl;
		//std::cout << threadId << "Requested" << this->GetOutput()->GetRequestedRegion() << std::endl;
		//std::cout << "Requested" << this->GetOutput()->GetRequestedRegion() << std::endl;
		//m_Lock.Unlock();
		return;
	}

private:
	StreamedTestFilter(const StreamedTestFilter & other);
	itk::SimpleFastMutexLock m_Lock;

};

}


#endif /* MODULES_5DTESTING_INCLUDE_TTTSTREAMEDTESTFILTER_H_ */
