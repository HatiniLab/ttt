/*
 * tttAdvectiveDiffusionImageFilter.h
 *
 *  Created on: May 11, 2016
 *      Author: morgan
 */

#ifndef MODULES_FUNCTIONS_IMAGEFUNCTIONS_ADVECTIONDIFFUSION_INCLUDE_TTTADVECTIVEDIFFUSIONIMAGEFILTER_H_
#define MODULES_FUNCTIONS_IMAGEFUNCTIONS_ADVECTIONDIFFUSION_INCLUDE_TTTADVECTIVEDIFFUSIONIMAGEFILTER_H_

#include "itkImageToImageFilter.h"

#include "../../AdvectionDiffusion/include/tttAdvectiveDiffusionIterationImageFilter.h"

namespace ttt{
template<class TInputImage,class TOutputImage> class AdvectiveDiffusionImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>{
public:
	typedef AdvectiveDiffusionImageFilter<TInputImage,TOutputImage> Self;
	typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro(Self);
protected:
	AdvectiveDiffusionImageFilter(){
		m_NumberOfIterations=100;

	}
	virtual void GenerateData(){
		typedef ttt::AdvectiveDiffusionIterationImageFilter<TInputImage,TOutputImage> AdvectiveDiffusionIterationFilter;

		this->GetOutput()->Graft(this->GetInput());
		typename AdvectiveDiffusionIterationFilter::Pointer advectiveDiffusionIterationFilter =AdvectiveDiffusionIterationFilter::New();

		for(int it=0;it<m_NumberOfIterations;it++){

			advectiveDiffusionIterationFilter->SetInput(this->GetOutput());
			advectiveDiffusionIterationFilter->Modified();
			advectiveDiffusionIterationFilter->Update();
			this->GetOutput()->Graft(advectiveDiffusionIterationFilter->GetOutput());
			std::cout << it << std::endl;
		}
	}
private:
	unsigned m_NumberOfIterations;
};
}


#endif /* MODULES_FUNCTIONS_IMAGEFUNCTIONS_ADVECTIONDIFFUSION_INCLUDE_TTTADVECTIVEDIFFUSIONIMAGEFILTER_H_ */
