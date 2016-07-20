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

/** \addtogroup TTTAnisotropicDiffusion
 *  @{
 */
#ifndef __itkAnisotropicDiffusionVesselEnhancementImageFilter_h
#define __itkAnisotropicDiffusionVesselEnhancementImageFilter_h

//#include "itkFiniteDifferenceImageFilter.h"
#include "itkDenseFiniteDifferenceImageFilter.h"
#include "itkMultiThreader.h"
#include "itkDiffusionTensor3D.h"
#include "tttMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"

#include "../../AJEnhancementDiffusion/include/tttAnisotropicDiffusionVesselEnhancementFunction.h"
#include "itkSymmetricEigenVectorAnalysisImageFilter.h"
#include "tttPlatenessMeasurementFunction.h"

namespace ttt {
/** \class AnisotropicDiffusionPlateEnhancementFunction
 * \brief This class iteratively enhances vessels in an image by solving
 * non-linear diffusion equation developed by Manniesing et al.
 *
 * \par References
 *  Manniesing, R, Viergever, MA, & Niessen, WJ (2006). Vessel Enhancing 
 *  Diffusion: A Scale Space Representation of Vessel Structures. Medical 
 *  Image Analysis, 10(6), 815-825. 
 * 
 * \sa MultiScaleHessianSmoothed3DToVesselnessMeasureImageFilter 
 * \sa AnisotropicDiffusionVesselEnhancementImageFilter 
 * \ingroup FiniteDifferenceFunctions
 * \ingroup Functions
 */


template <class TInputImage, class TOutputImage>
class ITK_EXPORT AJEnhancementDiffusionImageFilter  
: public DenseFiniteDifferenceImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef AJEnhancementDiffusionImageFilter Self;

//  typedef FiniteDifferenceImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::DenseFiniteDifferenceImageFilter<TInputImage,TOutputImage> Superclass;
  typedef SmartPointer<Self>                               Pointer;
  typedef SmartPointer<const Self>                         ConstPointer;

  typedef enum { UNINITIALIZED = 0, INITIALIZED = 1 } FilterStateType;
 

  /** Method for creation through the object factory */
  itkNewMacro( Self );

  /** Run-time type information (and related methods) */
  itkTypeMacro(AJEnhancementDiffusionImageFilter,
                                                ImageToImageFilter );
  


  /** Convenient typedefs */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename Superclass::PixelType       PixelType;

  typedef itk::Image< DiffusionTensor3D< double > , 3 > DiffusionTensorImageType;

  /** Dimensionality of input and output data is assumed to be the same.
   * It is inherited from the superclass. */
  itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);

  typedef AnisotropicDiffusionPlateEnhancementFunction<InputImageType> FiniteDifferenceFunctionType;
  
  typedef HessianRecursiveGaussianImageFilter< InputImageType > HessianFilterType;

  typedef itk::Image< PixelType, 3 >               ObjectnessOutputImageType;

  typedef PlatenessMeasurementFunction PlatenessMeasurementFunctionType;

  typedef MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter<PlatenessMeasurementFunctionType,
                                                InputImageType, 
                                                ObjectnessOutputImageType >
                                                MultiScaleVesselnessFilterType;

  typedef itk::Matrix<double, ImageDimension, ImageDimension> MatrixType;

  // Define image of matrix pixel type 
  typedef itk::Image< MatrixType, ImageDimension>  OutputMatrixImageType;

  // Define the symmetric tensor pixel type
  typedef itk::SymmetricSecondRankTensor< double, ImageDimension> 
                                                         TensorPixelType;
  typedef itk::Image< TensorPixelType, ImageDimension>  
                                                         TensorImageType;

   // Define the type for storing the eigen-value
  typedef itk::FixedArray< double, ImageDimension >      EigenValueArrayType;
  
  // Declare the types of the output images
  typedef itk::Image< EigenValueArrayType, ImageDimension >  
                                                  EigenAnalysisOutputImageType;
  
  // Declare the type for the filter
  typedef itk::SymmetricEigenVectorAnalysisImageFilter< 
                                    TensorImageType, 
                                    EigenAnalysisOutputImageType,
                                    OutputMatrixImageType 
                                    >  EigenVectorMatrixAnalysisFilterType;

  /** The value type of a time step.  Inherited from the superclass. */
  typedef typename Superclass::TimeStepType TimeStepType;

  /** The container type for the update buffer. */
  typedef OutputImageType UpdateBufferType;

  /** Define diffusion image nbd type */
  typedef typename FiniteDifferenceFunctionType::DiffusionTensorNeighborhoodType
                                               DiffusionTensorNeighborhoodType;


  /** Set/Get Macro for VED parameters */
  itkSetMacro( TimeStep, double ); 
  itkSetMacro( Epsilon, double ); 
  itkSetMacro( WStrength, double ); 
  itkSetMacro( Sensitivity, double ); 

  itkGetMacro( TimeStep, double ); 
  itkGetMacro( Epsilon, double ); 
  itkGetMacro( WStrength, double ); 
  itkGetMacro( Sensitivity, double ); 

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(OutputTimesDoubleCheck,
    (Concept::MultiplyOperator<PixelType, double>));
  itkConceptMacro(OutputAdditiveOperatorsCheck,
    (Concept::AdditiveOperators<PixelType>));
  itkConceptMacro(InputConvertibleToOutputCheck,
    (Concept::Convertible<typename TInputImage::PixelType, PixelType>));
  /** End concept checking */
