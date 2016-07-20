/*
 * CellVertex.h
 *
 *  Created on: Mar 23, 2015
 *      Author: morgan
 */

#ifndef CELL_H_
#define CELL_H_

#include <itkDataObject.h>
#include <itkMacro.h>
#include <itkObjectFactory.h>
#include <itkPoint.h>
#include <itkArray.h>
namespace ttt{
class Cell : public itk::Object{

public:
    typedef Cell Self;
    typedef itk::DataObject Superclass;
    typedef itk::SmartPointer<Self> Pointer;

    typedef itk::Point<double,3> PointType;
    typedef itk::Vector<double,3> VectorType;
    typedef itk::Array<double> DescriptorType;

    itkNewMacro(Self)

    itkGetMacro(Position,PointType)
    itkSetMacro(Position,PointType)


	itkGetMacro(Area,double);
	itkSetMacro(Area,double);


	itkGetMacro(XX,double);
	itkSetMacro(XX,double);

	itkGetMacro(XY,double);
	itkSetMacro(XY,double);

	itkGetMacro(YY,double);
	itkSetMacro(YY,double);

	itkGetMacro(PerimeterLength,double);
	itkSetMacro(PerimeterLength,double);

	itkGetMacro(Eccentricity,double);
	itkSetMacro(Eccentricity,double);

	typedef std::vector<unsigned long> PerimeterContainer;
    typedef PerimeterContainer::iterator PerimeterIterator;

	void AddAJVertexToPerimeter(unsigned long v ){
    	m_Perimeter.push_back(v);
    }

	PerimeterIterator PerimeterBegin(){
		return m_Perimeter.begin();
    }
	PerimeterIterator PerimeterEnd(){
		return m_Perimeter.end();
    }
protected:
	Cell(){
		m_Position.Fill(0);
		m_XX=0;
		m_XY=0;
		m_YY=0;
		m_Area=0;
		m_PerimeterLength=0;
		m_Eccentricity=0;
		m_Skewness=0;
		m_Perimeter.clear();
	}


private:

    PointType m_Position;

    double m_XX;
    double m_YY;
    double m_XY;
    double m_Area;
    double m_PerimeterLength;
    double m_Eccentricity;
    double m_Skewness;

    std::vector<unsigned long> m_Perimeter;

};


template<class CellType> void CellUnion(const typename CellType::Pointer & a, const typename CellType::Pointer & b,typename CellType::Pointer & result){

	typedef typename CellType::PerimeterIterator PerimeterIterator;

	PerimeterIterator itA = a->PerimeterBegin();

	typename CellType::PerimeterIterator itAEnd = a->PerimeterEnd();

	PerimeterIterator itB = b->PerimeterBegin();
	PerimeterIterator itBEnd = b->PerimeterEnd();

	std::vector<unsigned long > pointsInA;
	pointsInA.insert(pointsInA.begin(),itA,itAEnd);
	std::sort(pointsInA.begin(),pointsInA.end());

	std::vector<unsigned long  > pointsInB;
	pointsInB.insert(pointsInB.begin(),itB,itBEnd);
	std::sort(pointsInB.begin(),pointsInB.end());

	std::vector<unsigned long> common(2);
	std::set_intersection(pointsInA.begin(),pointsInA.end(),pointsInB.begin(),pointsInB.end(),common.begin());

	assert(common.size()==2);

	unsigned long point1 = common[0];
	unsigned long point2 = common[1];


	PerimeterIterator loc1A = std::find(itA,itAEnd,point1);
	PerimeterIterator loc2A	=std::find(itA,itAEnd,point2);

	assert(loc1A!=itAEnd);
	assert(loc2A!=itAEnd);

	bool aClockwise;
	PerimeterIterator startA, endA;

	startA=loc2A;
	endA=loc1A;

	if(loc2A==loc1A+1){
		aClockwise=1;
	}else if(loc1A==loc2A+1){
		aClockwise=0;
	}else if(loc1A==itA && loc2A==itAEnd-1){
		aClockwise=0;
	}else if(loc1A==itAEnd-1 && loc2A==itA){
		aClockwise=1;
	}

	PerimeterIterator loc1B = std::find(itB,itBEnd,point1);
	PerimeterIterator loc2B	=std::find(itB,itBEnd,point2);
	PerimeterIterator startB, endB;
	assert(loc1B!=itBEnd);
	assert(loc2B!=itBEnd);

	bool bClockwise;
	startB=loc1B;
	endB=loc2B;
	if(loc2B==loc1B+1){
		bClockwise=0;
	}else if(loc1B==loc2B+1){
		bClockwise=1;
	}else if(loc1B==itB && loc2B==itBEnd-1){
		bClockwise=1;
	}else if(loc1B==itBEnd-1 && loc2B==itB){
		bClockwise=0;
	}



	PerimeterIterator perimIter=startA;
	while(perimIter!=endA){

		result->AddAJVertexToPerimeter(*perimIter);

		if(aClockwise){
			++perimIter;

			if(perimIter==itAEnd){
				perimIter=itA;
			}
		}else{
			if(perimIter==itA){
				perimIter=itAEnd;
			}
			--perimIter;
		}
	}
	perimIter=startB;
	while(perimIter!=endB){


		result->AddAJVertexToPerimeter(*perimIter);

		if(bClockwise){
			++perimIter;

			if(perimIter==itBEnd){
				perimIter=itB;
			}
		}else{
			if(perimIter==itB){
				perimIter=itBEnd;
			}
			--perimIter;
		}
	}


}

}
#endif /* CELLVERTEX_H_ */
