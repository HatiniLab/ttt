/*
 * tttAJGraphIOFactory.h
 *
 *  Created on: Feb 3, 2016
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHIOFACTORY_H_
#define MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHIOFACTORY_H_

#include "itkObject.h"
#include "itkMeshIOBase.h"
namespace ttt {
class AJGraphIOFactory: public itk::Object {
public:
	typedef AJGraphIOFactory Self;
	typedef itk::Object Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro(Self)
	;

	itkTypeMacro(AJGraphIOFactory, Object)
	;

	typedef ::ttt::AJGraphIOBase::Pointer AJGraphIOBasePointer;

	typedef enum {
		ReadMode, WriteMode
	} FileModeType;

	static AJGraphIOBasePointer CreateAJGraphIO(const char *path, FileModeType mode);

	static void RegisterBuiltInFactories();

protected:
	AJGraphIOFactory();
	~AJGraphIOFactory();

private:
	AJGraphIOFactory(const Self &) ITK_DELETE_FUNCTION;
	void operator=(const Self &) ITK_DELETE_FUNCTION;
};
} // end namespace itk

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHIOFACTORY_H_ */
