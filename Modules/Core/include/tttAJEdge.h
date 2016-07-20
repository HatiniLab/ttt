#ifndef AJEDGE_H
#define AJEDGE_H

#include <itkDataObject.h>
#include <itkMacro.h>
#include <itkObjectFactory.h>
#include <itkPoint.h>
namespace ttt{
class AJEdge : public itk::Object{

public:
    typedef AJEdge Self;
    typedef itk::DataObject Superclass;
    typedef itk::SmartPointer<Self> Pointer;

    typedef itk::Array<double> DescriptorType;

    itkNewMacro(Self)


protected:
	AJEdge(){


	}
private:


};
}

#endif // AJVERTEX_H
