/*
 * Tissue.h
 *
 *  Created on: May 5, 2015
 *      Author: morgan
 */

#ifndef _TTTTISSUE_H_
#define _TTTTISSUE_H_

#include <itkDataObject.h>
#include <itkObjectFactory.h>
#include <itkMacro.h>
namespace ttt{
template<class AJGraph,class CellGraph> class Tissue : public itk::DataObject{
public:
	typedef Tissue Self;
	typedef itk::SmartPointer<Self> Pointer;

	static const unsigned long NumDimensions=3;
	typedef AJGraph AJGraphType;
	typedef CellGraph CellGraphType;

	itkNewMacro(Self);

	itkGetObjectMacro(AJGraph,AJGraph);
	itkGetObjectMacro(CellGraph,CellGraph);

	itkSetObjectMacro(AJGraph,AJGraph);
	itkSetObjectMacro(CellGraph,CellGraph);

	typedef std::vector<typename AJGraph::AJVertexHandler> PerimeterContainer;
	typedef typename PerimeterContainer::iterator PerimeterIterator;

	inline PerimeterIterator PerimeterBegin(){
		return m_Perimeter.begin();
	}
	inline PerimeterIterator PerimeterEnd(){
		return m_Perimeter.end();
	}

	inline void AddAJVertexToPerimeter(const typename AJGraph::AJVertexHandler & ajvertex){
		m_Perimeter.push_back(ajvertex);
	}

protected:
	Tissue(){
		m_AJGraph = AJGraph::New();
		m_CellGraph = CellGraph::New();
	}
private:
	typename AJGraph::Pointer m_AJGraph;
	typename CellGraph::Pointer m_CellGraph;


	PerimeterContainer m_Perimeter;
};
}

#endif /* MODULES_CORE_INCLUDE_TTTTISSUEDESCRIPTOR_H_ */
