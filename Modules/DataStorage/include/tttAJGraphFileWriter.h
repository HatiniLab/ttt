/*
 * tttAJGraphWriter.h
 *
 *  Created on: Feb 3, 2016
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHFILEWRITER_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHFILEWRITER_H_



#include "itkProcessObject.h"
#include "tttAJGraphIOBase.h"
#include "itkExceptionObject.h"
namespace ttt{

class AJGraphFileWriterException:public itk::ExceptionObject {
public:
itkTypeMacro(MeshFileWriterException, itk::ExceptionObject);

AJGraphFileWriterException(const char *file, unsigned int line,
                           const char *message = "Error in IO",
                           const char *loc = "Unknown"):
     itk::ExceptionObject(file, line, message, loc)
   {}

AJGraphFileWriterException(const std::string & file, unsigned int line,
                              const char *message = "Error in IO",
                              const char *loc = "Unknown"):
        ExceptionObject(file, line, message, loc)
      {}
};
template<class TInputAJGraph> class AJGraphFileWriter : public itk::ProcessObject{

public:
	typedef AJGraphFileWriter Self;
	typedef itk::ProcessObject Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer< const Self > ConstPointer;

	itkNewMacro(Self);

	itkTypeMacro(AJGraphFileWriter, ProcessObject);

	  /** Some convenient typedefs. */
	  typedef TInputAJGraph                         InputAJGraphType;
	  typedef typename InputAJGraphType::Pointer    InputAJGraphPointer;

	  typedef typename InputAJGraphType::AJVertexType    InputAJVertexType;
	  typedef typename InputAJGraphType::AJEdgeType    InputAJEdgeType;


	  using Superclass::SetInput;
	  void SetInput(const InputAJGraphType *input);

	 const InputAJGraphType * GetInput();
	 const InputAJGraphType * GetInput(unsigned int idx);

	  /** Specify the name of the output file to write. */
	  itkSetStringMacro(FileName);
	  itkGetStringMacro(FileName);

	  /** Set/Get the MeshIO helper class. Usually this is created via the object
	  * factory mechanism that determines whether a particular MeshIO can
	  * write a certain file. This method provides a way to get the MeshIO
	  * instance that is created, or one can be manually set where the
	  * IO factory mechanism may not work (some mesh files with non-standard filename suffix's.
	  * If the user specifies the MeshIO, we assume she makes the
	  * correct choice and will allow a file to be created regardless of
	  * the file extension. If the factory has set the MeshIO, the
	  * extension must be supported by the specified MeshIO. */
	  void SetAJGraphIO(AJGraphIOBase *io)
	  {
	    if ( this->m_AJGraphIO != io )
	      {
	      this->Modified();
	      this->m_AJGraphIO = io;
	      }
	    m_FactorySpecifiedAJGraphIO = false;
	    m_UserSpecifiedAJGraphIO = true;
	  }

	  itkGetModifiableObjectMacro(AJGraphIO, AJGraphIOBase);

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
	  AJGraphFileWriter();
	  ~AJGraphFileWriter();
	  virtual void PrintSelf(std::ostream & os, itk::Indent indent) const ITK_OVERRIDE;

	  template< typename Output >
	  void CopyPointsToBuffer(Output *data);

	  template< typename Output >
	  void CopyCellsToBuffer(Output *data);

	  template< typename Output >
	  void CopyPointDataToBuffer(Output *data);

	  template< typename Output >
	  void CopyCellDataToBuffer(Output *data);

	  void WriteAJGraph();

	private:
	  AJGraphFileWriter(const Self &) ITK_DELETE_FUNCTION;
	  void operator=(const Self &) ITK_DELETE_FUNCTION;

	  std::string         m_FileName;
	  AJGraphIOBase::Pointer m_AJGraphIO;
	  bool                m_UserSpecifiedAJGraphIO;    // track whether the MeshIO is
	                                                // user specified
	  bool                m_FactorySpecifiedAJGraphIO; // track whether the factory
	                                                // mechanism set the MeshIO
	  bool                m_UseCompression;
	  bool                m_FileTypeIsBINARY;
};
}
#include <tttAJGraphFileWriter.hxx>


#endif /* MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHFILEWRITER_H_ */
