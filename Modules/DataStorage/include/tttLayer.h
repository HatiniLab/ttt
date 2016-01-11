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
	enum LayerType {IMAGE,VECTORIMAGE,TENSORIMAGE,AJGRAPH,CELLGRAPH};

	LayerHandlerType m_ID;
	std::string m_Description;
	LayerType m_Type;

	Layer(){
		m_ID="";
		m_Type=IMAGE;
		m_Description="";

	}
	Layer(LayerHandlerType & id, std::string & description,LayerType & type){
		m_ID=id;
		m_Description=description;
		m_Type = type;
	}
	LayerHandlerType GetLayerID(){
		return m_ID;
	}
	std::string GetLayerDescription(){
		return m_Description;
	}
	LayerType GetLayerType(){
		return m_Type;
	}
};
}

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTLAYER_H_ */
