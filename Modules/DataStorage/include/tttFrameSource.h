/*
 * tttFrameSource.h
 *
 *  Created on: Dec 2, 2015
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTFRAMESOURCE_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTFRAMESOURCE_H_
#include <memory>

namespace ttt{

template<class Type> class FrameSource{
public:
	typedef FrameSource Self;
	typedef std::shared_ptr<FrameSource> Pointer;

	std::string m_FileName;
	virtual void Update()=0;
	virtual typename Type::Pointer GetOutput()=0;
};

template<class PixelType,int dim> class FrameSource<itk::Image<PixelType,dim> > : public itk::ImageFileReader<itk::Image<PixelType,dim> >{

protected:
	virtual void Update(){
		itk::ImageFileReader<itk::Image<PixelType,dim> >::Update();
	}
	virtual typename itk::Image<PixelType,dim>::Pointer GetOutput(){
		return itk::ImageFileReader<itk::Image<PixelType,dim> >::GetOutput();
	}
};
}


#endif /* MODULES_DATASTORAGE_INCLUDE_TTTFRAMESOURCE_H_ */
