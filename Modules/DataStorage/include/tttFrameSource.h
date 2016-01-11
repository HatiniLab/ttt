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
#if 0
template<class Type> class FrameSource{
public:
	typedef FrameSource Self;
	typedef std::shared_ptr<FrameSource> Pointer;


	std::string m_FileName;
	virtual void Update()=0;
	virtual typename Type::Pointer GetOutput()=0;
	virtual void SetFileName(const std::string & fileName)=0;
};
#endif
#if 0
template<class PixelType,int dim> class FrameSource<itk::Image<PixelType,dim> > : public itk::ImageFileReader<itk::Image<PixelType,dim> >{

public:
	typedef std::shared_ptr<FrameSource<itk::Image<PixelType,dim> > > Pointer;

	static Pointer  New(){
		return std::make_shared<FrameSource>(new FrameSource);
	}

	virtual void SetFileName(const std::string & fileName){
		itk::ImageFileReader<itk::Image<PixelType,dim> >::SetFileName(fileName);
	}
	virtual void Update(){
		itk::ImageFileReader<itk::Image<PixelType,dim> >::Update();
	}
	virtual typename itk::Image<PixelType,dim>::Pointer GetOutput(){
		return itk::ImageFileReader<itk::Image<PixelType,dim> >::GetOutput();
	}
};
}

#endif
#endif /* MODULES_DATASTORAGE_INCLUDE_TTTFRAMESOURCE_H_ */
