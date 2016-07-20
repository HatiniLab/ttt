/*
 * tttPyramidSurfaceMeshSource.h
 *
 *  Created on: Jan 27, 2016
 *      Author: morgan
 */

#ifndef TTTPYRAMIDSURFACEMESHSOURCE_H_
#define TTTPYRAMIDSURFACEMESHSOURCE_H_
#include "itkImageToMeshFilter.h"
#include "itkRecursiveMultiResolutionPyramidImageFilter.h"
#include "tttTissueSurfaceMeshSource.h"
namespace ttt {

template<class TInputImage,class TOutputMesh> class PyramidSurfaceMeshSource:
		public itk::ImageToMeshFilter<TInputImage,TOutputMesh>{
		//public itk::ImageToMeshFilter<TInputImage,
		//itk::Mesh<typename TInputImage::PixelType, TInputImage::ImageDimension,
		//		itk::DefaultStaticMeshTraits<typename TInputImage::PixelType, TInputImage::ImageDimension, TInputImage::ImageDimension - 1, float,
		//				double, typename TInputImage::PixelType> > > {
public:
	/** Standard "Self" typedef. */
	typedef PyramidSurfaceMeshSource Self;

	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Method for creation through the object factory. */
	itkNewMacro (Self);

	/** Run-time type information (and related methods). */
	itkTypeMacro(PyramidSurfaceMeshSource, itk::ImageToMeshFilter);

	typedef TInputImage InputImageType;

	static const unsigned int NumDimensions = InputImageType::ImageDimension;

	/** Hold on to the type information specified by the template parameters. */

	typedef itk::DefaultStaticMeshTraits<typename InputImageType::PixelType, NumDimensions, NumDimensions - 1, float, double,
			typename InputImageType::PixelType> MeshTraits;

	//typedef itk::Mesh<typename TInputImage::PixelType, NumDimensions, MeshTraits> OutputMeshType;
	typedef itk::Mesh<typename TInputImage::PixelType, NumDimensions> OutputMeshType;

	typedef OutputMeshType InputMeshType;
	typedef typename OutputMeshType::MeshTraits OMeshTraits;
	typedef typename OutputMeshType::PointType OPointType;
	typedef typename OMeshTraits::PixelType OPixelType;

	typedef itk::ImageToMeshFilter<TInputImage, OutputMeshType> Superclass;

	typedef itk::CovariantVector<typename OutputMeshType::PixelType, NumDimensions> VectorType;
	typedef itk::Mesh<VectorType, NumDimensions> InternalMeshType;

	typedef itk::LineCell<typename InternalMeshType::CellType> LineType;

protected:
	PyramidSurfaceMeshSource();
	~PyramidSurfaceMeshSource();
	void PrintSelf(std::ostream & os, itk::Indent indent) const
	ITK_OVERRIDE;

	void GenerateData()
	ITK_OVERRIDE;

	virtual void GenerateOutputInformation() ITK_OVERRIDE {}  // do nothing ITK_OVERRIDE
private:
	PyramidSurfaceMeshSource(const Self &); //purposely not implemented
	void operator=(const Self &);         //purposely not implemented

	typedef itk::GradientImageFilter<TInputImage> GradientImageFilterType;

	typename GradientImageFilterType::Pointer m_GradientFilter;

	typename GradientImageFilterType::OutputImageType::Pointer m_GradientImage;

	float m_Lambda;
	float m_LambdaL1;
	float m_Alpha;

	typedef itk::RecursiveMultiResolutionPyramidImageFilter<InputImageType, InputImageType> PyramidFilterType;

	typename PyramidFilterType::Pointer m_PyramidFilter;

	unsigned int m_NumberOfLevels;

	typedef ttt::TissueSurfaceMeshSource<InputImageType,TOutputMesh> SurfaceFittingType;

	typename SurfaceFittingType::Pointer m_SurfaceFittingFilter;

};
}
#include "tttPyramidSurfaceMeshSource.hxx"

#endif /* MODULES_FUNCTIONS_IMAGEFUNCTIONS_SURFACEEXTRACTION_INCLUDE_TTTPYRAMIDSURFACEMESHSOURCE_H_ */
