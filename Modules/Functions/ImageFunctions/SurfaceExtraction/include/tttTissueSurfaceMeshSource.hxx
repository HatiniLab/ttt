/*
 * TissueSurfaceMeshSource.hxx
 *
 *  Created on: Jul 8, 2015
 *      Author: morgan
 */

#ifndef TISSUESURFACEMESHSOURCE_HXX_
#define TISSUESURFACEMESHSOURCE_HXX_

#include <itkPointsLocator.h>
#include "tttTissueSurfaceMeshSource.h"
#include <vnl/vnl_least_squares_cost_function.h>
#include <vnl/algo/vnl_lbfgsb.h>
#include "itkImageFileWriter.h"


template< typename TInputImage,typename TOutputMesh>
ttt::TissueSurfaceMeshSource< TInputImage, TOutputMesh>
::TissueSurfaceMeshSource() {
	this->SetNumberOfRequiredInputs(1);
	m_FirstGradientFilter= GradientImageFilterType::New();
	m_FirstGradientFilter->SetUseImageSpacing(true);
	m_FirstGradientFilter->SetOrder(1);
	m_FirstGradientFilter->SetDirection(2);


	m_SecondGradientFilter= GradientImageFilterType::New();
	m_SecondGradientFilter->SetUseImageSpacing(true);
	m_SecondGradientFilter->SetOrder(2);
	m_SecondGradientFilter->SetDirection(2);

	m_Interpolator = InterpolatorType::New();
	m_FirstGradientInterpolator = GradientInterpolatorType::New();
	m_SecondGradientInterpolator = GradientInterpolatorType::New();

	m_NumberOfIterations=0;
	//m_NumberOfIterations=100;
	m_CurrentIteration=0;
	m_NumberOfCells=0;
	m_NumberOfPoints=0;
	//m_Lambda=1000000000000;//std::pow(2,10);
	m_Lambda=100000;
	//m_Lambda=std::pow(2,10);
	//m_LambdaL1=std::pow(2,-20);

	m_Rho=1.0;
	m_Alpha=std::pow(2,4);
	m_UseInitialMesh=false;
	m_ObjectiveFunction=0;
}

template< typename TInputImage,typename TOutputMesh>
ttt::TissueSurfaceMeshSource< TInputImage, TOutputMesh>
::~TissueSurfaceMeshSource() {


}


