/*
 * tttTissueReader.h
 *
 *  Created on: Feb 3, 2016
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTTISSUEFILEREADER_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTTISSUEFILEREADER_H_

#include "tttTissueSource.h"

namespace ttt{

template<class TOutputTissue> class TissueFileReader : public TissueSource<TOutputTissue>{

public:
	typedef TissueFileReader Self;
	typedef TissueSource<TOutputTissue> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;


	itkTypeMacro(TissueFileReader,TissueSource<TOutputTissue>);

	itkNewMacro(Self);
	itkGetMacro(FileName,std::string);
	itkSetMacro(FileName,std::string);

protected:
	TissueFileReader();
	~TissueFileReader();
	void GenerateData() ITK_OVERRIDE;
	void PrintSelf(std::ostream & os, itk::Indent indent) const ITK_OVERRIDE;


private:
	TissueFileReader(const Self &) ITK_DELETE_FUNCTION;
	void operator=(const Self &) ITK_DELETE_FUNCTION;

	std::string m_FileName;


};

}
#include "tttTissueFileReader.hxx"

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTTISSUEFILEREADER_H_ */
