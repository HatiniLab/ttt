/*
 * tttTissueSurfaceImageFilter.h
 *
 *  Created on: May 25, 2016
 *      Author: morgan
 */

#ifndef MODULES_FUNCTIONS_IMAGEFUNCTIONS_SURFACEEXTRACTION_INCLUDE_TTTTISSUESURFACEIMAGEFILTER_H_
#define MODULES_FUNCTIONS_IMAGEFUNCTIONS_SURFACEEXTRACTION_INCLUDE_TTTTISSUESURFACEIMAGEFILTER_H_

#include <itkImageToImageFilter.h>
#include "itkLineCell.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkGradientImageFilter.h"
#include "itkDerivativeImageFilter.h"

#include "itkMesh.h"
#include "itkCovariantVector.h"
namespace ttt{

template<class TInputImage,class TOutputImage> class TissueSurfaceImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>{
public:
	typedef TissueSurfaceImageFilter<TInputImage,TOutputImage> Self;
	typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	static const unsigned int NumDimensions = TInputImage::ImageDimension;

	itkNewMacro(Self);
	itkTypeMacro(TissueSurfaceImageFilter,itk::ImageToImageFilter);

	typedef itk::CovariantVector<double,NumDimensions> VectorType;
	typedef itk::Mesh<VectorType,NumDimensions> InternalMeshType;

	typedef itk::LineCell< typename InternalMeshType::CellType >        LineType;
protected:
	TissueSurfaceImageFilter();

	virtual ~TissueSurfaceImageFilter();
	virtual void GenerateInputRequestedRegion() ITK_OVERRIDE;
	virtual void GenerateData() ITK_OVERRIDE;

    void ZStep();
    void XStep();
    void LambdaStep();
    double Eval(const typename InternalMeshType::Pointer & mesh);

private:

	TissueSurfaceImageFilter(const TissueSurfaceImageFilter &);
	void operator=(const  TissueSurfaceImageFilter &);
    void InitInternalMesh();
    bool StoppingCriteria();

    typedef itk::DerivativeImageFilter<TInputImage,TInputImage> GradientImageFilterType;


	typename GradientImageFilterType::Pointer m_FirstGradientFilter;
	typename GradientImageFilterType::Pointer m_SecondGradientFilter;

	typename GradientImageFilterType::OutputImageType::Pointer m_FirstGradientImage;
	typename GradientImageFilterType::OutputImageType::Pointer m_SecondGradientImage;

	typedef itk::LinearInterpolateImageFunction<TInputImage, float >  InterpolatorType;
    typedef itk::LinearInterpolateImageFunction< typename GradientImageFilterType::OutputImageType, float >  GradientInterpolatorType;

    typename InterpolatorType::Pointer m_Interpolator;
    typename GradientInterpolatorType::Pointer m_FirstGradientInterpolator;
    typename GradientInterpolatorType::Pointer m_SecondGradientInterpolator;

    typename InternalMeshType::Pointer m_InternalMesh;


    unsigned int m_NumberOfIterations;
	unsigned int m_CurrentIteration;
	unsigned int m_NumberOfPoints;
    unsigned long m_NumberOfCells;


    std::vector<double> m_XSupport;
    std::vector<double> m_LambdaSupport;

    std::map<unsigned long,std::map<unsigned long,double>> m_XReg;
    std::map<unsigned long,std::map<unsigned long,double>> m_LambdaReg;

    float m_Rho;
    float m_Lambda;


};

}

#include "tttTissueSurfaceImageFilter.hxx"

#endif /* MODULES_FUNCTIONS_IMAGEFUNCTIONS_SURFACEEXTRACTION_INCLUDE_TTTTISSUESURFACEIMAGEFILTER_H_ */