#endif

  /** Set/Get MultiScaleVesselness function parameters */
  void SetSigmaMin( double ); 
  void SetSigmaMax( double ); 
  void SetNumberOfSigmaSteps( int );

  double GetSigmaMin( ); 
  double GetSigmaMax( ); 
  int GetNumberOfSigmaSteps( );


  void SetPlatenessMeasurementFunction(const PlatenessMeasurementFunctionType::Pointer & platenessMeasurementFunction){
	  m_MultiScaleVesselnessFilter->SetObjectnessMeasurementFunction(platenessMeasurementFunction);
  }
protected:
  AJEnhancementDiffusionImageFilter();
 ~AJEnhancementDiffusionImageFilter() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /* overloaded GenerateData method */
  virtual void GenerateData(); 

  /** A simple method to copy the data from the input to the output. ( Supports
   * "read-only" image adaptors in the case where the input image type converts
   * to a different output image type. )  */
  virtual void CopyInputToOutput();

  /** This method applies changes from the m_UpdateBuffer to the output using
   * the ThreadedApplyUpdate() method and a multithreading mechanism.  "dt" is
   * the time step to use for the update of each pixel. */
  virtual void ApplyUpdate(TimeStepType dt);

  /** Method to allow subclasses to get direct access to the update
   * buffer */
  virtual UpdateBufferType* GetUpdateBuffer()
    { return m_UpdateBuffer; }

  /** This method populates an update buffer with changes for each pixel in the
   * output using the ThreadedCalculateChange() method and a multithreading
   * mechanism. Returns value is a time step to be used for the update. */
  virtual TimeStepType CalculateChange();

  /** This method allocates storage in m_UpdateBuffer.  It is called from
   * Superclass::GenerateData(). */
  virtual void AllocateUpdateBuffer();

  /** This method allocates storage for the diffusion tensor image */
  void AllocateDiffusionTensorImage();
 
  /** Update diffusion tensor image */
  void UpdateDiffusionTensorImage();
 
  /** The type of region used for multithreading */
  typedef typename UpdateBufferType::RegionType ThreadRegionType;

  /** The type of region used for multithreading */
  typedef typename DiffusionTensorImageType::RegionType 
                                        ThreadDiffusionImageRegionType;

  /**  Does the actual work of updating the output from the UpdateContainer 
   *   over an output region supplied by the multithreading mechanism.
   *  \sa ApplyUpdate
   *  \sa ApplyUpdateThreaderCallback */ 
  virtual
  void ThreadedApplyUpdate(
                TimeStepType dt,
                const ThreadRegionType &regionToProcess,
                const ThreadDiffusionImageRegionType &diffusionRegionToProcess,
                int threadId);

  /** Does the actual work of calculating change over a region supplied by
   * the multithreading mechanism.
   * \sa CalculateChange
   * \sa CalculateChangeThreaderCallback */
  virtual
  TimeStepType ThreadedCalculateChange(
               const ThreadRegionType &regionToProcess,
               const ThreadDiffusionImageRegionType &diffusionRegionToProcess,
               int threadId);

  /** Prepare for the iteration process. */
  virtual void InitializeIteration();

private:
  //purposely not implemented
  AJEnhancementDiffusionImageFilter(const Self&); 
  void operator=(const Self&); //purposely not implemented

  /** Structure for passing information into static callback methods.  Used in
   * the subclasses' threading mechanisms. */
  struct DenseFDThreadStruct
    {
    AJEnhancementDiffusionImageFilter *Filter;
    TimeStepType TimeStep;
    //TimeStepType *TimeStepList;
    std::vector<TimeStepType> TimeStepList;
    //bool *ValidTimeStepList;
    std::vector<bool> ValidTimeStepList;
    };
    
  /** This callback method uses ImageSource::SplitRequestedRegion to acquire an
   * output region that it passes to ThreadedApplyUpdate for processing. */
  static ITK_THREAD_RETURN_TYPE ApplyUpdateThreaderCallback( void *arg );
  
  /** This callback method uses SplitUpdateContainer to acquire a region
   * which it then passes to ThreadedCalculateChange for processing. */
  static ITK_THREAD_RETURN_TYPE CalculateChangeThreaderCallback( void *arg );
 
  /** The buffer that holds the updates for an iteration of the algorithm. */
  typename UpdateBufferType::Pointer m_UpdateBuffer;

  TimeStepType                                          m_TimeStep;
  typename DiffusionTensorImageType::Pointer            m_DiffusionTensorImage;
  typename MultiScaleVesselnessFilterType::Pointer      m_MultiScaleVesselnessFilter;  
  typename HessianFilterType::Pointer                   m_HessianFilter;  

  typename EigenVectorMatrixAnalysisFilterType::Pointer 
                                      m_EigenVectorMatrixAnalysisFilter; 

  // Vesselness guided diffusion parameters
  double                                                 m_Epsilon;
  double                                                 m_WStrength;
  double                                                 m_Sensitivity;

};
  

}// end namespace ttt

//#if ITK_MANUAL_INSTANTIATION
# include "tttAJEnhancementDiffusionImageFilter.hxx"
//#endif

#endif

/** @}*/
