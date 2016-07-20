/*
 * tttTemporalContainerFileReader.h
 *
 *  Created on: Jul 19, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTTEMPORALCONTAINERFILEREADER_H_
#define MODULES_CORE_INCLUDE_TTTTEMPORALCONTAINERFILEREADER_H_
#include "itkProcessObject.h"

namespace ttt{

template<class TTemporalFile> class TemporalContainerFileReader : public itk::ProcessObject{
public:
	typedef TemporalContainerFileReader<TTemporalFile> Self;
	typedef itk::ProcessObject Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const  Self> ConstPointer;

	itkNewMacro(Self)

protected:
	TemporalContainerFileReader(){

	}

	virtual void GenerateData(){

	}

	virtual void SetFileName(std::string & fileName){
		m_FileName=fileName;
	}

private:
	std::string m_FileName;
};
}

#endif /* MODULES_CORE_INCLUDE_TTTTEMPORALCONTAINERFILEREADER_H_ */