/** Generate the data */
template< typename TInputImage,typename TOutputMesh>
void
ttt::TissueSurfaceMeshSource< TInputImage, TOutputMesh>
::InitInternalMesh(){
	m_InternalMesh= InternalMeshType::New();

	m_InternalMesh->GetPoints()->Reserve(this->GetInput()->GetLargestPossibleRegion().GetSize(0)*this->GetInput()->GetLargestPossibleRegion().GetSize(1));

	typedef itk::Image<unsigned long,2> IndexImageType;
	IndexImageType::Pointer indexImage = IndexImageType::New();
	IndexImageType::RegionType region;
	region.SetIndex(0,0);
	region.SetIndex(1,0);
	region.SetSize(0,this->GetInput()->GetLargestPossibleRegion().GetSize(0));
	region.SetSize(1,this->GetInput()->GetLargestPossibleRegion().GetSize(1));
	indexImage->SetRegions(region);

	indexImage->Allocate();


	if(m_UseInitialMesh){
#if 0
		typedef itk::Image<float,2> HeightImageType;
		typename HeightImageType::Pointer heightImage = HeightImageType::New();
		heightImage->SetRegions(region);
		typename HeightImageType::SpacingType heightSpacing;
		heightSpacing[0]=this->GetInput()->GetSpacing()[0];
		heightSpacing[1]=this->GetInput()->GetSpacing()[1];
		typename HeightImageType::PointType heightOrigin;
		heightOrigin[0]=this->GetInput()->GetOrigin()[0];
		heightOrigin[1]=this->GetInput()->GetOrigin()[1];

		heightImage->SetSpacing(heightSpacing);
		heightImage->SetOrigin(heightOrigin);

		heightImage->Allocate();

		heightImage->FillBuffer(0.0);
#endif
		typename TInputImage::ConstPointer input = this->GetInput();
		typedef itk::PointsLocator<typename InternalMeshType::PointsContainer> PointsLocatorType;



		for(int r=0;r<this->GetInput()->GetLargestPossibleRegion().GetSize(0);r++){
			for(int c=0;c<this->GetInput()->GetLargestPossibleRegion().GetSize(1);c++){

				typename InputImageType::IndexType index;
				typename InputImageType::PointType point;

				index[0]=r;
				index[1]=c;
				index[2]=0;

				this->GetInput()->TransformIndexToPhysicalPoint(index,point);

				m_InternalMesh->SetPoint(m_NumberOfPoints,point);
				m_InternalMesh->SetPointData(m_NumberOfPoints,0.0);

				typename IndexImageType::IndexType index2;
				index2[0]=r;
				index2[1]=c;
				indexImage->SetPixel(index2,m_NumberOfPoints);

				m_NumberOfPoints++;
			}
		}

		{
			typename PointsLocatorType::Pointer pointsLocator= PointsLocatorType::New();
			pointsLocator->SetPoints(m_InputMesh->GetPoints());
			pointsLocator->Initialize();
			double change =1;
			while(change>0){
				change=0;
					 // Access points
				typedef typename InputMeshType::PointsContainer::Iterator     PointsIterator;

				PointsIterator  pointIterator = this->m_InternalMesh->GetPoints()->Begin();

				PointsIterator end = m_InternalMesh->GetPoints()->End();

				while( pointIterator != end ){
					typename InternalMeshType::PointType & p = pointIterator.Value();  // access the point
					auto nearestPointId = pointsLocator->FindClosestPoint(p);

					typename InputMeshType::PointType nearestPoint;
					m_InputMesh->GetPoint(nearestPointId,&nearestPoint);
					//std::cout << p << " " << nearestPoint << std::endl;
					change+=std::abs(nearestPoint[2]-p[2]);

					p[2]=nearestPoint[2];

					++pointIterator;                                // advance to next point
				}
				std::cout << "Change: " << change << std::endl;
			}
		}

#if 0
		{
			 // Access points
			typedef typename InputMeshType::PointsContainer::Iterator     PointsIterator;

			PointsIterator  pointIterator = this->m_InputMesh->GetPoints()->Begin();

			PointsIterator end = m_InputMesh->GetPoints()->End();


			while( pointIterator != end ){
				typename InputMeshType::PointType p = pointIterator.Value();  // access the point

				//std::cout << p << std::endl;

				typename HeightImageType::PointType point2D;
				point2D[0]=p[0];
				point2D[1]=p[1];
				typename HeightImageType::IndexType index2D;
				heightImage->TransformPhysicalPointToIndex(point2D,index2D);

				heightImage->SetPixel(index2D,p[2]);

				++pointIterator;                                // advance to next point
			}
		}
#endif
#if 0

#endif

	}else{
		for(int r=0;r<this->GetInput()->GetLargestPossibleRegion().GetSize(0);r++){
			for(int c=0;c<this->GetInput()->GetLargestPossibleRegion().GetSize(1);c++){

				typename IndexImageType::IndexType index;
				typename TInputImage::PointType point;

				index[0]=r;
				index[1]=c;
				double pointValue=0;

					typename TInputImage::IndexType meshIndex;
					meshIndex[0]=r;
					meshIndex[1]=c;
					double max=0;
					int maxLoc=0;
				for(int h=0;h<this->GetInput()->GetLargestPossibleRegion().GetSize(2);h++){
					meshIndex[2]=h;
					double value = this->GetInput()->GetPixel(meshIndex);
					if(value>max){
						max=value;
						maxLoc=h;
					}
				}
				meshIndex[2]=maxLoc;
				//meshIndex[2]=0;
				//meshIndex[2]=(this->GetInput()->GetLargestPossibleRegion().GetSize(2)-1);
				//pointValue=max;
				pointValue=0;
				this->GetInput()->TransformIndexToPhysicalPoint(meshIndex,point);


				m_InternalMesh->SetPoint(m_NumberOfPoints,point);
				m_InternalMesh->SetPointData(m_NumberOfPoints,pointValue);

				indexImage->SetPixel(index,m_NumberOfPoints);

				m_NumberOfPoints++;

			}

		}
	}

	typedef typename InternalMeshType::CellType::CellAutoPointer         CellAutoPointer;

	for(int r=0;r<this->GetInput()->GetLargestPossibleRegion().GetSize(0)-1;r++){
		for(int c=0;c<this->GetInput()->GetLargestPossibleRegion().GetSize(1)-1;c++){
			CellAutoPointer collineR,collineC,collineRC;
			collineR.TakeOwnership(new LineType);
			collineC.TakeOwnership(new LineType);
			collineRC.TakeOwnership(new LineType);
			typename IndexImageType::IndexType index;
			index[0] = r;
			index[1] = c;
			unsigned int centerId = indexImage->GetPixel(index);
			index[0] = r+1;
			index[1] = c;
			unsigned int rId = indexImage->GetPixel(index);

			collineR->SetPointId(0, centerId); // line between points 0 and 1
			collineR->SetPointId(1, rId);
			m_InternalMesh->SetCell( m_NumberOfCells++, collineR );

			index[0] = r;
			index[1] = c+1;
			unsigned int cId = indexImage->GetPixel(index);
			collineC->SetPointId(0, centerId); // line between points 0 and 1
			collineC->SetPointId(1, cId);
			m_InternalMesh->SetCell( m_NumberOfCells++, collineC );
#if 0
			index[0] = r+1;
			index[1] = c+1;
			unsigned int rcId = indexImage->GetPixel(index);

			collineRC->SetPointId(0, centerId); // line between points 0 and 1
			collineRC->SetPointId(1, rcId);

			m_InternalMesh->SetCell( m_NumberOfCells++, collineRC );
#endif

		}
	}
}

