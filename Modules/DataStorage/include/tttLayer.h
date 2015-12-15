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
	std::string m_Name;
	std::string m_Description;
	typedef std::string LayerHandlerType;
};
}

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTLAYER_H_ */
