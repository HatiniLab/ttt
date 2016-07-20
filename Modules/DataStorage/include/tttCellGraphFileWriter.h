/*
 * tttCellGraphFileWriter.h
 *
 *  Created on: Jun 28, 2016
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEWRITER_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEWRITER_H_

#include <itkProcessObject.h>

namespace ttt{

template<class TInputCellGraph> class CellGraphFileWriter : public itk::ProcessObject{


public:
	typedef CellGraphFileWriter Self;
	typedef itk::ProcessObject Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer< const Self > ConstPointer;

	itkNewMacro(Self);

	itkTypeMacro(CellGraphFileWriter, ProcessObject);

	  /** Some convenient typedefs. */
	  typedef TInputCellGraph                         InputCellGraphType;
	  typedef typename InputCellGraphType::Pointer    InputCellGraphPointer;

	  typedef typename InputCellGraphType::AJVertexType    InputAJVertexType;
	  typedef typename InputCellGraphType::AJEdgeType    InputAJEdgeType;


	  using Superclass::SetInput;
	  void SetInput(const InputCellGraphType *input);

	 const InputCellGraphType * GetInput();
	 const InputCellGraphType * GetInput(unsigned int idx);

	  /** Specify the name of the output file to write. */
	  itkSetStringMacro(FileName);
	  itkGetStringMacro(FileName);


	  void SetFileTypeAsASCII(){m_FileTypeIsBINARY = false; }
	  void SetFileTypeAsBINARY(){m_FileTypeIsBINARY = true; }

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

	  /** Set the compression On or Off */
	  itkSetMacro(UseCompression, bool);
	  itkGetConstReferenceMacro(UseCompression, bool);
	  itkBooleanMacro(UseCompression);

	protected:
	  CellGraphFileWriter();
	  ~CellGraphFileWriter();
	  virtual void PrintSelf(std::ostream & os, itk::Indent indent) const ITK_OVERRIDE;

	  template< typename Output >
	  void CopyPointsToBuffer(Output *data);

	  template< typename Output >
	  void CopyCellsToBuffer(Output *data);

	  template< typename Output >
	  void CopyPointDataToBuffer(Output *data);

	  template< typename Output >
	  void CopyCellDataToBuffer(Output *data);

	  void WriteCellGraph();

	private:
	  CellGraphFileWriter(const Self &) ITK_DELETE_FUNCTION;
	  void operator=(const Self &) ITK_DELETE_FUNCTION;

	  std::string         m_FileName;
	  bool                m_UserSpecifiedAJGraphIO;    // track whether the MeshIO is
	                                                // user specified
	  bool                m_FactorySpecifiedAJGraphIO; // track whether the factory
	                                                // mechanism set the MeshIO
	  bool                m_UseCompression;
	  bool                m_FileTypeIsBINARY;
};
}

#include <tttCellGraphFileWriter.hxx>

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEWRITER_H_ */
