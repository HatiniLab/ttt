/*
 * tttCellGraphToTissueFilter.hxx
 *
 *  Created on: Jun 29, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTCELLGRAPHTOTISSUEFILTER_HXX_
#define MODULES_CORE_INCLUDE_TTTCELLGRAPHTOTISSUEFILTER_HXX_


#include "tttCellGraphToTissueFilter.h"
namespace ttt{
/**
 *
 */
template< typename TInputCellGraph, typename TOutputTissue >
CellGraphToTissueFilter< TInputCellGraph, TOutputTissue >
::CellGraphToTissueFilter()
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
template< typename TInputCellGraph, typename TOutputTissue >
CellGraphToTissueFilter< TInputCellGraph, TOutputTissue >
::~CellGraphToTissueFilter()
{}

/**
 *   Make Ouput
 */
template< typename TInputCellGraph, typename TOutputTissue >
itk::DataObject::Pointer
CellGraphToTissueFilter< TInputCellGraph, TOutputTissue >
::MakeOutput(DataObjectPointerArraySizeType)
{
  OutputTissuePointer outputTissue = OutputTissueType::New();

  return dynamic_cast< itk::DataObject * >( outputTissue.GetPointer() );
}


/**
 *
 */
template< typename TInputCellGraph, typename TOutputTissue >
void
CellGraphToTissueFilter< TInputCellGraph, TOutputTissue >
::SetInput(unsigned int idx, const InputCellGraphType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->itk::ProcessObject::SetNthInput( idx,
                                    const_cast< InputCellGraphType * >( input ) );
}



/**
 *
 */
template< typename TInputCellGraph, typename TOutputTissue >
const typename CellGraphToTissueFilter< TInputCellGraph, TOutputTissue >::InputCellGraphType *
CellGraphToTissueFilter< TInputCellGraph, TOutputTissue >
::GetInput(unsigned int idx)
{
  return dynamic_cast< const InputCellGraphType * >
         ( this->itk::ProcessObject::GetInput(idx) );
}

/**
 *
 */
template< typename TInputCellGraph, typename TOutputTissue >
typename CellGraphToTissueFilter< TInputCellGraph, TOutputTissue >::OutputTissueType *
CellGraphToTissueFilter< TInputCellGraph, TOutputTissue >
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
template< typename TInputCellGraph, typename TOutputTissue >
void
CellGraphToTissueFilter< TInputCellGraph, TOutputTissue >
::GenerateOutputInformation()
{}

}

#endif /* MODULES_CORE_INCLUDE_TTTCELLGRAPHTOTISSUEFILTER_HXX_ */
