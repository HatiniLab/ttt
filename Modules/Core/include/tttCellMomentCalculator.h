/*
 * CellMomentCalculator.h
 *
 *  Created on: May 1, 2014
 *      Author: morgan
 */

#ifndef CELLMOMENTCALCULATOR_H_
#define CELLMOMENTCALCULATOR_H_
//#include "Feature.h"
//#include "FeatureMap.h"
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_matrix_fixed.h>
#include <vnl/algo/vnl_symmetric_eigensystem.h>
template<class TissueDescriptorType> class CellMomentCalculator{
#if 1
public:
	static const int NumDimensions = TissueDescriptorType::NumDimensions;

	typedef typename TissueDescriptorType::CellGraphType::CellType CellType;
	typedef typename TissueDescriptorType::CellGraphType::CellVertexHandler CellVertexHandler;
	typedef itk::Point<double,3> PointType;
	typedef itk::Vector<double,3> VectorType;

#if 0
	typedef FeatureMap<CellVertexHandler,double> XXFeatureMapType;
	typedef FeatureMap<CellVertexHandler,double> YYFeatureMapType;
	typedef FeatureMap<CellVertexHandler,double> XYFeatureMapType;
	typedef FeatureMap<CellVertexHandler,double> AreaFeatureMapType;
	typedef FeatureMap<CellVertexHandler,double> PerimeterFeatureMapType;
	typedef FeatureMap<CellVertexHandler,double> NormalFeatureMapType;

	typedef FeatureMap<CellVertexHandler,itk::Point<double,3> > CentroidFeatureMapType;
#endif

private:
	typename TissueDescriptorType::Pointer m_TissueDescriptor;

public:

	void SetTissueDescriptor(const typename TissueDescriptorType::Pointer & tissueDescriptor){
		m_TissueDescriptor=tissueDescriptor;
	}

	void GetCellMoments(const typename CellType::Pointer & cell) {
		int total=0;
		std::vector<PointType> points,centeredPoints;
		PointType meanPoint;
		meanPoint.Fill(0);


		for(typename CellType::PerimeterIterator perimeterIt= cell->PerimeterBegin();perimeterIt!=cell->PerimeterEnd();++perimeterIt){
			auto position = m_TissueDescriptor->GetAJGraph()->GetAJVertex(*perimeterIt)->GetPosition();
			points.push_back(position);
			meanPoint[0]+=position[0];
			meanPoint[1]+=position[1];
			meanPoint[2]+=position[2];
			total++;
		}

		meanPoint[0]/=total;
		meanPoint[1]/=total;
		meanPoint[2]/=total;
		//std::cout << "MeanPoint " << meanPoint << std::endl;
		vnl_matrix<double> A(total,3);

		int row=0;

		for(std::vector<PointType>::iterator pointIt = points.begin();pointIt!=points.end();++pointIt){
			PointType diff = *pointIt-meanPoint;
			centeredPoints.push_back(diff);
			A(row,0)=diff[0];
			A(row,1)=diff[1];
			A(row,2)=diff[2];
			row++;

		}
		//std::cout << "A:" << std::endl;
		//std::cout << A << std::endl;

		vnl_matrix_fixed<double,3,3> S= A.transpose()*A; //FEATURE: Scatter Matrix

		vnl_symmetric_eigensystem<double > eigensystem(S);

		vnl_vector_fixed<double,3> normalPlane = eigensystem.get_eigenvector(0); //FEATURE: Supporting plane

		std::vector<PointType> projectedPoints;
		//std::cout << "Projections" << std::endl;
		for(std::vector<PointType>::iterator pointIt = centeredPoints.begin();pointIt!=centeredPoints.end();++pointIt){

			PointType a = *pointIt;
			PointType proj;

			double dot = a[0]*normalPlane[0] + a[1]*normalPlane[1] + a[2]*normalPlane[2];
			proj[0]=a[0]-dot*normalPlane[0];
			proj[1]=a[1]-dot*normalPlane[1];
			proj[2]=a[2]-dot*normalPlane[2];
			projectedPoints.push_back(proj);
			//std::cout << proj << std::endl;
		}

		std::vector<vnl_vector<double> > transformedPoints;

		//std::cout << "Transformed" << std::endl;

		for(std::vector<PointType>::iterator pointIt = projectedPoints.begin();pointIt!=projectedPoints.end();++pointIt){

			PointType a = *pointIt;
			vnl_vector<double> vec(3);
			vec(0)=a[0]; vec(1)=a[1];vec(2)=a[2];

			vnl_vector<double> ap= eigensystem.V.transpose() * vec;
			vnl_vector<double> transformed(2);
			transformed(0)=ap[1];
			transformed(1)=ap[2];
			transformedPoints.push_back(transformed);
			//std::cout << transformed << std::endl;

		}

		transformedPoints.push_back(transformedPoints[0]);


		vnl_vector<double> centroid2D(2);

		centroid2D.fill(0);
		double perimeter=0;
		double area=0;

		double xx=0;
		double xy=0;
		double yy=0;

		for(int i=0;i<transformedPoints.size()-1;i++){
			perimeter+=(transformedPoints[i+1]-transformedPoints[i]).magnitude();

			double common= (transformedPoints[i](0)*transformedPoints[i+1](1) - transformedPoints[i+1](0)*transformedPoints[i](1));

			area+=common;
			centroid2D(0)+=(transformedPoints[i](0)+ transformedPoints[i+1](0))*common;
			centroid2D(1)+=(transformedPoints[i](1)+ transformedPoints[i+1](1))*common;
			xx+= (pow(transformedPoints[i](0),2)+transformedPoints[i](0)* transformedPoints[i+1](0) + pow(transformedPoints[i+1](0),2) )*common;
			yy+= (pow(transformedPoints[i](1),2)+transformedPoints[i](1)* transformedPoints[i+1](1) + pow(transformedPoints[i+1](1),2) )*common;
			xy+= (2*transformedPoints[i](0)*transformedPoints[i](1) + transformedPoints[i](0)*transformedPoints[i+1](1) +  transformedPoints[i+1](0)*transformedPoints[i](1) )*common;
			//std::cout << common << std::endl;
		}

		area=fabs(area/2);

		//std::cout << "Area:" << area << std::endl;
		//std::cout << "Perimeter:" << perimeter << std::endl;

		cell->SetPerimeterLength(fabs(perimeter));
		cell->SetArea(area);

		centroid2D(0)=centroid2D(0)/(6*area);
		centroid2D(1)=centroid2D(1)/(6*area);
		//std::cout << "Centroid2D" << std::endl;
		//std::cout<< centroid2D << std::endl;

		xx=fabs(xx/(12*area));
		yy=fabs(yy/(12*area));
		xy=xy/(24*area);
		//std::cout << "XX:" << xx << std::endl;
		//std::cout << "XY:" << xy << std::endl;
		//std::cout << "YY:" << yy << std::endl;

		cell->SetXX(xx);
		cell->SetYY(yy);
		cell->SetXY(xy);

		vnl_vector<double> cp(3);
		cp(0)=0;
		cp(1)=centroid2D(0);
		cp(2)=centroid2D(1);

		vnl_vector<double> cu = eigensystem.V*cp;

		//std::cout << "CU" << std::endl;
		//std::cout << cu << std::endl;

		PointType cg;
		PointType centroid3D;

		centroid3D[0]=cu[0]+meanPoint[0];
		centroid3D[1]=cu[1]+meanPoint[1];
		centroid3D[2]=cu[2]+meanPoint[2];

		//std::cout << "Centroid3D" << centroid3D << std::endl;


		cell->SetPosition(centroid3D);


	}
	void Compute(){
		std::vector<PointType> points,centeredPoints;


		for(auto it=m_TissueDescriptor->GetCellGraph()->CellsBegin();it!=m_TissueDescriptor->GetCellGraph()->CellsEnd();++it){
			this->GetCellMoments(m_TissueDescriptor->GetCellGraph()->GetCell(*it));
		}
	}

#endif
};



#endif /* CELLMOMENTCALCULATOR_H_ */
