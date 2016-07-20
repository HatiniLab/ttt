/*
 * tttCellGraphFileReader.h
 *
 *  Created on: Jun 28, 2016
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEREADER_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEREADER_H_
#include <tttCellGraphSource.h>
namespace ttt{


template<class TOutputCellGraph> class CellGraphFileReader : public CellGraphSource<TOutputCellGraph>{

public:
	typedef CellGraphFileReader Self;
	typedef CellGraphSource<TOutputCellGraph> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;


	itkTypeMacro(CellGraphFileReader,CellGraphSource<TOutputCellGraph>);

	itkNewMacro(Self);
	itkGetMacro(FileName,std::string);
	itkSetMacro(FileName,std::string);

protected:
	CellGraphFileReader();
	~CellGraphFileReader();
	void GenerateData() ITK_OVERRIDE;
	void PrintSelf(std::ostream & os, itk::Indent indent) const ITK_OVERRIDE;


private:
	CellGraphFileReader(const Self &) ITK_DELETE_FUNCTION;
	void operator=(const Self &) ITK_DELETE_FUNCTION;

	std::string m_FileName;


};
}

#include "tttCellGraphFileReader.hxx"

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEREADER_H_ */
