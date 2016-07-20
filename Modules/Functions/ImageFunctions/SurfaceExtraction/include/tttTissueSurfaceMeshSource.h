/*
 * TissueSurfaceMeshSource.h
 *
 *  Created on: Jul 8, 2015
 *      Author: morgan
 */

#ifndef TISSUESURFACEMESHSOURCE_H_
#define TISSUESURFACEMESHSOURCE_H_


#include "itkImageToMeshFilter.h"
#include "itkGradientImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkLineCell.h"
#include "itkDerivativeImageFilter.h"
#include "itkDefaultStaticMeshTraits.h"
#include "itkMesh.h"

namespace ttt{
template<class TInputImage,class TOutputMesh> class TissueSurfaceMeshSource : public itk::ImageToMeshFilter<TInputImage,TOutputMesh>{
		//public itk::ImageToMeshFilter<TInputImage,
		//itk::Mesh<typename TInputImage::PixelType,TInputImage::ImageDimension,
		//itk::DefaultStaticMeshTraits<typename TInputImage::PixelType, TInputImage::ImageDimension, TInputImage::ImageDimension -1,
		//float, double, typename TInputImage::PixelType > > > {
public:
	/** Standard "Self" typedef. */
	  typedef TissueSurfaceMeshSource                        Self;

	  typedef itk::SmartPointer< Self >                          Pointer;
	  typedef itk::SmartPointer< const Self >                    ConstPointer;

	  /** Method for creation through the object factory. */
	  itkNewMacro(Self);


	  /** Run-time type information (and related methods). */
	  itkTypeMacro(TissueSurfaceMeshSource, itk::ImageToMeshFilter);

	  typedef TInputImage						  InputImageType;

	  static const unsigned int NumDimensions = InputImageType::ImageDimension;


	  /** Hold on to the type information specified by the template parameters. */

	  typedef itk::DefaultStaticMeshTraits<
			  typename InputImageType::PixelType, NumDimensions, NumDimensions -1,
	            float, double, typename InputImageType::PixelType > MeshTraits;

	  //typedef itk::Mesh<typename TInputImage::PixelType,NumDimensions,MeshTraits>                         OutputMeshType;

	  typedef TOutputMesh OutputMeshType;

	  typedef OutputMeshType InputMeshType;
	  typedef typename OutputMeshType::MeshTraits OMeshTraits;
	  typedef typename OutputMeshType::PointType  OPointType;
	  typedef typename OMeshTraits::PixelType     OPixelType;


	  typedef itk::ImageToMeshFilter< TInputImage,OutputMeshType > Superclass;

	  typedef itk::CovariantVector<typename OutputMeshType::PixelType,NumDimensions> VectorType;
	  typedef itk::Mesh<VectorType,NumDimensions> InternalMeshType;

	  typedef itk::LineCell< typename InternalMeshType::CellType >        LineType;

	  itkSetObjectMacro(InputMesh,InputMeshType);

	  itkSetMacro(UseInitialMesh,bool)

protected:
	   TissueSurfaceMeshSource();
	   ~TissueSurfaceMeshSource();
	   void PrintSelf(std::ostream & os, itk::Indent indent) const ITK_OVERRIDE;

	   void GenerateData() ITK_OVERRIDE;

	   virtual void GenerateOutputInformation() ITK_OVERRIDE {}  // do nothing ITK_OVERRIDE
private:
	   TissueSurfaceMeshSource(const Self &); //purposely not implemented
	    void operator=(const Self &);         //purposely not implemented

	    void InitInternalMesh();
	    typename InternalMeshType::Pointer GenerateNewMesh(typename InternalMeshType::Pointer & mesh,const std::map<unsigned long,float>,float stepSize);
	    void UpdateOutputMesh();
	    double Evaluate(const typename InternalMeshType::Pointer & mesh);
	    void ComputeGradient(typename InternalMeshType::Pointer & mesh);
	    bool StoppingCriteria();

	    typedef itk::DerivativeImageFilter<TInputImage,TInputImage> GradientImageFilterType;

	    void XStep();
	    void ZStep();
	    void LambdaStep();

	    double Eval(const typename InternalMeshType::Pointer & mesh);
	    typename GradientImageFilterType::Pointer m_FirstGradientFilter;
	    typename GradientImageFilterType::Pointer m_SecondGradientFilter;

	    typename GradientImageFilterType::OutputImageType::Pointer m_FirstGradientImage;
	    typename GradientImageFilterType::OutputImageType::Pointer m_SecondGradientImage;

	    unsigned long m_NumberOfPoints;
	    unsigned long m_NumberOfCells;
	    double m_ObjectiveFunction;
	    unsigned int m_CurrentIteration;
	    unsigned int m_NumberOfIterations;
	    float m_Lambda;

	    float m_Alpha;

	    float m_Rho;

	    typedef itk::LinearInterpolateImageFunction<TInputImage, float >  InterpolatorType;

	    typedef itk::LinearInterpolateImageFunction< typename GradientImageFilterType::OutputImageType, float >  GradientInterpolatorType;


	    typename InterpolatorType::Pointer m_Interpolator;
	    typename GradientInterpolatorType::Pointer m_FirstGradientInterpolator;
	    typename GradientInterpolatorType::Pointer m_SecondGradientInterpolator;
	    typename InternalMeshType::Pointer m_InternalMesh;

	    typename InputMeshType::Pointer m_InputMesh;
	    bool m_UseInitialMesh;

	    std::vector<double> m_XSupport;
	    std::vector<double> m_LambdaSupport;

	    std::map<unsigned long,std::map<unsigned long,double>> m_XReg;
	    std::map<unsigned long,std::map<unsigned long,double>> m_LambdaReg;

};
}
#include "tttTissueSurfaceMeshSource.hxx"

#endif /* TISSUESURFACEMESHSOURCE_H_ */
