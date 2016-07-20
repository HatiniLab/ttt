/*
 * tttImageToAJGraphFilter.hxx
 *
 *  Created on: Feb 3, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTIMAGETOAJGRAPHFILTER_HXX_
#define MODULES_CORE_INCLUDE_TTTIMAGETOAJGRAPHFILTER_HXX_

#include "tttImageToAJGraphFilter.h"

namespace ttt
{
/**
 *
 */
template< typename TInputImage, typename TOutputAJGraph >
ImageToAJGraphFilter< TInputImage, TOutputAJGraph >
::ImageToAJGraphFilter()
{
  this->itk::ProcessObject::SetNumberOfRequiredInputs(1);

  OutputAJGraphPointer output =
    dynamic_cast< OutputAJGraphType * >( this->MakeOutput(0).GetPointer() );

  this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);
  this->itk::ProcessObject::SetNthOutput( 0, output.GetPointer() );
}

/**
 *
 */
template< typename TInputImage, typename TOutputAJGraph >
ImageToAJGraphFilter< TInputImage, TOutputAJGraph >
::~ImageToAJGraphFilter()
{}

/**
 *   Make Ouput
 */
template< typename TInputImage, typename TOutputAJGraph >
itk::DataObject::Pointer
ImageToAJGraphFilter< TInputImage, TOutputAJGraph >
::MakeOutput(DataObjectPointerArraySizeType)
{
  OutputAJGraphPointer outputMesh = OutputAJGraphType::New();

  return dynamic_cast< itk::DataObject * >( outputMesh.GetPointer() );
}

/**
 *
 */
template< typename TInputImage, typename TOutputMesh >
void
ImageToAJGraphFilter< TInputImage, TOutputMesh >
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
template< typename TInputImage, typename TOutputMesh >
const typename ImageToAJGraphFilter< TInputImage, TOutputMesh >::InputImageType *
ImageToAJGraphFilter< TInputImage, TOutputMesh >
::GetInput(unsigned int idx)
{
  return dynamic_cast< const InputImageType * >
         ( this->itk::ProcessObject::GetInput(idx) );
}

/**
 *
 */
template< typename TInputImage, typename TOutputAJGraph >
typename ImageToAJGraphFilter< TInputImage, TOutputAJGraph >::OutputAJGraphType *
ImageToAJGraphFilter< TInputImage, TOutputAJGraph >
::GetOutput(void)
{
  return dynamic_cast< OutputAJGraphType * >
         ( this->itk::ProcessObject::GetOutput(0) );
}

/**
 * copy information from first input to all outputs
 * This is a void implementation to prevent the
 * ProcessObject version to be called
 */
template< typename TInputImage, typename TOutputAJGraph >
void
ImageToAJGraphFilter< TInputImage, TOutputAJGraph >
::GenerateOutputInformation()
{}
} // end namespace itk


#endif /* MODULES_CORE_INCLUDE_TTTIMAGETOAJGRAPHFILTER_HXX_ */