/** Generate the data */
template< typename TInputImage,typename TOutputMesh>
bool
ttt::TissueSurfaceMeshSource< TInputImage, TOutputMesh>
::StoppingCriteria()
{
	return m_CurrentIteration >= m_NumberOfIterations;


}
#if 1
template< typename TInputImage, typename TOutputMesh>
void
ttt::TissueSurfaceMeshSource< TInputImage,TOutputMesh>
::ZStep()
{
	double maxZ = this->GetInput()->GetSpacing()[2]*(this->GetInput()->GetLargestPossibleRegion().GetSize()[2]-1);

	{
	 // Access points
		typedef typename InternalMeshType::PointsContainer::Iterator     PointsIterator;

		PointsIterator  pointIterator = m_InternalMesh->GetPoints()->Begin();
		auto  pointDataIterator = m_InternalMesh->GetPointData()->Begin();

		PointsIterator end = m_InternalMesh->GetPoints()->End();
		auto pointEnd = m_InternalMesh->GetPointData()->End();

		while( pointIterator != end ){

			unsigned long index = pointIterator.Index();
			auto  & point = pointIterator.Value();
			double sum = m_XSupport[index]+ (1/m_Rho)*m_LambdaSupport[index];
			int total=1;

			for(auto it = m_XReg[index].begin();it!=m_XReg[index].end();it++){

				sum=sum+it->second + (1/m_Rho)*m_LambdaReg[index][it->first];
				total++;
			}
			sum=sum/total;

			if(sum<=0) sum=0;
			if(sum>=maxZ) sum=maxZ;

			point[2]=sum;



			++pointIterator;

		}
	}
}
#endif
#if 1
template<class InterpolatorType, class GradientInterpolatorType,class PointType> struct SupportProximalMapping : public vnl_cost_function {

	typename InterpolatorType::Pointer m_Interpolator;
	typename GradientInterpolatorType::Pointer m_GradientInterpolator;
	double m_Y;
	PointType m_Z;
	double m_Rho;

	SupportProximalMapping():  vnl_cost_function(1) {

	}

	void compute(	vnl_vector< double > const & 	x,double * 	f,vnl_vector< double > * 	g)    {
		PointType xPoint = m_Z;
		xPoint[2]=x[0];

		*f = -m_Interpolator->Evaluate(xPoint) + m_Y*(x[0]-m_Z[2]) + (m_Rho/2)*pow(x[0] - m_Z[2],2);
		//std::cout<< xPoint <<  " " << *f << std::endl;
		if(g){
			auto gradient =-m_GradientInterpolator->Evaluate(xPoint) + m_Y + m_Rho*(x[0] - m_Z[2]);

			(*g)[0]= gradient;

		}

    }

};
#endif
#if 1
template<typename TInputImage,class TOutputMesh>
void ttt::TissueSurfaceMeshSource<TInputImage,TOutputMesh>::LambdaStep() {
	{
		// Access points
		typedef typename InternalMeshType::PointsContainer::Iterator PointsIterator;

		PointsIterator pointIterator = m_InternalMesh->GetPoints()->Begin();
		PointsIterator end = m_InternalMesh->GetPoints()->End();

		while (pointIterator != end) {
			//u = x - Lambda

			auto x = pointIterator.Value();
			auto lambda = m_LambdaSupport[pointIterator.Index()];
			lambda = lambda + m_Rho*(m_XSupport[pointIterator.Index()]-x[2]);

			m_LambdaSupport[pointIterator.Index()] = lambda;
			++pointIterator;                                // advance to next point
		}
	}

	{
		typedef typename InternalMeshType::CellsContainer::Iterator     CellsIterator;

		auto  cellIterator = m_InternalMesh->GetCells()->Begin();

		auto end = m_InternalMesh->GetCells()->End();

		while( cellIterator != end ){
			typename InternalMeshType::CellType * cellptr = cellIterator.Value();

			LineType * line = dynamic_cast<LineType *>( cellptr );

			long unsigned int* linePoint0 = line->PointIdsBegin();
			long unsigned int* linePoint1 = linePoint0+1;
			typename InternalMeshType::PointType Z0=m_InternalMesh->GetPoint(*linePoint0);
			typename InternalMeshType::PointType Z1=m_InternalMesh->GetPoint(*linePoint1);

			auto lambda0 = m_LambdaReg[*linePoint0][*linePoint1];
			auto lambda1 = m_LambdaReg[*linePoint1][*linePoint0];

			lambda0 = lambda0 + m_Rho*(m_XReg[*linePoint0][*linePoint1]-Z0[2]);
			lambda1 = lambda1 + m_Rho*(m_XReg[*linePoint1][*linePoint0]-Z1[2]);


			m_LambdaReg[*linePoint1][*linePoint0]=lambda1;
			m_LambdaReg[*linePoint0][*linePoint1]=lambda0;

			++cellIterator;
		}
	}

}
#endif
#if 1
template< typename TInputImage,typename TOutputMesh>
void
ttt::TissueSurfaceMeshSource< TInputImage,TOutputMesh>
::XStep()
{
#if 1
	m_XSupport.resize(m_NumberOfPoints);

	{
	 // Access points
		typedef typename InternalMeshType::PointsContainer::Iterator     PointsIterator;

		PointsIterator  pointIterator = m_InternalMesh->GetPoints()->Begin();
		auto  pointDataIterator = m_InternalMesh->GetPointData()->Begin();

		PointsIterator end = m_InternalMesh->GetPoints()->End();
		auto pointEnd = m_InternalMesh->GetPointData()->End();

		double maxZ = this->GetInput()->GetSpacing()[2]*(this->GetInput()->GetLargestPossibleRegion().GetSize()[2]-1);
		double minZ=0;
		while( pointIterator != end ){
			//u = x - Lambda

			auto Z = pointIterator.Value();
			auto lambda = m_LambdaSupport[pointIterator.Index()];

			auto x0=Z;
			auto x1=x0;

			typename InputImageType::IndexType index;
			this->GetInput()->TransformPhysicalPointToIndex(x0,index);




			if(this->GetInput()->GetLargestPossibleRegion().IsInside(index)){

				auto f0 = -std::pow(m_Interpolator->Evaluate(x0),2)  + lambda*(x0[2]-Z[2])+m_Rho/2*std::pow(x0[2]-Z[2],2);
				auto g1= -2*m_Interpolator->Evaluate(x0)*m_FirstGradientInterpolator->Evaluate(x0) + lambda + m_Rho*(x0-Z)[2] ;
				auto g2= -2*std::pow(m_FirstGradientInterpolator->Evaluate(x0),2)-2*m_Interpolator->Evaluate(x0)*m_SecondGradientInterpolator->Evaluate(x0) + m_Rho;


				double step = 1.e-2;
				bool found = false;

				while(step>1e-8 && !found){
					if(g1!=0 && g2!=0){
						x1[2]=x0[2]-step*g1/g2;
					}

					if(x1[2]<0 || x1[2]>=maxZ){
						found = false;
						step=step/2;
						x1[2]=x0[2];
					}else{
						//auto f1 = -m_Interpolator->Evaluate(x1) + lambda*(x1[2]-Z[2])+m_Rho/2*std::pow(x1[2]-Z[2],2);
						auto f1 = -std::pow(m_Interpolator->Evaluate(x1),2)  + lambda*(x1[2]-Z[2])+m_Rho/2*std::pow(x1[2]-Z[2],2);

						if(f1<f0){
							found=true;
						}else{
							step=step/2;
						}
					}
				}
			}else{
				auto x1=Z;
				//std::cout << "Problem" << std::endl;
			}
			m_XSupport[pointIterator.Index()]=x1[2];
	    	++pointIterator;                                // advance to next point
		}
	}
#endif
#if 0
	m_XSupport.resize(m_NumberOfPoints);

	SupportProximalMapping<InterpolatorType,GradientInterpolatorType,OPointType> supportProximalMapping;
	supportProximalMapping.m_Interpolator=m_Interpolator;
	supportProximalMapping.m_GradientInterpolator=m_FirstGradientInterpolator;
	supportProximalMapping.m_Rho=m_Rho;
	{
	 // Access points
		typedef typename InternalMeshType::PointsContainer::Iterator     PointsIterator;

		PointsIterator  pointIterator = m_InternalMesh->GetPoints()->Begin();
		auto  pointDataIterator = m_InternalMesh->GetPointData()->Begin();

		PointsIterator end = m_InternalMesh->GetPoints()->End();
		auto pointEnd = m_InternalMesh->GetPointData()->End();

		double maxZ = this->GetInput()->GetSpacing()[2]*(this->GetInput()->GetLargestPossibleRegion().GetSize()[2]-1);

		while( pointIterator != end ){
			//u = x - Lambda

			auto x = pointIterator.Value();
			auto lambda = m_LambdaSupport[pointIterator.Index()];

			supportProximalMapping.m_Y=lambda;
			supportProximalMapping.m_Z=x[2];
			vnl_lbfgsb minimizer(supportProximalMapping);
			minimizer.set_trace(true);
			minimizer.set_projected_gradient_tolerance(1e-16);
			vnl_vector<double> lowerbound(1);
			lowerbound[0]=0;

			vnl_vector<double> upperbound(1);
			upperbound[0]=maxZ;

			minimizer.set_lower_bound(lowerbound);
			minimizer.set_upper_bound(upperbound);

			vnl_vector<long>  boundSelector(1);
			boundSelector[0]=2;
			minimizer.set_bound_selection(boundSelector);

			vnl_vector<double> init(1);
			init[0]=x[2];

			auto z = minimizer.minimize(init);

			m_XSupport[pointIterator.Index()]=init[0];
	    	++pointIterator;                                // advance to next point
		}
	}
#endif
	{
		typedef typename InternalMeshType::CellsContainer::Iterator     CellsIterator;

		auto  cellIterator = m_InternalMesh->GetCells()->Begin();

		auto end = m_InternalMesh->GetCells()->End();

		while( cellIterator != end ){
			typename InternalMeshType::CellType * cellptr = cellIterator.Value();

			LineType * line = dynamic_cast<LineType *>( cellptr );

			long unsigned int* linePoint0 = line->PointIdsBegin();
			long unsigned int* linePoint1 = linePoint0+1;
			typename InternalMeshType::PointType Z0=m_InternalMesh->GetPoint(*linePoint0);
			typename InternalMeshType::PointType Z1=m_InternalMesh->GetPoint(*linePoint1);

			auto lambda0 = m_LambdaReg[*linePoint0][*linePoint1];
			auto lambda1 = m_LambdaReg[*linePoint1][*linePoint0];

			//auto z0 = u0 + m_Lambda*(u1-u0)/2;
			//auto z1 = u1 + m_Lambda*(u0-u1)/2;
			//auto z0 =(u0* m_Lambda + u1*m_Lambda + u0)/(1+2*m_Lambda);
			//auto z1 =(u0* m_Lambda + u1*m_Lambda + u1)/(1+2*m_Lambda);
			auto x0 = ((2*m_Rho*Z1[2]-2*lambda1+2*m_Rho*Z0[2] -2*lambda0)*m_Lambda + m_Rho*m_Rho*Z0[2]-m_Rho*lambda0)/(4*m_Lambda*m_Rho + m_Rho*m_Rho);
			auto x1 = ((2*m_Rho*Z1[2]-2*lambda1+2*m_Rho*Z0[2] -2*lambda0)*m_Lambda + m_Rho*m_Rho*Z1[2]-m_Rho*lambda1)/(4*m_Lambda*m_Rho + m_Rho*m_Rho);

			m_XReg[*linePoint0][*linePoint1]=x0;
			m_XReg[*linePoint1][*linePoint0]=x1;
			++cellIterator;
		}
	}

}
#endif
/** Generate the data */
template< typename TInputImage,typename TOutputMesh>
double ttt::TissueSurfaceMeshSource< TInputImage, TOutputMesh>
::Eval(const typename InternalMeshType::Pointer & mesh) {

	double support =0;

	double lsupport=0;
	double alsupport=0;
	{
		// Access points
		typedef typename InternalMeshType::PointsContainer::Iterator PointsIterator;

		PointsIterator pointIterator = mesh->GetPoints()->Begin();
		auto pointDataIterator = mesh->GetPointData()->Begin();

		PointsIterator end = mesh->GetPoints()->End();
		auto pointEnd = mesh->GetPointData()->End();



		while (pointIterator != end) {
			auto Z = pointIterator.Value();
			auto X = Z;
			X[2]=m_XSupport[pointIterator.Index()];
			support-=std::pow(m_Interpolator->Evaluate(X),2);

			lsupport += m_LambdaSupport[pointIterator.Index()]*(m_XSupport[pointIterator.Index()]-Z[2]);
			alsupport +=m_Rho*std::pow(m_XSupport[pointIterator.Index()]-Z[2],2);

			++pointIterator;                                // advance to next point
		}


	}

	double reg=0;
	{
		typedef typename InternalMeshType::CellsContainer::Iterator CellsIterator;

		auto cellIterator = mesh->GetCells()->Begin();

		auto end = mesh->GetCells()->End();

		while (cellIterator != end) {
			typename InternalMeshType::CellType * cellptr = cellIterator.Value();

			LineType * line = dynamic_cast<LineType *>(cellptr);

			long unsigned int* linePoint0 = line->PointIdsBegin();
			long unsigned int* linePoint1 = linePoint0 + 1;
			typename InternalMeshType::PointType point0 = mesh->GetPoint(*linePoint0);
			typename InternalMeshType::PointType point1 = mesh->GetPoint(*linePoint1);

			lsupport += m_LambdaSupport[*linePoint0]*(m_XReg[*linePoint0][*linePoint1]-point0[2]);
			lsupport += m_LambdaSupport[*linePoint1]*(m_XReg[*linePoint1][*linePoint0]-point1[2]);

			alsupport +=m_Rho*std::pow(m_XReg[*linePoint0][*linePoint1]-point0[2],2);
			alsupport +=m_Rho*std::pow(m_XReg[*linePoint1][*linePoint0]-point1[2],2);

			point0[2]=m_XReg[*linePoint0][*linePoint1];
			point1[2]=m_XReg[*linePoint1][*linePoint0];




			reg+= m_Lambda*std::pow(point0[2] -point1[2],2);

			++cellIterator;
		}
	}



	double funObj = support + reg + support + alsupport ;
	std::cout << "Support: " << support << "\tReg: " << reg <<"\tFunObj: " << funObj << std::endl;
#if 0
	double funObj=0;
	double support=0;
	{
		// Access points
		typedef typename InternalMeshType::PointsContainer::Iterator PointsIterator;

		PointsIterator pointIterator = m_InternalMesh->GetPoints()->Begin();
		auto pointDataIterator = m_InternalMesh->GetPointData()->Begin();

		PointsIterator end = m_InternalMesh->GetPoints()->End();
		auto pointEnd = m_InternalMesh->GetPointData()->End();

		double maxZ = this->GetInput()->GetSpacing()[2] * (this->GetInput()->GetLargestPossibleRegion().GetSize()[2] - 1);

		while (pointIterator != end) {
			auto x = pointIterator.Value();
			x[2]=m_ZSupport[pointIterator.Index()];
			support=support-m_Interpolator->Evaluate(x);
			++pointIterator;                                // advance to next point
		}
	}

	double reg=0;
	{
		typedef typename InternalMeshType::CellsContainer::Iterator CellsIterator;

		auto cellIterator = m_InternalMesh->GetCells()->Begin();

		auto end = m_InternalMesh->GetCells()->End();

		while (cellIterator != end) {
			typename InternalMeshType::CellType * cellptr = cellIterator.Value();

			LineType * line = dynamic_cast<LineType *>(cellptr);

			long unsigned int* linePoint0 = line->PointIdsBegin();
			long unsigned int* linePoint1 = linePoint0 + 1;
			typename InternalMeshType::PointType point0 = m_InternalMesh->GetPoint(*linePoint0);

			point0[2]=m_ZReg[*linePoint0][*linePoint1];


			typename InternalMeshType::PointType point1 = m_InternalMesh->GetPoint(*linePoint1);
			point1[2]=m_ZReg[*linePoint1][*linePoint0];
			reg=reg+ m_Lambda*(point0 -point1).GetSquaredNorm();

			++cellIterator;
		}
	}
	double lagrangeTermSupport=0;
	{
			// Access points
			typedef typename InternalMeshType::PointsContainer::Iterator PointsIterator;

			PointsIterator pointIterator = m_InternalMesh->GetPoints()->Begin();
			auto pointDataIterator = m_InternalMesh->GetPointData()->Begin();

			PointsIterator end = m_InternalMesh->GetPoints()->End();
			auto pointEnd = m_InternalMesh->GetPointData()->End();

			while (pointIterator != end) {

				auto x = pointIterator.Value();
				lagrangeTermSupport = lagrangeTermSupport+m_LambdaSupport[pointIterator.Index()]*(m_ZSupport[pointIterator.Index()]- x[2]);
				++pointIterator;                                // advance to next point

			}
		}


#endif
}
/** Generate the data */
template< typename TInputImage,typename TOutputMesh>
void
ttt::TissueSurfaceMeshSource< TInputImage, TOutputMesh>
::GenerateData()
{
	m_CurrentIteration=0;
	typename TInputImage::ConstPointer input = this->GetInput();
	m_Interpolator->SetInputImage(this->GetInput());

	m_FirstGradientFilter->SetInput(this->GetInput());
	m_SecondGradientFilter->SetInput(this->GetInput());

	m_FirstGradientFilter->Update();
	m_SecondGradientFilter->Update();
	m_FirstGradientInterpolator->SetInputImage(m_FirstGradientFilter->GetOutput());
	m_SecondGradientInterpolator->SetInputImage(m_SecondGradientFilter->GetOutput());


	typedef itk::ImageFileWriter<TInputImage> FileWriterType;
	typename FileWriterType::Pointer firstWriter = FileWriterType::New();
	typename FileWriterType::Pointer secondWriter = FileWriterType::New();

	firstWriter->SetFileName("firstGradient.mha");
	secondWriter->SetFileName("secondGradient.mha");
	firstWriter->SetInput(m_FirstGradientFilter->GetOutput());
	secondWriter->SetInput(m_SecondGradientFilter->GetOutput());
	firstWriter->Update();
	secondWriter->Update();
	this->InitInternalMesh();
#if 1
	m_XSupport.resize(m_NumberOfPoints);
	m_LambdaSupport.resize(m_NumberOfPoints);

	for(int i=0;i<m_NumberOfPoints;i++){
		m_XSupport[i]=0;
		m_LambdaSupport[i]=0;
	}
#endif
#if 1
	//std::for_each(m_LambdaSupport.begin(),m_LambdaSupport.end(),[](OPointType & p){p.Fill(0);});
	//std::for_each(m_ZSupport.begin(),m_ZSupport.end(),[](OPointType & p){p.Fill(0);});

	while(!this->StoppingCriteria()){
		this->XStep();
		this->ZStep();
		this->LambdaStep();
		this->Eval(this->m_InternalMesh);
		this->m_CurrentIteration++;

	}
#endif
#if 0
	this->Eval(this->m_InternalMesh);
	std::cout <<  this->m_CurrentIteration << "\t" << m_ObjectiveFunction << std::endl;

	while(!this->StoppingCriteria()){
		std::map<unsigned long,float> firstGradient;
		std::map<unsigned long,float> secondGradient;


		{
		 // Access points
			typedef typename InternalMeshType::PointsContainer::Iterator     PointsIterator;

			PointsIterator  pointIterator = m_InternalMesh->GetPoints()->Begin();
			auto  pointDataIterator = m_InternalMesh->GetPointData()->Begin();

			PointsIterator end = m_InternalMesh->GetPoints()->End();
			auto pointEnd = m_InternalMesh->GetPointData()->End();

			while( pointIterator != end ){

				auto x= pointIterator.Value();
				firstGradient[pointIterator.Index()]= -m_FirstGradientInterpolator->Evaluate(x) ;
				secondGradient[pointIterator.Index()]= -m_SecondGradientInterpolator->Evaluate(x) ;
		    	++pointIterator; // advance to next point
			}
		}

		{
			typedef typename InternalMeshType::CellsContainer::Iterator     CellsIterator;

			auto  cellIterator = m_InternalMesh->GetCells()->Begin();

			auto end = m_InternalMesh->GetCells()->End();

			while( cellIterator != end ){
				typename InternalMeshType::CellType * cellptr = cellIterator.Value();

				LineType * line = dynamic_cast<LineType *>( cellptr );

				long unsigned int* linePoint0 = line->PointIdsBegin();
				long unsigned int* linePoint1 = linePoint0+1;
				typename InternalMeshType::PointType point0=m_InternalMesh->GetPoint(*linePoint0);
				typename InternalMeshType::PointType point1=m_InternalMesh->GetPoint(*linePoint1);

				firstGradient[*linePoint0]-=2*m_Lambda*(point1-point0)[2];
				secondGradient[*linePoint0]-=2*m_Lambda;

				firstGradient[*linePoint1]-=2*m_Lambda*(point0-point1)[2];
				secondGradient[*linePoint1]-=2*m_Lambda;

				++cellIterator;
			}
		}

		{
			// Access points
			typedef typename InternalMeshType::PointsContainer::Iterator     PointsIterator;

			PointsIterator  pointIterator = m_InternalMesh->GetPoints()->Begin();
			auto  pointDataIterator = m_InternalMesh->GetPointData()->Begin();

			PointsIterator end = m_InternalMesh->GetPoints()->End();
			auto pointEnd = m_InternalMesh->GetPointData()->End();

			double maxZ = this->GetInput()->GetSpacing()[2]*(this->GetInput()->GetLargestPossibleRegion().GetSize()[2]-1);
			double minZ=0;

			while( pointIterator != end ){

				auto & x= pointIterator.Value();
				double f1=firstGradient[pointIterator.Index()];
				double f2=secondGradient[pointIterator.Index()];
				double variation =f1/f2;
				//double scaledVariation=this->GetInput()->GetSpacing()[2]*variation;
				double scaledVariation=variation;
				//std::cout << variation << " " << scaledVariation<<  std::endl;

				if(f2!=0){
					auto x1=x;
					x1[2]=x[2]- scaledVariation;
					if(x1[2]>maxZ){
						x1[2]=x[2];
					}
					if(x1[2]<minZ){
						x1[2]=x[2];
					}
					x[2]=x1[2];
				}
				//std::cout << x << std::endl;

				++pointIterator; // advance to next point
			}

		}
		this->m_CurrentIteration++;

		this->Eval(this->m_InternalMesh);
	}
#endif
	this->UpdateOutputMesh();

}



