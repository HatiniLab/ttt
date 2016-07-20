/*
 * tttAJGraphWriter.hxx
 *
 *  Created on: Feb 3, 2016
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHFILEWRITER_HXX_
#define MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHFILEWRITER_HXX_

#include <tttAJGraphFileWriter.h>
#include <tttAJGraphIOFactory.h>
#include <itkEventObject.h>
#include <vtk_jsoncpp.h>
template<class TInputAJGraph> ttt::AJGraphFileWriter<TInputAJGraph>::AJGraphFileWriter(){
	  m_AJGraphIO = ITK_NULLPTR;
	  m_UseCompression = false;
	  m_FactorySpecifiedAJGraphIO = false;
	  m_UserSpecifiedAJGraphIO = false;
	  m_FileTypeIsBINARY = false;
}

template<class TInputAJGraph> ttt::AJGraphFileWriter<TInputAJGraph>::~AJGraphFileWriter(){

}

template<class TInputAJGraph> void ttt::AJGraphFileWriter<TInputAJGraph>::SetInput(const InputAJGraphType *input)
{
  // ProcessObject is not const_correct so this cast is required here.
  this->ProcessObject::SetNthInput( 0,
                                    const_cast< TInputAJGraph * >( input ) );
}

//---------------------------------------------------------
template< typename TInputAJGraph >
const typename ttt::AJGraphFileWriter< TInputAJGraph >::InputAJGraphType *
ttt::AJGraphFileWriter< TInputAJGraph >
::GetInput(void)
{
	if ( this->GetNumberOfInputs() < 1 )
	    {
	    return ITK_NULLPTR;
	    }

	  return static_cast< TInputAJGraph * >( this->ProcessObject::GetInput(0) );
}

//---------------------------------------------------------
template< typename TInputAJGraph >
const typename ttt::AJGraphFileWriter< TInputAJGraph >::InputAJGraphType *
ttt::AJGraphFileWriter< TInputAJGraph >
::GetInput(unsigned int idx)
{
  return itkDynamicCastInDebugMode< TInputAJGraph * >( this->ProcessObject::GetInput(idx) );
}

template< typename TInputAJGraph >
void
ttt::AJGraphFileWriter< TInputAJGraph >
::Write()
{
	InputAJGraphType *ajgraph = const_cast<InputAJGraphType*>(this->GetInput());
	ajgraph->SetRequestedRegionToLargestPossibleRegion();
	ajgraph->Update();
	  this->InvokeEvent( itk::StartEvent() );





		Json::StyledWriter writer;
		Json::Value root;

		unsigned int k = 0;

		for (auto vit = ajgraph->VerticesBegin();vit!= ajgraph->VerticesEnd();vit++){
			auto vertexHandler = *vit;
			auto vertex = ajgraph->GetAJVertex(vertexHandler);
			root["vertices"][k]["id"] = (Json::UInt64) vertexHandler;
			root["vertices"][k]["x"] = vertex->GetPosition()[0];
			root["vertices"][k]["y"] = vertex->GetPosition()[1];
			root["vertices"][k]["z"] = vertex->GetPosition()[2];
			k++;
		}
		k = 0;
		for(auto eit= ajgraph->EdgesBegin();eit!=ajgraph->EdgesEnd();eit++){
			auto edgeHandler = *eit;
			root["edges"][k]["src"] = (Json::UInt64) ajgraph->GetAJEdgeSource(edgeHandler);
			root["edges"][k]["tgt"] = (Json::UInt64) ajgraph->GetAJEdgeTarget(edgeHandler);
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
template< typename TInputAJGraph >
void
ttt::AJGraphFileWriter< TInputAJGraph >
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


#endif /* MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHFILEWRITER_HXX_ */
