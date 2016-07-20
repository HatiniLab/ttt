/*
 * tttAJGraphSource.hxx
 *
 *  Created on: Feb 3, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTAJGRAPHSOURCE_HXX_
#define MODULES_CORE_INCLUDE_TTTAJGRAPHSOURCE_HXX_


#include "tttAJGraphSource.h"

namespace ttt
{
/**
 *
 */
template< typename TOutputAJGraph >
AJGraphSource< TOutputAJGraph >
::AJGraphSource()
{
  // Create the output. We use static_cast<> here because we know the default
  // output must be of type TOutputMesh
  OutputAJGraphPointer output =
    static_cast< TOutputAJGraph * >( this->MakeOutput(0).GetPointer() );

  this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);
  this->itk::ProcessObject::SetNthOutput( 0, output.GetPointer() );


}

/**
 *
 */
template< typename TOutputAJGraph >
typename AJGraphSource< TOutputAJGraph >::DataObjectPointer
AJGraphSource< TOutputAJGraph >
::MakeOutput(DataObjectPointerArraySizeType)
{
  return TOutputAJGraph::New().GetPointer();
}

/**
 *
 */
template< typename TOutputAJGraph >
typename AJGraphSource< TOutputAJGraph >::OutputAJGraphType *
AJGraphSource< TOutputAJGraph >
::GetOutput(void)
{
  return itkDynamicCastInDebugMode< TOutputAJGraph * >( this->GetPrimaryOutput() );
}

/**
 *
 */
template< typename TOutputAJGraph >
typename AJGraphSource< TOutputAJGraph >::OutputAJGraphType *
AJGraphSource< TOutputAJGraph >
::GetOutput(unsigned int idx)
{
  return itkDynamicCastInDebugMode< TOutputAJGraph * >
         ( this->ProcessObject::GetOutput(idx) );
}

/**
 *
 */
template< typename TOutputAJGraph >
void
AJGraphSource< TOutputAJGraph >
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

/**
 *
 */
template< typename TOutputAJGraph >
void
AJGraphSource< TOutputAJGraph >
::GraftOutput(itk::DataObject *graft)
{
  this->GraftNthOutput(0, graft);
}

/**
 *
 */
template< typename TOutputAJGraph >
void
AJGraphSource< TOutputAJGraph >
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
template< typename TOutputAJGraph >
void
AJGraphSource< TOutputAJGraph >
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
template< typename TOutputAJGraph >
void
AJGraphSource< TOutputAJGraph >
::PrintSelf(std::ostream & os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace ttt




#endif /* MODULES_CORE_INCLUDE_TTTAJGRAPHSOURCE_HXX_ */
