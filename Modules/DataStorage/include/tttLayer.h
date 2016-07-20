/*
 * tttLayer.h
 *
 *  Created on: Dec 2, 2015
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTLAYER_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTLAYER_H_

namespace ttt{

class Layer{
public:
	typedef std::string LayerHandlerType;
	/**
	 * TODO
	 */
	enum LayerType {INTEGER3DIMAGE,FLOAT3DIMAGE,BINARY3DIMAGE,INTEGER2DIMAGE,FLOAT2DIMAGE,BINARY2DIMAGE,CELLGRAPH,TISSUE,NULLLAYER};

	LayerHandlerType m_ID;
	std::string m_Description;
	LayerType m_Type;

	Layer(){
		m_ID="";
		m_Type=NULLLAYER;
		m_Description="";

	}
	Layer(const LayerHandlerType & id, const std::string & description,const LayerType & type){
		m_ID=id;
		m_Description=description;
		m_Type = type;
	}
	LayerHandlerType GetLayerID() const{
		return m_ID;
	}
	std::string GetLayerDescription() const{
		return m_Description;
	}
	LayerType GetLayerType() const{
		return m_Type;
	}
};
}

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTLAYER_H_ */
