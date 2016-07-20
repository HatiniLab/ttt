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
#ifndef __itkMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter_txx
#define __itkMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter_txx

#include "tttMultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "vnl/vnl_math.h"

#define EPSILON  1e-03

namespace ttt
{

/**
 * Constructor
 */
template <class TObjectnessMeasurementFunction,typename TInputImage, typename TOutputImage >
MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter
<TObjectnessMeasurementFunction,TInputImage,TOutputImage>
::MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter()
{
  m_SigmaMin = 0.2;
  m_SigmaMax = 2.0;

  m_NumberOfSigmaSteps = 10;

  m_HessianFilter                = HessianFilterType::New();
  m_ObjectnessFilter             = ObjectnessFilterType::New();

  //Turn off vesselness measure scaling
  //m_VesselnessFilter->SetScaleVesselnessMeasure( false );

  //Instantiate Update buffer
  m_UpdateBuffer                 = UpdateBufferType::New();
}

template <class TObjectnessMeasurementFunction,typename TInputImage, typename TOutputImage >
void
MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter
<TObjectnessMeasurementFunction,TInputImage,TOutputImage>
::AllocateUpdateBuffer()
{
  /* The update buffer looks just like the output and holds the best response
     in the  vesselness measure */
  
  typename TOutputImage::Pointer output = this->GetOutput();

  m_UpdateBuffer->SetSpacing(output->GetSpacing());
  m_UpdateBuffer->SetOrigin(output->GetOrigin());
  m_UpdateBuffer->SetLargestPossibleRegion(output->GetLargestPossibleRegion());
  m_UpdateBuffer->SetRequestedRegion(output->GetRequestedRegion());
  m_UpdateBuffer->SetBufferedRegion(output->GetBufferedRegion());
  m_UpdateBuffer->Allocate();
  m_UpdateBuffer->FillBuffer(0);
}


template <class TObjectnessMeasurementFunction,typename TInputImage, typename TOutputImage >
void
MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter
<TObjectnessMeasurementFunction,TInputImage,TOutputImage>
::GenerateData()
{
	//m_ObjectnessFilter

  // Allocate the output
  this->GetOutput()->SetBufferedRegion( 
                 this->GetOutput()->GetRequestedRegion() );
  this->GetOutput()->Allocate();
  this->GetOutput()->FillBuffer(0);
  // Allocate the buffer
  AllocateUpdateBuffer();

  typename InputImageType::ConstPointer input = this->GetInput();
 
  this->m_HessianFilter->SetInput( input );

  this->m_HessianFilter->SetNormalizeAcrossScale( true );
 
  double sigma = m_SigmaMin;

  int scaleLevel = 1;

  while ( sigma <= m_SigmaMax )
    {
    std::cout << "Computing objectness for scale with sigma= "
              << sigma << std::endl;

    m_HessianFilter->SetSigma( sigma );

    m_ObjectnessFilter->SetInput ( m_HessianFilter->GetOutput() );

    m_ObjectnessFilter->Update();
 
    this->UpdateMaximumResponse();

    sigma  = this->ComputeSigmaValue( scaleLevel );

    scaleLevel++;
    } 

  //Write out the best response to the output image
  ImageRegionIterator<UpdateBufferType> 
               it(m_UpdateBuffer,m_UpdateBuffer->GetLargestPossibleRegion());
  it.GoToBegin();

  ImageRegionIterator<TOutputImage> oit(this->GetOutput(),
                          this->GetOutput()->GetLargestPossibleRegion());
  oit.GoToBegin();

  while(!oit.IsAtEnd())
    {
    oit.Value() = static_cast< OutputPixelType >( it.Get() );
    ++oit;
    ++it;
    }

}

template <class TObjectnessMeasurementFunction,typename TInputImage, typename TOutputImage >
void
MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter
<TObjectnessMeasurementFunction,TInputImage,TOutputImage>
::UpdateMaximumResponse()
{

  ImageRegionIterator<UpdateBufferType> 
            oit(m_UpdateBuffer,m_UpdateBuffer->GetLargestPossibleRegion());

  oit.GoToBegin();

  typedef typename ObjectnessFilterType::OutputImageType
                                         ObjectnessOutputImageType;

  ImageRegionIterator<ObjectnessOutputImageType>
            it(m_ObjectnessFilter->GetOutput(),
            this->m_ObjectnessFilter->GetOutput()->GetLargestPossibleRegion());

  it.GoToBegin();

  while(!oit.IsAtEnd())
    {
    if( oit.Value() < it.Value() )
      {
      oit.Value() = it.Value();
      }
    ++oit;
    ++it;
    }
}

template <class TObjectnessMeasurementFunction,typename TInputImage, typename TOutputImage >
double
MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter
<TObjectnessMeasurementFunction,TInputImage,TOutputImage>
::ComputeSigmaValue( int ScaleLevel )
{
  double stepSize = 
     ( vcl_log( m_SigmaMax )  - vcl_log( m_SigmaMin) ) / m_NumberOfSigmaSteps;

  if( stepSize <= 1e-10 )
    {
    stepSize = 1e-10;
    } 

  return ( vcl_exp( vcl_log (m_SigmaMin) + stepSize * ScaleLevel) );
}

template <class TObjectnessMeasurementFunction,typename TInputImage, typename TOutputImage >
void
MultiScaleHessianSmoothed3DToObjectnessMeasureImageFilter
<TObjectnessMeasurementFunction,TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  
  os << indent << "SigmaMin:  " << m_SigmaMin << std::endl;
  os << indent << "SigmaMax:  " << m_SigmaMax  << std::endl;
}


} // end namespace itk
  
#endif
/** @}*/
