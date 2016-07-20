/*
 * CellGraph.h
 *
 *  Created on: Mar 23, 2015
 *      Author: morgan
 */

#ifndef CELLGRAPH_H_
#define CELLGRAPH_H_


#include <itkDataObject.h>
#include <itkMacro.h>
#include <itkObjectFactory.h>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace ttt{
template<class TCell> class CellGraph : public itk::DataObject {


private:

    struct CellVertexPropertyTag {
        typedef boost::vertex_property_tag kind;
    };


    typedef boost::property<CellVertexPropertyTag, typename TCell::Pointer,

			boost::property<boost::vertex_index_t, int> >   VertexProperty;

    typedef boost::property<boost::edge_index_t,int> EdgeProperty;
public:
    typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS,VertexProperty,EdgeProperty,boost::no_property> BoostGraphType;

public:

    typedef TCell CellType;
    typedef CellGraph Self;
    typedef itk::LightObject Superclass;
    typedef itk::SmartPointer<Self> Pointer;


    typedef typename boost::graph_traits<BoostGraphType>::vertex_descriptor CellVertexHandler;
    typedef typename boost::graph_traits<BoostGraphType>::edge_descriptor CellEdgeHandler;

    typedef typename boost::graph_traits<BoostGraphType>::vertex_iterator VertexIterator;
    typedef typename boost::graph_traits<BoostGraphType>::edge_iterator EdgeIterator;

    typedef typename boost::graph_traits<BoostGraphType>::adjacency_iterator AdjacencyIterator;
private:

    BoostGraphType m_Graph;
    CellVertexHandler m_OuterFace;


public:

    itkNewMacro(Self)

   virtual  CellVertexHandler GetCellEdgeSource(const CellEdgeHandler & source){
        return boost::source(source,m_Graph);
    }
    virtual CellEdgeHandler GetCellEdgeHandler(const CellVertexHandler & source,const CellVertexHandler & target){
        return boost::edge(source,target,m_Graph).first;

    }
    virtual void SetOutboundedFace(const CellVertexHandler & handler){
    	m_OuterFace=handler;
    }
    virtual CellVertexHandler GetUnboundedFace() const{
    	return m_OuterFace;
    }

    virtual CellVertexHandler GetCellEdgeTarget(const CellEdgeHandler & target){
        return boost::target(target,m_Graph);
    }

   virtual typename TCell::Pointer GetCell(const CellVertexHandler & vertex){
        return boost::get(CellVertexPropertyTag(),m_Graph,vertex);

    }

    virtual int CellVertexDegree(const CellVertexHandler & vertex){
        return boost::degree(vertex,m_Graph);
    }

    virtual AdjacencyIterator AdjacentCellsBegin(const CellVertexHandler & vertex){
        return boost::adjacent_vertices(vertex,m_Graph).first;
    }

    virtual AdjacencyIterator AdjacentCellsEnd(const CellVertexHandler & vertex){
        return boost::adjacent_vertices(vertex,m_Graph).second;
    }

   virtual CellVertexHandler AddCell(const typename CellType::Pointer & cell){

	   CellVertexHandler result=boost::add_vertex(m_Graph);

        boost::get(CellVertexPropertyTag(),m_Graph,result)=cell;
        return result;

    }
    virtual void DeleteCell(const CellVertexHandler & vertex){
        boost::clear_vertex(vertex,m_Graph);
        boost::remove_vertex(vertex,m_Graph);
    }

    virtual void DeleteCellEdge(const CellEdgeHandler & edge){
        boost::remove_edge(edge,m_Graph);
    }


    virtual CellEdgeHandler AddCellEdge(const CellVertexHandler & a, const CellVertexHandler & b){
    	CellEdgeHandler result= boost::add_edge(a,b,m_Graph).first;

        return result;
    }



    unsigned long GetNumberOfCells(){
        return boost::num_vertices(m_Graph);
    }

    unsigned long GetNumEdges(){
        return boost::num_edges(m_Graph);
    }

    EdgeIterator CellEdgesBegin(){
        return boost::edges(m_Graph).first;
    }


    EdgeIterator CellEdgesEnd(){
        return boost::edges(m_Graph).second;
    }

    VertexIterator CellsBegin(){
        return boost::vertices(m_Graph).first;
    }

    VertexIterator CellsEnd(){
        return boost::vertices(m_Graph).second;
    }
    BoostGraphType & GetBoostGraph(){
    	return m_Graph;
    }
protected:
    CellGraph(){

    }

};
}
#endif /* CELLGRAPH_H_ */