/** Generate the data */
template< typename TInputImage,typename TOutputMesh>
void
ttt::TissueSurfaceMeshSource< TInputImage, TOutputMesh>
::UpdateOutputMesh(){
	{
	 // Access points
		typedef typename InternalMeshType::PointsContainer::Iterator     PointsIterator;

		PointsIterator  pointIterator = m_InternalMesh->GetPoints()->Begin();

		PointsIterator end = m_InternalMesh->GetPoints()->End();

		this->GetOutput()->GetPoints()->Reserve(this->GetInput()->GetLargestPossibleRegion().GetSize(0)*this->GetInput()->GetLargestPossibleRegion().GetSize(1));
		int currentPoint=0;
		while( pointIterator != end ){
			typename InternalMeshType::PointType p = pointIterator.Value();  // access the point


			this->GetOutput()->SetPoint(currentPoint,p);

			typename InputImageType::IndexType index;
			this->GetInput()->TransformPhysicalPointToIndex(p,index);
			this->GetOutput()->SetPointData(currentPoint,this->GetInput()->GetPixel(index));
			currentPoint++;
	    	++pointIterator;                                // advance to next point
		}

	}
}
template< typename TInputImage,typename TOutputMesh>
void
ttt::TissueSurfaceMeshSource< TInputImage, TOutputMesh>
::PrintSelf(std::ostream & os, itk::Indent indent) const{


}

