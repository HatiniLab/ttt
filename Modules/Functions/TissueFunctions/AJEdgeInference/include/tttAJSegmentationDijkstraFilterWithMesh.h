//    This file is part of TTT Tissue Tracker.
//
//    TTT Tissue Tracker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    TTT Tissue Tracker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with TTT Tissue Tracker.  If not, see <http://www.gnu.org/licenses/>.

/** \addtogroup TTTAdherensJunctionSegmentation
 *  @{
 */

#ifndef ADHERENSJUNCTIONSEGMENTATIONDIJKSTRACOMMAND_H_
#define ADHERENSJUNCTIONSEGMENTATIONDIJKSTRACOMMAND_H_


#include "itkImage.h"
#include "itkSmartPointer.h"
#include "tttAJGraphToAJGraphFilter.h"

//#include "itkPointSet.h"
//#include "itkImage.h"
#include "heapplus.h"
#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>
#include <itkIndex.h>
namespace std{

template<int dim> static bool operator<(const  itk::Index<dim> & a, const itk::Index<dim> & b){
	for(int i=0;i<dim;i++){
		if(a[i]<b[i]){
			return true;
		}else if(a[i]>b[i]){
			return false;
		}
	}
	return false;
}

}
namespace ttt{
/**
 * TODO
 * \class AdherensJunctionSegmentationCommand
 */
template<class TInputAJGraph,class TPlatenessImage,class TVertexnessImage,class TInputMesh,class TOutputAJGraph>
class AJSegmentationDijkstraFilterWithMesh: public AJGraphToAJGraphFilter<TInputAJGraph,TOutputAJGraph> {

public:
	typedef AJSegmentationDijkstraFilterWithMesh Self;
	typedef AJGraphToAJGraphFilter<TInputAJGraph,TOutputAJGraph> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;


	const static unsigned int NumDimensions = TPlatenessImage::ImageDimension;
	/**
	 * TODO
	 */

	typedef TInputAJGraph InputAJGraphType;
	typedef TPlatenessImage PlatenessImageType;
	typedef TVertexnessImage VertexnessImageType;
	typedef TOutputAJGraph OutputAJGraphType;
	typedef TInputMesh InputMeshType;

	typedef typename TPlatenessImage::IndexType IndexType;
	itkNewMacro(Self);

	itkTypeMacro(AJSegmentationDijkstraFilter, AJGraphToAJGraphFilter);

	itkGetMacro(StoppingValue,double);
	itkSetMacro(StoppingValue,double);

	void SetInputAJGraph(const TInputAJGraph * ajgraph);
	void SetPlatenessImage(const PlatenessImageType * plateness);
	void SetVertexnessImage(const VertexnessImageType * vertexness);
	void SetInputMesh(const TInputMesh * inputMesh);

	const TInputAJGraph * GetInputAJGraph(){
		return dynamic_cast<TInputAJGraph*>(this->itk::ProcessObject::GetInput(0));
	}
	const TPlatenessImage * GetPlatenessImage(){
		return dynamic_cast<TPlatenessImage*>(this->itk::ProcessObject::GetInput(1));
	}
	const TVertexnessImage * GetVertexnessImage(){
		return dynamic_cast<TVertexnessImage*>(this->itk::ProcessObject::GetInput(2));
	}

	const TInputMesh * GetMesh(){
		return dynamic_cast<TInputMesh*>(this->itk::ProcessObject::GetInput(3));
	}

protected:
	/**
	 * TODO
	 */
	AJSegmentationDijkstraFilterWithMesh();
	/**
	 * TODO
	 */
	virtual ~AJSegmentationDijkstraFilterWithMesh();

protected:
	/**
	 * TODO
	 */
	virtual void GenerateData();

private:

	typedef itk::Image<typename TPlatenessImage::PixelType,NumDimensions> SpeedImageType;
	typedef itk::Image<typename TPlatenessImage::PixelType,NumDimensions> DistanceImageType;

	typedef itk::Image<int,NumDimensions> LabelImageType;

	typedef itk::Image<unsigned char,NumDimensions> ColorImageType;


	typedef itk::Image<typename TPlatenessImage::PixelType,NumDimensions> LevelSetImageType;

	typedef typename LevelSetImageType::IndexType Index;
	typedef typename LevelSetImageType::OffsetType Offset;

	typename SpeedImageType::Pointer m_Speed;

	typename LevelSetImageType::Pointer m_LevelSet;

	typename LabelImageType::Pointer m_Labels;
	double m_LevelSetThreshold;

	double m_StoppingValue;
public:


	inline void SetLimit(double stoppingValue){
		m_StoppingValue=stoppingValue;
	}

private:
	void PruneLevelSet(double threshold);
	void ComputeLevelsetThreshold(double percentage);
	void ComputeSpeedImage();
	void BuildGraph();
	void AllocateLabelsImage();
	void StoreLabels();
	void InitDefGraph();
	void DoFastMarching();
	void DoVertexSegmentation();
	void GetNeighbors(const IndexType & index,std::vector<IndexType > & neighbors);
	//double ComputePath(const SkeletonVertexType & a, const SkeletonVertexType & b);
};
}
#include "tttAJSegmentationDijkstraFilterWithMesh.hpp"
#endif
/** @}*/
