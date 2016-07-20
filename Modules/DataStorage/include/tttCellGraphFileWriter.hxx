/*
 * tttCellGraphFileWriter.hxx
 *
 *  Created on: Jun 28, 2016
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEWRITER_HXX_
#define MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEWRITER_HXX_


namespace ttt{
template<class TInputCellGraph> CellGraphFileWriter<TInputCellGraph>::CellGraphFileWriter(){
	  m_UseCompression = false;
	  m_FactorySpecifiedAJGraphIO = false;
	  m_UserSpecifiedAJGraphIO = false;
	  m_FileTypeIsBINARY = false;
}

template<class TInputCellGraph> CellGraphFileWriter<TInputCellGraph>::~CellGraphFileWriter(){

}

template<class TInputCellGraph> void CellGraphFileWriter<TInputCellGraph>::SetInput(const InputCellGraphType *input)
{
  // ProcessObject is not const_correct so this cast is required here.
  this->ProcessObject::SetNthInput( 0,
                                    const_cast< TInputCellGraph * >( input ) );
}

//---------------------------------------------------------
template< typename TInputCellGraph >
const typename CellGraphFileWriter< TInputCellGraph >::InputCellGraphType *
CellGraphFileWriter< TInputCellGraph >
::GetInput(void)
{
	if ( this->GetNumberOfInputs() < 1 )
	    {
	    return ITK_NULLPTR;
	    }

	  return static_cast< TInputCellGraph * >( this->ProcessObject::GetInput(0) );
}

//---------------------------------------------------------
template< typename TInputCellGraph >
const typename CellGraphFileWriter< TInputCellGraph >::InputCellGraphType *
CellGraphFileWriter< TInputCellGraph >
::GetInput(unsigned int idx)
{
  return itkDynamicCastInDebugMode< TInputCellGraph * >( this->ProcessObject::GetInput(idx) );
}

template< typename TInputCellGraph >
void
CellGraphFileWriter< TInputCellGraph >
::Write()
{
	InputCellGraphType *cellGraph = const_cast<InputCellGraphType*>(this->GetInput());
	cellGraph->SetRequestedRegionToLargestPossibleRegion();
	cellGraph->Update();
	this->InvokeEvent( itk::StartEvent() );





	Json::StyledWriter writer;
	Json::Value root;

	unsigned int k = 0;

	for(auto cellit = tissue->GetCellGraph()->CellsBegin();cellit!=tissue->GetCellGraph()->CellsEnd();cellit++){
		ttt::Cell::Pointer currentCell = tissue->GetCellGraph()->GetCell(*cellit);
		root["dual"]["vertices"][k]["id"] = (Json::UInt64) *cellit;
		root["dual"]["vertices"][k]["x"] = currentCell->GetPosition()[0];
		root["dual"]["vertices"][k]["y"] = currentCell->GetPosition()[1];
		root["dual"]["vertices"][k]["z"] = currentCell->GetPosition()[2];
		root["dual"]["vertices"][k]["area"] = currentCell->GetArea();
		root["dual"]["vertices"][k]["xx"] = currentCell->GetXX();
		root["dual"]["vertices"][k]["xy"] = currentCell->GetXY();
		root["dual"]["vertices"][k]["yy"] = currentCell->GetYY();
		root["dual"]["vertices"][k]["perimeterLength"] = currentCell->GetPerimeterLength();
		root["dual"]["vertices"][k]["eccentricity"] = currentCell->GetEccentricity();

		unsigned int l = 0;
		for (ttt::Cell::PerimeterIterator perimeterIt =	currentCell->PerimeterBegin();
				perimeterIt != currentCell->PerimeterEnd(); ++perimeterIt) {
			root["dual"]["vertices"][k]["perimeter"][l] =(Json::UInt64) *perimeterIt;
			l++;
		}
		k++;
	}
	k = 0;
	for(auto edgeIt = tissue->GetCellGraph()->CellEdgesBegin();edgeIt!=tissue->GetCellGraph()->CellEdgesEnd();edgeIt++){
		root["dual"]["edges"][k]["src"] = (Json::UInt64)tissue->GetCellGraph()->GetCellEdgeSource(*edgeIt);
		root["dual"]["edges"][k]["tgt"] = (Json::UInt64)tissue->GetCellGraph()->GetCellEdgeTarget(*edgeIt);
		k++;
	}



	std::string jsoncontent = writer.write(root);
	std::ofstream file(m_FileName,std::ofstream::out | std::ofstream::trunc);

	file << jsoncontent;

	file.close();
	// Notify end event observers
	this->InvokeEvent( itk::EndEvent() );

	// Release upstream data if requested
	this->ReleaseInputs();
}

//---------------------------------------------------------
template< typename TInputCellGraph >
void
CellGraphFileWriter< TInputCellGraph >
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


}


#endif /* MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEWRITER_HXX_ */
