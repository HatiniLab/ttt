//    This file is part of TTT Tissue Tracker.
//
//    TTT Tissue Tracker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    TTT Tissue Tracker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with TTT Tissue Tracker.  If not, see <http://www.gnu.org/licenses/>.

/** \addtogroup TTTObjectnessMeasurementBase
 *  @{
 */
#ifndef __itkMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter_h
#define __itkMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "tttHessianSmoothed3DToObjectnessMeasureImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "tttObjectnessMeasurementFunction.h"
namespace ttt
{
/**\class MultiScaleHessianSmoothed3DToVesselnessMeasureImageFilter
 * \brief A filter to enhance 3D vascular structures using Hessian 
 *         eigensystem in a multiscale framework
 * 
 * The vesselness measure is based on the analysis of the the Hessian 
 * eigen system. The vesseleness function is a smoothed (continous) 
 * version of the Frang's vesselness function. The filter takes an 
 * image of any pixel type and generates a Hessian image pixels at different
 * scale levels. The vesselness measure is computed from the Hessian image 
 * at each scale level and the best response is selected.  The vesselness 
 * measure is computed using HessianSmoothed3DToVesselnessMeasureImageFilter.
 *
 * Minimum and maximum sigma value can be set using SetMinSigma and SetMaxSigma
 * methods respectively. The number of scale levels is set using 
 * SetNumberOfSigmaSteps method. Exponentially distributed scale levels are 
 * computed within the bound set by the minimum and maximum sigma values 
 *  
 *
 * \par References
 *  Manniesing, R, Viergever, MA, & Niessen, WJ (2006). Vessel Enhancing 
 *  Diffusion: A Scale Space Representation of Vessel Structures. Medical 
 *  Image Analysis, 10(6), 815-825. 
 * 
 * \sa MultiScaleHessianSmoothed3DToVesselnessMeasureImageFilter 
 * \sa Hessian3DToVesselnessMeasureImageFilter
 * \sa HessianSmoothedRecursiveGaussianImageFilter 
 * \sa SymmetricEigenAnalysisImageFilter
 * \sa SymmetricSecondRankTensor
 * 
 * \ingroup IntensityImageFilters TensorObjects
 *
 */
template <class TObjectnessMeasurementFunction,class TInputImage, class TOutputImage = TInputImage >
class ITK_EXPORT MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter
: public
ImageToImageFilter< TInputImage,TOutputImage > 
{
public:
  /** Standard class typedefs. */
  typedef MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>              Superclass;

  typedef SmartPointer<Self>                                      Pointer;
  typedef SmartPointer<const Self>                                ConstPointer;

  
  typedef TInputImage                                    InputImageType;
  typedef TOutputImage                                   OutputImageType;

  typedef typename TInputImage::PixelType                InputPixelType;
  typedef typename TOutputImage::PixelType               OutputPixelType;

  /** Update image buffer that holds the best vesselness response */ 
  typedef Image< double, 3>                              UpdateBufferType;

  typedef TObjectnessMeasurementFunction ObjectnessMeasurementFunctionType;

  typedef HessianSmoothed3DToObjectnessMeasureImageFilter<ObjectnessMeasurementFunctionType, double > ObjectnessFilterType;

  /** Image dimension = 3. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                   InputImageType::ImageDimension);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Set/Get macros for Alpha */
  itkSetMacro(SigmaMin, double);
  itkGetMacro(SigmaMin, double);
  
  /** Set/Get macros for Beta */
  itkSetMacro(SigmaMax, double);
  itkGetMacro(SigmaMax, double);

  /** Set/Get macros for Number of Scales */
  itkSetMacro(NumberOfSigmaSteps, int);
  itkGetMacro(NumberOfSigmaSteps, int);



  void SetObjectnessMeasurementFunction(const typename ObjectnessMeasurementFunctionType::Pointer & objectnessMeasurementFunction){
	  m_ObjectnessFilter->SetObjectnessMeasurementFunction(objectnessMeasurementFunction);
  }


protected:
  MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter();
  ~MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  typedef HessianRecursiveGaussianImageFilter< InputImageType >
                                                        HessianFilterType;



  /** Generate Data */
  void GenerateData( void );

private:
  void UpdateMaximumResponse();

  double ComputeSigmaValue( int scaleLevel );
  
  void   AllocateUpdateBuffer();

  //purposely not implemented
  MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter(const Self&);
  void operator=(const Self&); //purposely not implemented

  double                                            m_SigmaMin;
  double                                            m_SigmaMax;

  int                                               m_NumberOfSigmaSteps;

  typename ObjectnessFilterType::Pointer           	m_ObjectnessFilter;
  typename HessianFilterType::Pointer               m_HessianFilter;

  UpdateBufferType::Pointer                         m_UpdateBuffer;


};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "tttMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter.hxx"
#endif
  
#endif
/** @}*/
