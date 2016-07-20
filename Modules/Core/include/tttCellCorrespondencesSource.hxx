/*
 * tttCellCorrespondencesSource.hxx
 *
 *  Created on: Feb 4, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTCELLCORRESPONDENCESSOURCE_HXX_
#define MODULES_CORE_INCLUDE_TTTCELLCORRESPONDENCESSOURCE_HXX_
#include "tttCellCorrespondencesSource.h"
namespace ttt
{
/**
 *
 */
template< typename TOutputCellCorrespondences >
CellCorrespondenceSource< TOutputCellCorrespondences >
::CellCorrespondenceSource()
{
  // Create the output. We use static_cast<> here because we know the default
  // output must be of type TOutputMesh
	OutputCellCorrespondencesPointer output =
    static_cast< TOutputCellCorrespondences * >( this->MakeOutput(0).GetPointer() );

  this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);
  this->itk::ProcessObject::SetNthOutput( 0, output.GetPointer() );


}

/**
 *
 */
template< typename TOutputCellCorrespondences >
typename CellCorrespondenceSource< TOutputCellCorrespondences >::DataObjectPointer
CellCorrespondenceSource< TOutputCellCorrespondences >
::MakeOutput(DataObjectPointerArraySizeType)
{
  return TOutputCellCorrespondences::New().GetPointer();
}

/**
 *
 */
template< typename TOutputCellCorrespondences >
typename CellCorrespondenceSource< TOutputCellCorrespondences >::OutputCellCorrespondencesType *
CellCorrespondenceSource< TOutputCellCorrespondences >
::GetOutput(void)
{
  return itkDynamicCastInDebugMode< TOutputCellCorrespondences * >( this->GetPrimaryOutput() );
}

/**
 *
 */
template< typename TOutputTissue >
typename CellCorrespondenceSource< TOutputTissue >::OutputCellCorrespondencesType *
CellCorrespondenceSource< TOutputTissue >
::GetOutput(unsigned int idx)
{
  return itkDynamicCastInDebugMode< TOutputTissue * >
         ( this->ProcessObject::GetOutput(idx) );
}

/**
 *
 */
template< typename TOutputTissue >
void
CellCorrespondenceSource< TOutputTissue >
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

/**
 *
 */
template< typename TOutputTissue >
void
CellCorrespondenceSource< TOutputTissue >
::GraftOutput(itk::DataObject *graft)
{
  this->GraftNthOutput(0, graft);
}

/**
 *
 */
template< typename TOutputTissue >
void
CellCorrespondenceSource< TOutputTissue >
::GraftOutput(const DataObjectIdentifierType & key, itk::DataObject *graft)
{
  if ( !graft )
    {
    itkExceptionMacro(<< "Requested to graft output that is a ITK_NULLPTR pointer");
    }

  // we use the process object method since all out output may not be
  // of the same type
  itk::DataObject *output = this->ProcessObject::GetOutput(key);

  // Call GraftImage to copy meta-information, regions, and the pixel container
  output->Graft(graft);
}

/**
 *
 */
template< typename TOutputTissue >
void
CellCorrespondenceSource< TOutputTissue >
::GraftNthOutput(unsigned int idx, itk::DataObject *graft)
{
  if ( idx >= this->GetNumberOfIndexedOutputs() )
    {
    itkExceptionMacro(<< "Requested to graft output " << idx
                      << " but this filter only has " << this->GetNumberOfIndexedOutputs() << " indexed Outputs.");
    }
  this->GraftOutput( this->MakeNameFromOutputIndex(idx), graft );
}

/**
 *
 */
template< typename TOutputTissue >
void
CellCorrespondenceSource< TOutputTissue >
::PrintSelf(std::ostream & os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace ttt

#endif /* MODULES_CORE_INCLUDE_TTTCELLCORRESPONDENCESSOURCE_HXX_ */
