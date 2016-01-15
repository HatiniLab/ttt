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
	enum LayerType {IMAGE,VECTORIMAGE,TENSORIMAGE,AJGRAPH,CELLGRAPH,NULLLAYER};

	LayerHandlerType m_ID;
	std::string m_Description;
	LayerType m_Type;

	Layer(){
		m_ID="";
		m_Type=NULLLAYER;
		m_Description="";

	}
	Layer(LayerHandlerType & id, std::string & description,LayerType & type){
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
