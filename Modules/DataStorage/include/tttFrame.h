/*
 * tttFrame.h
 *
 *  Created on: Dec 2, 2015
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTFRAME_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTFRAME_H_

#include "tttFrameBase.h"

namespace ttt{

class Frame : public FrameBase{
public:

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


	TimestampType GetTimestamp() const{
		return m_Timestamp;
	}
	/**
	 * TODO
	 */
	TimestampType m_Timestamp;

};
}

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTFRAME_H_ */
