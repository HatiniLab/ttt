/*
 * tttCellCorrespondenceFileWriter.h
 *
 *  Created on: Feb 4, 2016
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTCELLCORRESPONDENCESFILEWRITER_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTCELLCORRESPONDENCESFILEWRITER_H_

#include "itkProcessObject.h"

namespace ttt {

template<class TInputCellCorrespondences> class CellCorrespondencesFileWriter: public itk::ProcessObject {
public:
	typedef CellCorrespondencesFileWriter Self;
	typedef itk::ProcessObject Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro (Self);
	itkTypeMacro(CellCorrespondencesFileWriter, itk::ProcessObject)



	  /** Some convenient typedefs. */
	  typedef TInputCellCorrespondences                         InputCellCorrespondencesType;
	  typedef typename InputCellCorrespondencesType::Pointer    InputAJGraphPointer;


	  using Superclass::SetInput;
	  void SetInput(const InputCellCorrespondencesType *input);

	 const InputCellCorrespondencesType * GetInput();
	 const InputCellCorrespondencesType * GetInput(unsigned int idx);

	  /** Specify the name of the output file to write. */
	  itkSetStringMacro(FileName);
	  itkGetStringMacro(FileName);

	  /** A special version of the Update() method for writers.  It
	  * invokes start and end events and handles releasing data. It
	  * eventually calls GenerateData() which does the actual writing. */
	  virtual void Write();

	  /** Aliased to the Write() method to be consistent with the rest of the
	  * pipeline. */
	  virtual void Update() ITK_OVERRIDE
	  {
	    this->Write();
	  }


protected:
	  CellCorrespondencesFileWriter();
	  ~CellCorrespondencesFileWriter();
	  virtual void PrintSelf(std::ostream & os, itk::Indent indent) const ITK_OVERRIDE;


private:
	  CellCorrespondencesFileWriter(const Self &) ITK_DELETE_FUNCTION;
	  void operator=(const Self &) ITK_DELETE_FUNCTION;

	  std::string         m_FileName;


};
}
#include "tttCellCorrespondencesFileWriter.hxx"
#endif /* MODULES_DATASTORAGE_INCLUDE_TTTCELLCORRESPONDENCESFILEWRITER_H_ */
