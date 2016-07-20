/*
 * tttTissueToCellCorrespondenceFilter.hxx
 *
 *  Created on: Feb 4, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTTISSUETOCELLCORRESPONDENCEFILTER_HXX_
#define MODULES_CORE_INCLUDE_TTTTISSUETOCELLCORRESPONDENCEFILTER_HXX_



#include "tttTissueToCellCorrespondenceFilter.h"
namespace ttt{
/**
 *
 */
template< typename TInputTissue, typename TOutputCellCorrespondence >
TissueToCellCorrespondencesFilter< TInputTissue, TOutputCellCorrespondence >
::TissueToCellCorrespondencesFilter()
{
  this->itk::ProcessObject::SetNumberOfRequiredInputs(2);

  OutputCellCorrespondencesPointer output =
    dynamic_cast< OutputCellCorrespondencesType * >( this->MakeOutput(0).GetPointer() );

  this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);
  this->itk::ProcessObject::SetNthOutput( 0, output.GetPointer() );
}

/**
 *
 */
template< typename TInputTissue, typename TOutputCellCorrespondence >
TissueToCellCorrespondencesFilter< TInputTissue, TOutputCellCorrespondence >
::~TissueToCellCorrespondencesFilter()
{}

/**
 *   Make Ouput
 */
template< typename TInputTissue, typename TOutputCellCorrespondence >
itk::DataObject::Pointer
TissueToCellCorrespondencesFilter< TInputTissue, TOutputCellCorrespondence >
::MakeOutput(DataObjectPointerArraySizeType)
{
	OutputCellCorrespondencesPointer outputAJGraph = OutputCellCorrespondencesType::New();

  return dynamic_cast< itk::DataObject * >( outputAJGraph.GetPointer() );
}


/**
 *
 */
template< typename TInputTissue, typename TOutputCellCorrespondence >
void
TissueToCellCorrespondencesFilter< TInputTissue, TOutputCellCorrespondence >
::SetInput(unsigned int idx, const InputTissueType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->itk::ProcessObject::SetNthInput( idx,
                                    const_cast< InputTissueType * >( input ) );
}



/**
 *
 */
template< typename TInputTissue, typename TOutputCellCorrespondence >
const typename TissueToCellCorrespondencesFilter< TInputTissue, TOutputCellCorrespondence >::InputTissueType *
TissueToCellCorrespondencesFilter< TInputTissue, TOutputCellCorrespondence >
::GetInput(unsigned int idx)
{
  return dynamic_cast< const InputTissueType * >
         ( this->itk::ProcessObject::GetInput(idx) );
}

/**
 *
 */
template< typename TInputTissue, typename TOutputCellCorrespondence >
typename TissueToCellCorrespondencesFilter< TInputTissue, TOutputCellCorrespondence >::OutputCellCorrespondencesType *
TissueToCellCorrespondencesFilter< TInputTissue, TOutputCellCorrespondence >
::GetOutput(void)
{
  return dynamic_cast< OutputCellCorrespondencesType * >
         ( this->itk::ProcessObject::GetOutput(0) );
}

/**
 * copy information from first input to all outputs
 * This is a void implementation to prevent the
 * ProcessObject version to be called
 */
template< typename TInputTissue, typename TOutputCellCorrespondence >
void
TissueToCellCorrespondencesFilter< TInputTissue, TOutputCellCorrespondence >
::GenerateOutputInformation()
{}

}

#endif /* MODULES_CORE_INCLUDE_TTTTISSUETOCELLCORRESPONDENCEFILTER_HXX_ */