/** Generate the data */
template< typename TInputImage,typename TOutputMesh>
typename ttt::TissueSurfaceMeshSource< TInputImage,TOutputMesh>::InternalMeshType::Pointer
ttt::TissueSurfaceMeshSource< TInputImage,TOutputMesh>
::GenerateNewMesh(typename ttt::TissueSurfaceMeshSource< TInputImage,TOutputMesh>::InternalMeshType::Pointer & mesh
		,const std::map<unsigned long,float> gradients,float stepSize){

	typename TissueSurfaceMeshSource< TInputImage,TOutputMesh>::InternalMeshType::Pointer newMesh = InternalMeshType::New();

	newMesh->GetPoints()->Reserve(this->GetInput()->GetLargestPossibleRegion().GetSize(0)*this->GetInput()->GetLargestPossibleRegion().GetSize(1));
	newMesh->GetPointData()->Reserve(this->GetInput()->GetLargestPossibleRegion().GetSize(0)*this->GetInput()->GetLargestPossibleRegion().GetSize(1));

	// Access points
	typedef typename InternalMeshType::PointsContainer::Iterator     PointsIterator;

	PointsIterator  pointIterator = mesh->GetPoints()->Begin();
	auto  pointDataIterator = mesh->GetPointData()->Begin();

	PointsIterator end = mesh->GetPoints()->End();
	auto pointEnd = mesh->GetPointData()->End();

	int i=0;
	while( pointIterator != end ){
		typename InternalMeshType::PointType &  p = pointIterator.Value();  // access the point

		typename InternalMeshType::PointType p1 = p -stepSize*gradients[pointIterator.Index()];


		typename InputImageType::IndexType index;

		this->GetInput()->TransformPhysicalPointToIndex(p1,index);

		for(unsigned d =0;d<3;d++){
			if(index[d]<0) index[d]=0;
			if(index[d]>this->GetInput()->GetLargestPossibleRegion().GetSize(d)-1) index[d]=this->GetInput()->GetLargestPossibleRegion().GetSize(d)-1;
		}
		this->GetInput()->TransformIndexToPhysicalPoint(index,p1);
		p1[0]=p[0];
		p1[1]=p[1];
		p1[2]=p1[2];

		if(p1[0]<0 || p1[1] <0 || p1[2] < 0){
			std::cout << "Fuck!" << std::endl;
		}
		newMesh->SetPoint(i,p1);
		//newMesh->SetPointData(i,pointDataIterator.Value());
		i++;
		//p=p1;

		++pointDataIterator;
		++pointIterator;                                // advance to next point
	}

	i = 0;
	typename InternalMeshType::CellsContainerPointer cells = mesh->GetCells( );
	typename InternalMeshType::CellsContainerConstIterator itCells = cells->Begin();
	typename InternalMeshType::CellsContainerConstIterator itCellsEnd = cells->End();
	while ( itCells != itCellsEnd )
	    {
		typename InternalMeshType::CellType::CellAutoPointer cellCopy;
		itCells.Value()->MakeCopy( cellCopy );
		newMesh->SetCell( i++, cellCopy );
		++itCells;
	    }
	newMesh->DisconnectPipeline();
	return newMesh;
}
#endif /* TISSUESURFACEMESHSOURCE_HXX_ */
