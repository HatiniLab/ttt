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
#ifndef __itkHessianSmoothed3DToObjectnessMeasureImageFilter_h
#define __itkHessianSmoothed3DToObjectnessMeasureImageFilter_h

#include "itkSymmetricSecondRankTensor.h"
#include "itkSymmetricEigenAnalysisImageFilter.h"
#include "tttObjectnessMeasurementFunction.h"
namespace ttt
{
/** \class HessianSmoothed3DToVesselnessMeasureImageFilter
 * \brief A filter to enhance 3D vascular structures 
 * 
 * The vesselness measure is based on the analysis of the the Hessian 
 * eigen system. The vesseleness function is a smoothed (continuous) 
 * version of the Frang's vesselness function. The filter takes an 
 * image of a Hessian pixels ( SymmetricSecondRankTensor pixels ) and
 * produces an enhanced image. The Hessian input image can be produced using 
 * itkHessianSmoothedRecursiveGaussianImageFilter. 
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
  
template <class TObjectnessMeasurementFunction,typename  TPixel > class HessianSmoothed3DToObjectnessMeasureImageFilter :
		public ImageToImageFilter< Image< SymmetricSecondRankTensor< double, 3 >, 3 >,
																	Image< TPixel, 3 > >
{
public:
  /** Standard class typedefs. */
  typedef HessianSmoothed3DToObjectnessMeasureImageFilter Self;

  typedef ImageToImageFilter< 
          Image< SymmetricSecondRankTensor< double, 3 >, 3 >, 
          Image< TPixel, 3 > >                 Superclass;

  typedef SmartPointer<Self>                   Pointer;
  typedef SmartPointer<const Self>             ConstPointer;
  
  typedef typename Superclass::InputImageType            InputImageType;
  typedef typename Superclass::OutputImageType           OutputImageType;
  typedef typename InputImageType::PixelType             InputPixelType;
  typedef TPixel                                         OutputPixelType;
  
  typedef TObjectnessMeasurementFunction ObjectnessMeasurementFunctionType;

  /** Image dimension = 3. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                    InputImageType::ImageDimension);

  itkStaticConstMacro(InputPixelDimension, unsigned int,
                    InputPixelType::Dimension);

  typedef  FixedArray< double, itkGetStaticConstMacro(InputPixelDimension) >
                                                          EigenValueArrayType;
  typedef  Image< EigenValueArrayType, itkGetStaticConstMacro(ImageDimension) >
                                                          EigenValueImageType;
  typedef   SymmetricEigenAnalysisImageFilter< 
            InputImageType, EigenValueImageType >     EigenAnalysisFilterType;

  typedef Image< TPixel, 3 > OuputImageType;
  typedef typename OutputImageType::RegionType OutputRegionType;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  itkSetObjectMacro(ObjectnessMeasurementFunction,ObjectnessMeasurementFunctionType);
#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(DoubleConvertibleToOutputCheck,
                  (Concept::Convertible<double, OutputPixelType>));
  /** End concept checking */
#endif

protected:
  HessianSmoothed3DToObjectnessMeasureImageFilter();
  ~HessianSmoothed3DToObjectnessMeasureImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;
  virtual void EnlargeOutputRequestedRegion(DataObject *output);
  virtual void GenerateInputRequestedRegion();
  virtual void BeforeThreadedGenerateData();
  /** Generate Data */
  virtual void ThreadedGenerateData(const OutputRegionType& windowRegion, ThreadIdType threadId);
  virtual void AfterThreadedGenerateData();
private:

  //purposely not implemented
  HessianSmoothed3DToObjectnessMeasureImageFilter(const Self&);

  void operator=(const Self&); //purposely not implemented

  typename EigenAnalysisFilterType::Pointer     m_SymmetricEigenValueFilter;
  typename ObjectnessMeasurementFunctionType::Pointer m_ObjectnessMeasurementFunction;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "tttHessianSmoothed3DToObjectnessMeasureImageFilter.hxx"
#endif
  
#endif
/** @}*/
