/*
 * tttAJGraphReader.h
 *
 *  Created on: Feb 3, 2016
 *      Author: morgan
 */




#ifndef MODULES_DATASTORAGE_INCLUDE_TTTTISSUEREADER_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTTISSUEREADER_H_

#include <tttAJGraphSource.h>

namespace ttt{

template<class TOutputAJGraph>
class AJGraphFileReader : public AJGraphSource<TOutputAJGraph>{

public:
	typedef AJGraphFileReader Self;
	typedef AJGraphSource<TOutputAJGraph> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;


	itkTypeMacro(AJGraphFileReader,AJGraphSource<TOutputAJGraph>);

	itkNewMacro(Self);
	itkGetMacro(FileName,std::string);
	itkSetMacro(FileName,std::string);

protected:
	AJGraphFileReader();
	~AJGraphFileReader();
	void GenerateData() ITK_OVERRIDE;
	void PrintSelf(std::ostream & os, itk::Indent indent) const ITK_OVERRIDE;


private:
	AJGraphFileReader(const Self &) ITK_DELETE_FUNCTION;
	void operator=(const Self &) ITK_DELETE_FUNCTION;

	std::string m_FileName;
};

}

#include "tttAJGraphFileReader.hxx"

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTTISSUEREADER_H_ */
