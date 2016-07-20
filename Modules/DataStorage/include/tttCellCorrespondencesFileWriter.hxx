/*
 * tttCellCorrespondencesFileWriter.hxx
 *
 *  Created on: Feb 4, 2016
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTCELLCORRESPONDENCESFILEWRITER_HXX_
#define MODULES_DATASTORAGE_INCLUDE_TTTCELLCORRESPONDENCESFILEWRITER_HXX_

#include "tttCellCorrespondencesFileWriter.h"
#include <vtk_jsoncpp.h>
#include <fstream>
template<class TInputCellCorrespondences> ttt::CellCorrespondencesFileWriter<TInputCellCorrespondences>::CellCorrespondencesFileWriter(){

}

template<class TInputCellCorrespondences> ttt::CellCorrespondencesFileWriter<TInputCellCorrespondences>::~CellCorrespondencesFileWriter(){

}

template<class TInputCellCorrespondences> void
ttt::CellCorrespondencesFileWriter<TInputCellCorrespondences>
::SetInput(const InputCellCorrespondencesType *input)
{
  // ProcessObject is not const_correct so this cast is required here.
  this->itk::ProcessObject::SetNthInput( 0,
                                    const_cast< TInputCellCorrespondences * >( input ) );
}

//---------------------------------------------------------
template< typename TInputCellCorrespondences >
const typename ttt::CellCorrespondencesFileWriter< TInputCellCorrespondences >::InputCellCorrespondencesType *
ttt::CellCorrespondencesFileWriter< TInputCellCorrespondences >
::GetInput(void)
{
	if ( this->GetNumberOfInputs() < 1 )
	    {
	    return ITK_NULLPTR;
	    }

	  return static_cast< TInputCellCorrespondences * >( this->itk::ProcessObject::GetInput(0) );
}

//---------------------------------------------------------
template< typename TInputCellCorrespondences >
const typename ttt::CellCorrespondencesFileWriter< TInputCellCorrespondences >::InputCellCorrespondencesType *
ttt::CellCorrespondencesFileWriter< TInputCellCorrespondences >
::GetInput(unsigned int idx)
{
  return itkDynamicCastInDebugMode< TInputCellCorrespondences * >( this->ProcessObject::GetInput(idx) );
}

template< typename TInputCellCorrespondences >
void
ttt::CellCorrespondencesFileWriter< TInputCellCorrespondences >
::Write()
{

	InputCellCorrespondencesType *correspondences = const_cast<InputCellCorrespondencesType*>(this->GetInput());
	correspondences->SetRequestedRegionToLargestPossibleRegion();
	correspondences->Update();
	this->InvokeEvent(itk::StartEvent());

	Json::StyledWriter writer;
	Json::Value root;

	std::string correspondencesStorageFile = m_FileName;

	int k = 0;
	for (auto corr = correspondences->SuccesorBegin(); corr != correspondences->SuccesorEnd(); corr++) {
		root["succesor"][k]["a"] = (Json::UInt64) corr->first;
		root["succesor"][k]["b"] = (Json::UInt64) corr->second;
		k++;
	}

	k = 0;
	for (auto corr = correspondences->ApoptosisBegin(); corr != correspondences->ApoptosisEnd(); corr++) {
		root["apoptosis"][k] = (Json::UInt64) *corr;
		k++;
	}
	k = 0;
	for (auto corr = correspondences->EnteringBegin(); corr != correspondences->EnteringEnd(); corr++) {
		root["entering"][k] = (Json::UInt64) *corr;
		k++;
	}
	k = 0;
	for (auto corr = correspondences->LeavingBegin(); corr != correspondences->LeavingEnd(); corr++) {
		root["leaving"][k] = (Json::UInt64) *corr;
		k++;
	}

	k = 0;
	for (auto corr = correspondences->MitosisBegin(); corr != correspondences->MitosisEnd(); corr++) {
		root["mitosis"][k]["a"] = (Json::UInt64) corr->first;
		root["mitosis"][k]["b0"] = (Json::UInt64) corr->second.first;
		root["mitosis"][k]["b1"] = (Json::UInt64) corr->second.second;
		k++;
	}

	std::string jsoncontent = writer.write(root);
	std::ofstream file(correspondencesStorageFile.c_str(), std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();
	this->InvokeEvent(itk::EndEvent());

	// Release upstream data if requested
	this->ReleaseInputs();

}

//---------------------------------------------------------
template< typename TInputCellCorrespondences >
void
ttt::CellCorrespondencesFileWriter< TInputCellCorrespondences >
::PrintSelf(std::ostream & os, itk::Indent indent) const
{
#if 0
  Superclass::PrintSelf(os, indent);

  os << indent << "File Name: "
     << ( m_FileName.data() ? m_FileName.data() : "(none)" ) << std::endl;

  os << indent << "Image IO: ";
  if ( m_ImageIO.IsNull() )
    {
    os << "(none)\n";
    }
  else
    {
    os << m_ImageIO << "\n";
    }

  os << indent << "IO Region: " << m_PasteIORegion << "\n";
  os << indent << "Number of Stream Divisions: " << m_NumberOfStreamDivisions << "\n";

  if ( m_UseCompression )
    {
    os << indent << "Compression: On\n";
    }
  else
    {
    os << indent << "Compression: Off\n";
    }

  if ( m_UseInputMetaDataDictionary )
    {
    os << indent << "UseInputMetaDataDictionary: On\n";
    }
  else
    {
    os << indent << "UseInputMetaDataDictionary: Off\n";
    }

  if ( m_FactorySpecifiedImageIO )
    {
    os << indent << "FactorySpecifiedmageIO: On\n";
    }
  else
    {
    os << indent << "FactorySpecifiedmageIO: Off\n";
    }
#endif
}

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTCELLCORRESPONDENCESFILEWRITER_HXX_ */
