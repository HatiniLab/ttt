/*
 * tttCellGraphSource.hxx
 *
 *  Created on: May 12, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTCELLGRAPHSOURCE_HXX_
#define MODULES_CORE_INCLUDE_TTTCELLGRAPHSOURCE_HXX_



#include "tttCellGraphSource.h"

namespace ttt
{
/**
 *
 */
template< typename TOutputCellGraph >
CellGraphSource< TOutputCellGraph >
::CellGraphSource()
{
  // Create the output. We use static_cast<> here because we know the default
  // output must be of type TOutputMesh
  OutputCellGraphPointer output =
    static_cast< TOutputCellGraph * >( this->MakeOutput(0).GetPointer() );

  this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);
  this->itk::ProcessObject::SetNthOutput( 0, output.GetPointer() );


}

/**
 *
 */
template< typename TOutputCellGraph >
typename CellGraphSource< TOutputCellGraph >::DataObjectPointer
CellGraphSource< TOutputCellGraph >
::MakeOutput(DataObjectPointerArraySizeType)
{
  return TOutputCellGraph::New().GetPointer();
}

/**
 *
 */
template< typename TOutputCellGraph >
typename CellGraphSource< TOutputCellGraph >::OutputCellGraphType *
CellGraphSource< TOutputCellGraph >
::GetOutput(void)
{
  return itkDynamicCastInDebugMode< TOutputCellGraph * >( this->GetPrimaryOutput() );
}

/**
 *
 */
template< typename TOutputCellGraph >
typename CellGraphSource< TOutputCellGraph >::OutputCellGraphType *
CellGraphSource< TOutputCellGraph >
::GetOutput(unsigned int idx)
{
  return itkDynamicCastInDebugMode< TOutputCellGraph * >
         ( this->ProcessObject::GetOutput(idx) );
}

/**
 *
 */
template< typename TOutputCellGraph >
void
CellGraphSource< TOutputCellGraph >
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

/**
 *
 */
template< typename TOutputCellGraph >
void
CellGraphSource< TOutputCellGraph >
::GraftOutput(itk::DataObject *graft)
{
  this->GraftNthOutput(0, graft);
}

/**
 *
 */
template< typename TOutputCellGraph >
void
CellGraphSource< TOutputCellGraph >
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
template< typename TOutputCellGraph >
void
CellGraphSource< TOutputCellGraph >
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
template< typename TOutputCellGraph >
void
CellGraphSource< TOutputCellGraph >
::PrintSelf(std::ostream & os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace ttt






#endif /* MODULES_CORE_INCLUDE_TTTCELLGRAPHSOURCE_HXX_ */
