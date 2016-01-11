/*
 * tttFrame.h
 *
 *  Created on: Dec 2, 2015
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTFRAME_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTFRAME_H_
#include "tttLayer.h"

namespace ttt{

class Frame{
public:

	/**
	 * TODO
	 */
	typedef Layer::LayerHandlerType LayerHandlerType;
	/**
	 * TODO
	 */
	typedef unsigned long TimestampType;
	/**
	 * TODO
	 */
	template<class SourceType> typename SourceType::Pointer GetFrameSource() const {

		typename SourceType::Pointer result = SourceType::New();
		result->SetFileName(m_FileName);
		return result;

	}
	/**
	 * TODO
	 */
	template<class SinkType> typename SinkType::Pointer GetFrameSink() const{
		typename SinkType::Pointer result = SinkType::New();
		result->SetFileName(m_FileName);
		return result;
	}

	/**
	 * TODO
	 */
	void SetTimestamp(const TimestampType & timestamp){
		m_Timestamp=timestamp;
	}
	/**
	 * TODO
	 */
	void SetLayerHandler(const LayerHandlerType & layer){
		m_LayerHandler=layer;
	}

	void SetFileName(const std::string & fileName){
		m_FileName=fileName;
	}
	/**
	* TODO
	 */
	LayerHandlerType m_LayerHandler;


	LayerHandlerType GetLayerHandler(){
		return m_LayerHandler;
	}
	/**
	 * TODO
	 */
	TimestampType m_Timestamp;

	/**
	 * TODO
	 */
	std::string m_FileName;
};
}

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTFRAME_H_ */
