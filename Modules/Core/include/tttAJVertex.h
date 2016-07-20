#ifndef AJVERTEX_H
#define AJVERTEX_H

#include <itkDataObject.h>
#include <itkMacro.h>
#include <itkObjectFactory.h>
#include <itkPoint.h>
namespace ttt{
template<int dim> class AJVertex : public itk::DataObject{

public:
    typedef AJVertex Self;
    typedef itk::DataObject Superclass;
    typedef itk::SmartPointer<Self> Pointer;

    typedef itk::Point<double,dim> PointType;
    typedef itk::Vector<double,dim> VectorType;
    typedef itk::Array<double> DescriptorType;

    itkNewMacro(Self)

    itkGetMacro(Position,PointType)
    itkSetMacro(Position,PointType)

    itkGetMacro(Descriptor,DescriptorType)
    itkSetMacro(Descriptor,DescriptorType)


protected:
	AJVertex(){
		m_Position.Fill(0);
	}
private:

    PointType m_Position;
    DescriptorType m_Descriptor;


};
}
#endif // AJVERTEX_H
