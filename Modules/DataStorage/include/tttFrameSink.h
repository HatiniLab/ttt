/*
 * tttFrameSink.h
 *
 *  Created on: Dec 2, 2015
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_TTTFRAMESINK_H_
#define MODULES_DATASTORAGE_TTTFRAMESINK_H_

#include <itkImageFileWriter.h>


template<class Type> class FrameSink{

	std::string m_FileName;
	typedef FrameSink Self;
	typedef std::shared_ptr<Self> Pointer;
};

template<class PixelType,int dim> class FrameSink<itk::Image<PixelType,dim> > : public itk::ImageFileWriter<itk::Image<PixelType,dim> >{
protected:
	FrameSink(){

	}
	virtual void Update(){
		itk::ImageFileWriter<itk::Image<PixelType,dim> >::Update();
	}
	virtual void SetInput(const typename itk::Image<PixelType,dim>::Pointer & input){
		itk::ImageFileWriter<itk::Image<PixelType,dim> >::SetInput(input);
	}
};



#endif /* MODULES_DATASTORAGE_TTTFRAMESINK_H_ */
