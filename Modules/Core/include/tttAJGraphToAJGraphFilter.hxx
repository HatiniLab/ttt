/*
 * tttAJGraphToAJGraphFilter.hxx
 *
 *  Created on: Feb 4, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTAJGRAPHTOAJGRAPHFILTER_HXX_
#define MODULES_CORE_INCLUDE_TTTAJGRAPHTOAJGRAPHFILTER_HXX_
#include "tttAJGraphToAJGraphFilter.h"
namespace ttt{
/**
 *
 */
template< typename TInputAJGraph, typename TOutputAJGraph >
AJGraphToAJGraphFilter< TInputAJGraph, TOutputAJGraph >
::AJGraphToAJGraphFilter()
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
template< typename TInputAJGraph, typename TOutputAJGraph >
AJGraphToAJGraphFilter< TInputAJGraph, TOutputAJGraph >
::~AJGraphToAJGraphFilter()
{}

/**
 *   Make Ouput
 */
template< typename TInputAJGraph, typename TOutputAJGraph >
itk::DataObject::Pointer
AJGraphToAJGraphFilter< TInputAJGraph, TOutputAJGraph >
::MakeOutput(DataObjectPointerArraySizeType)
{
  OutputAJGraphPointer outputAJGraph = OutputAJGraphType::New();

  return dynamic_cast< itk::DataObject * >( outputAJGraph.GetPointer() );
}


/**
 *
 */
template< typename TInputAJGraph, typename TOutputMesh >
void
AJGraphToAJGraphFilter< TInputAJGraph, TOutputMesh >
::SetInput(unsigned int idx, const InputAJGraphType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->itk::ProcessObject::SetNthInput( idx,
                                    const_cast< InputAJGraphType * >( input ) );
}



/**
 *
 */
template< typename TInputAJGraph, typename TOutputMesh >
const typename AJGraphToAJGraphFilter< TInputAJGraph, TOutputMesh >::InputAJGraphType *
AJGraphToAJGraphFilter< TInputAJGraph, TOutputMesh >
::GetInput(unsigned int idx)
{
  return dynamic_cast< const InputAJGraphType * >
         ( this->itk::ProcessObject::GetInput(idx) );
}

/**
 *
 */
template< typename TInputAJGraph, typename TOutputAJGraph >
typename AJGraphToAJGraphFilter< TInputAJGraph, TOutputAJGraph >::OutputAJGraphType *
AJGraphToAJGraphFilter< TInputAJGraph, TOutputAJGraph >
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
template< typename TInputAJGraph, typename TOutputAJGraph >
void
AJGraphToAJGraphFilter< TInputAJGraph, TOutputAJGraph >
::GenerateOutputInformation()
{}

}



#endif /* MODULES_CORE_INCLUDE_TTTAJGRAPHTOAJGRAPHFILTER_HXX_ */
