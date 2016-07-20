/*
 * tttImageToAJGraphFilter.hxx
 *
 *  Created on: Feb 3, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTIMAGETOCELLGRAPHFILTER_HXX_
#define MODULES_CORE_INCLUDE_TTTIMAGETOCELLGRAPHFILTER_HXX_

#include "tttImageToCellGraphFilter.h"

namespace ttt
{
/**
 *
 */
template< typename TInputImage, typename TOutputCellGraph >
ImageToCellGraphFilter< TInputImage, TOutputCellGraph >
::ImageToCellGraphFilter()
{
  this->itk::ProcessObject::SetNumberOfRequiredInputs(1);

  OutputCellGraphPointer output =
    dynamic_cast< OutputCellGraphType * >( this->MakeOutput(0).GetPointer() );

  this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);
  this->itk::ProcessObject::SetNthOutput( 0, output.GetPointer() );
}

/**
 *
 */
template< typename TInputImage, typename TOutputCellGraph >
ImageToCellGraphFilter< TInputImage, TOutputCellGraph >
::~ImageToCellGraphFilter()
{}

/**
 *   Make Ouput
 */
template< typename TInputImage, typename TOutputCellGraph >
itk::DataObject::Pointer
ImageToCellGraphFilter< TInputImage, TOutputCellGraph >
::MakeOutput(DataObjectPointerArraySizeType)
{
  OutputCellGraphPointer outputMesh = OutputCellGraphType::New();

  return dynamic_cast< itk::DataObject * >( outputMesh.GetPointer() );
}

/**
 *
 */
template< typename TInputImage, typename TOutputCellGraph >
void
ImageToCellGraphFilter< TInputImage, TOutputCellGraph >
::SetInput(unsigned int idx, const InputImageType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->itk::ProcessObject::SetNthInput( idx,
                                    const_cast< InputImageType * >( input ) );
}

/**
 *
 */
template< typename TInputImage, typename TOutputCellGraph >
const typename ImageToCellGraphFilter< TInputImage, TOutputCellGraph >::InputImageType *
ImageToCellGraphFilter< TInputImage, TOutputCellGraph >
::GetInput(unsigned int idx)
{
  return dynamic_cast< const InputImageType * >
         ( this->itk::ProcessObject::GetInput(idx) );
}

/**
 *
 */
template< typename TInputImage, typename TOutputCellGraph >
typename ImageToCellGraphFilter< TInputImage, TOutputCellGraph >::OutputCellGraphType *
ImageToCellGraphFilter< TInputImage, TOutputCellGraph >
::GetOutput(void)
{
  return dynamic_cast< OutputCellGraphType * >
         ( this->itk::ProcessObject::GetOutput(0) );
}

/**
 * copy information from first input to all outputs
 * This is a void implementation to prevent the
 * ProcessObject version to be called
 */
template< typename TInputImage, typename TOutputCellGraph >
void
ImageToCellGraphFilter< TInputImage, TOutputCellGraph >
::GenerateOutputInformation()
{}
} // end namespace itk


#endif /* MODULES_CORE_INCLUDE_TTTIMAGETOAJGRAPHFILTER_HXX_ */
