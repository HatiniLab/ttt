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
#ifndef __itkHessianSmoothed3DToObjectnessMeasureImageFilter_txx
#define __itkHessianSmoothed3DToObjectnessMeasureImageFilter_txx

#include "tttHessianSmoothed3DToObjectnessMeasureImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "vnl/vnl_math.h"

#define EPSILON  1e-03

#include "itkImageFileWriter.h"
namespace ttt
{

/**
 * Constructor
 */
template <class TObjectnessMeasurementFunction, typename TPixel >
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction, TPixel >
::HessianSmoothed3DToObjectnessMeasureImageFilter()
{
  m_SymmetricEigenValueFilter = EigenAnalysisFilterType::New();
  m_SymmetricEigenValueFilter->SetDimension( ImageDimension );
  m_SymmetricEigenValueFilter->OrderEigenValuesBy(EigenAnalysisFilterType::FunctorType::OrderByValue );

  // By default, scale the vesselness measure by the largest
  // eigen value

}

template<class TObjectnessMeasurementFunction,typename TPixel>
void
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction,TPixel>::BeforeThreadedGenerateData(){
	  m_SymmetricEigenValueFilter->SetInput( this->GetInput() );
	  m_SymmetricEigenValueFilter->Update();
}
template <class TObjectnessMeasurementFunction, typename TPixel >
void 
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction, TPixel >::ThreadedGenerateData(const OutputRegionType& windowRegion, ThreadIdType threadId){
  itkDebugMacro(
      << "HessianSmoothed3DToObjectMeasureImageFilter generating data ");

  typename OutputImageType::Pointer output = this->GetOutput();

  typedef typename EigenAnalysisFilterType::OutputImageType EigenValueImageType;


  
  const typename EigenValueImageType::ConstPointer eigenImage = 
                    m_SymmetricEigenValueFilter->GetOutput();
  
  // walk the region of eigen values and get the vesselness measure
  EigenValueArrayType eigenValue;

  ImageRegionConstIteratorWithIndex< EigenValueImageType > it(eigenImage, windowRegion );
  ImageRegionIterator< OutputImageType > oit( output, windowRegion );


  it.GoToBegin();
  oit.GoToBegin();
  while (!it.IsAtEnd())
    {
	  typename InputImageType::IndexType index = it.GetIndex();
	  typename EigenValueImageType::PixelType pixel = it.Get();
	  float objectProperty=m_ObjectnessMeasurementFunction->ComputeObjectProperty(pixel);
	  oit.Set(objectProperty);
	  ++it;
	  ++oit;
    }
}
template<class TObjectnessMeasurementFunction,typename TPixel>
void
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction,TPixel>
::EnlargeOutputRequestedRegion(DataObject *output)
{
  Superclass::EnlargeOutputRequestedRegion(output);
  output->SetRequestedRegionToLargestPossibleRegion();
}


template<class TObjectnessMeasurementFunction,typename TPixel>
void
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction,TPixel>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
  if ( this->GetInput() )
    {
    typename InputImageType::Pointer image =
      const_cast< InputImageType * >( this->GetInput() );
    image->SetRequestedRegionToLargestPossibleRegion();
    }
}



template<class TObjectnessMeasurementFunction,typename TPixel>
void
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction,TPixel>::AfterThreadedGenerateData(){
}


template <class TObjectnessMeasurementFunction, typename TPixel >
void
HessianSmoothed3DToObjectnessMeasureImageFilter<TObjectnessMeasurementFunction, TPixel >::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  
}


} // end namespace itk
  
#endif
/** @}*/
