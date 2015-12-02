/*
 * tttFrame.h
 *
 *  Created on: Dec 2, 2015
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTFRAME_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTFRAME_H_
#include "tttLayer.h"
#include "tttFrameSource.h"
#include "tttFrameSink.h"
namespace ttt{
class Frame{
public:
	enum FrameType {IMAGE,HESSIANIMAGE,AJGRAPH,CELLGRAPH,REGISTRATION} ;

	typedef Layer::LayerHandlerType LayerHandlerType;
	typedef unsigned long TimestampType;

	template<class T> typename FrameSource<T>::Pointer GetFrameSource(){
		typename FrameSource<T>::Pointer result = FrameSource<T>::New();
	}
	template<class T> typename FrameSink<T>::Pointer GetFrameSink();

	void SetTimestamp(const TimestampType & timestamp){
		m_Timestamp=timestamp;
	}
	void SetLayer(const LayerHandlerType & layer){
		m_LayerHandler=layer;
	}
	LayerHandlerType m_LayerHandler;
	TimestampType m_Timestamp;
	std::string   m_FileName;
};
}



#endif /* MODULES_DATASTORAGE_INCLUDE_TTTFRAME_H_ */
