/*
 * tttAJGraphToTissueFilter.hxx
 *
 *  Created on: Feb 4, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTAJGRAPHTOTISSUEFILTER_HXX_
#define MODULES_CORE_INCLUDE_TTTAJGRAPHTOTISSUEFILTER_HXX_


#include "tttAJGraphToTissueFilter.h"
namespace ttt{
/**
 *
 */
template< typename TInputAJGraph, typename TOutputTissue >
AJGraphToTissueFilter< TInputAJGraph, TOutputTissue >
::AJGraphToTissueFilter()
{
  this->itk::ProcessObject::SetNumberOfRequiredInputs(1);

  OutputTissuePointer output =
    dynamic_cast< OutputTissueType * >( this->MakeOutput(0).GetPointer() );

  this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);
  this->itk::ProcessObject::SetNthOutput( 0, output.GetPointer() );
}

/**
 *
 */
template< typename TInputAJGraph, typename TOutputAJGraph >
AJGraphToTissueFilter< TInputAJGraph, TOutputAJGraph >
::~AJGraphToTissueFilter()
{}

/**
 *   Make Ouput
 */
template< typename TInputAJGraph, typename TOutputAJGraph >
itk::DataObject::Pointer
AJGraphToTissueFilter< TInputAJGraph, TOutputAJGraph >
::MakeOutput(DataObjectPointerArraySizeType)
{
  OutputTissuePointer outputAJGraph = OutputTissueType::New();

  return dynamic_cast< itk::DataObject * >( outputAJGraph.GetPointer() );
}


/**
 *
 */
template< typename TInputAJGraph, typename TOutputMesh >
void
AJGraphToTissueFilter< TInputAJGraph, TOutputMesh >
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
template< typename TInputAJGraph, typename TOutputTissue >
const typename AJGraphToTissueFilter< TInputAJGraph, TOutputTissue >::InputAJGraphType *
AJGraphToTissueFilter< TInputAJGraph, TOutputTissue >
::GetInput(unsigned int idx)
{
  return dynamic_cast< const InputAJGraphType * >
         ( this->itk::ProcessObject::GetInput(idx) );
}

/**
 *
 */
template< typename TInputAJGraph, typename TOutputTissue >
typename AJGraphToTissueFilter< TInputAJGraph, TOutputTissue >::OutputTissueType *
AJGraphToTissueFilter< TInputAJGraph, TOutputTissue >
::GetOutput(void)
{
  return dynamic_cast< OutputTissueType * >
         ( this->itk::ProcessObject::GetOutput(0) );
}

/**
 * copy information from first input to all outputs
 * This is a void implementation to prevent the
 * ProcessObject version to be called
 */
template< typename TInputAJGraph, typename TOutputTissue >
void
AJGraphToTissueFilter< TInputAJGraph, TOutputTissue >
::GenerateOutputInformation()
{}

}






#endif /* MODULES_CORE_INCLUDE_TTTAJGRAPHTOTISSUEFILTER_HXX_ */
