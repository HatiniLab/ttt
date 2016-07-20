/*
 * tttTissueSurfaceImageFilter.hxx
 *
 *  Created on: May 25, 2016
 *      Author: morgan
 */

#ifndef MODULES_FUNCTIONS_IMAGEFUNCTIONS_SURFACEEXTRACTION_INCLUDE_TTTTISSUESURFACEIMAGEFILTER_HXX_
#define MODULES_FUNCTIONS_IMAGEFUNCTIONS_SURFACEEXTRACTION_INCLUDE_TTTTISSUESURFACEIMAGEFILTER_HXX_


#include "tttTissueSurfaceImageFilter.h"

#include <vnl/vnl_cost_function.h>
namespace ttt{

template<class TInputImage, class TOutputImage> TissueSurfaceImageFilter<TInputImage,TOutputImage>::TissueSurfaceImageFilter(){


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

	m_Lambda=16.0;//pow(2,30);//1.0;
	//m_Lambda=0;
	m_Rho=1.0;
	m_NumberOfIterations=10;
	m_NumberOfPoints=0;
	m_NumberOfCells=0;
	m_CurrentIteration=0;


}

template<class TInputImage, class TOutputImage> TissueSurfaceImageFilter<TInputImage,TOutputImage>::~TissueSurfaceImageFilter(){

}





template<class TInputImage, class TOutputImage> void TissueSurfaceImageFilter<TInputImage,TOutputImage>::GenerateInputRequestedRegion(){

	typename TInputImage::Pointer input = const_cast<TInputImage*>(this->GetInput());
	input->SetRequestedRegionToLargestPossibleRegion();


}
template< typename TInputImage,typename TOutputMesh>
void
ttt::TissueSurfaceImageFilter< TInputImage, TOutputMesh>
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

	for (int r = 0; r < this->GetInput()->GetLargestPossibleRegion().GetSize(0); r++) {
		for (int c = 0; c < this->GetInput()->GetLargestPossibleRegion().GetSize(1); c++) {

			typename IndexImageType::IndexType index;
			typename TInputImage::PointType point;

			index[0] = r;
			index[1] = c;
			double pointValue = 0;

			typename TInputImage::IndexType meshIndex;
			meshIndex[0] = r;
			meshIndex[1] = c;
			double max = 0;
			int maxLoc = 0;
			for (int h = 0; h < this->GetInput()->GetLargestPossibleRegion().GetSize(2); h++) {
				meshIndex[2] = h;
				double value = this->GetInput()->GetPixel(meshIndex);
				if (value > max) {
					max = value;
					maxLoc = h;
				}
			}
			meshIndex[2] = maxLoc;

			pointValue = 0;
			this->GetInput()->TransformIndexToPhysicalPoint(meshIndex, point);

			m_InternalMesh->SetPoint(m_NumberOfPoints, point);
			m_InternalMesh->SetPointData(m_NumberOfPoints, pointValue);

			indexImage->SetPixel(index, m_NumberOfPoints);

			m_NumberOfPoints++;

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

		}
	}
}
template< typename TInputImage,typename TOutputImage>
bool
ttt::TissueSurfaceImageFilter< TInputImage, TOutputImage>
::StoppingCriteria()
{
	return m_CurrentIteration >= m_NumberOfIterations;

}

template< typename TInputImage, typename TOutputImage>
void
ttt::TissueSurfaceImageFilter< TInputImage,TOutputImage>
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


template<class InterpolatorType, class GradientInterpolatorType,class PointType> struct SupportProximalMapping : public vnl_cost_function {

	typename InterpolatorType::Pointer m_Interpolator;
	typename GradientInterpolatorType::Pointer m_GradientInterpolator;
	double m_Y;
	PointType m_Z;
	double m_Rho;

	SupportProximalMapping():  vnl_cost_function(1) {
		m_Rho=1.0;
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


template<typename TInputImage,class TOutputMesh>
void ttt::TissueSurfaceImageFilter<TInputImage,TOutputMesh>::LambdaStep() {
	{
		// Access points
		typedef typename InternalMeshType::PointsContainer::Iterator PointsIterator;

		PointsIterator pointIterator = m_InternalMesh->GetPoints()->Begin();
		PointsIterator end = m_InternalMesh->GetPoints()->End();

		while (pointIterator != end) {
			//u = x - Lambda

			auto x = pointIterator.Value();
			auto lambda = m_LambdaSupport[pointIterator.Index()];

			auto xsupport = m_XSupport[pointIterator.Index()];
			lambda = lambda + m_Rho*(xsupport-x[2]);

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


template< typename TInputImage,typename TOutputMesh>
void
ttt::TissueSurfaceImageFilter< TInputImage,TOutputMesh>
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

			typename TInputImage::IndexType index;
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

/** Generate the data */
template< typename TInputImage,typename TOutputMesh>
double ttt::TissueSurfaceImageFilter< TInputImage, TOutputMesh>
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



	double funObj = support + reg + lsupport + alsupport ;
	std::cout << "Support: " << support << "\tReg: " << reg << "\tLSupport" << lsupport << "\tALSupport" << alsupport <<"\tFunObj: " << funObj << std::endl;
}




template<class TInputImage, class TOutputImage> void TissueSurfaceImageFilter<TInputImage,TOutputImage>::GenerateData(){

	m_CurrentIteration=0;

	m_NumberOfPoints=0;
	m_NumberOfCells=0;

	typename TInputImage::ConstPointer input = this->GetInput();
	m_Interpolator->SetInputImage(this->GetInput());

	m_FirstGradientFilter->SetInput(this->GetInput());
	m_SecondGradientFilter->SetInput(this->GetInput());

	m_FirstGradientFilter->Update();
	m_SecondGradientFilter->Update();
	m_FirstGradientInterpolator->SetInputImage(m_FirstGradientFilter->GetOutput());
	m_SecondGradientInterpolator->SetInputImage(m_SecondGradientFilter->GetOutput());

	this->InitInternalMesh();


	m_XSupport.resize(m_NumberOfPoints);
	m_LambdaSupport.resize(m_NumberOfPoints);

	for(int i=0;i<m_NumberOfPoints;i++){
		m_XSupport[i]=0;
		m_LambdaSupport[i]=0;
	}

	while(!this->StoppingCriteria()){
		std::cout << this->m_CurrentIteration << std::endl;

		this->XStep();
		std::cout<< "X\t";
		this->Eval(this->m_InternalMesh);
		this->ZStep();
		std::cout<< "Z\t";
		this->Eval(this->m_InternalMesh);
		this->LambdaStep();
		std::cout<< "Lambda\t";
		this->Eval(this->m_InternalMesh);
		this->m_CurrentIteration++;
	}

	this->GetOutput()->SetRegions(this->GetInput()->GetLargestPossibleRegion());
	this->GetOutput()->CopyInformation(this->GetInput());
	this->GetOutput()->Allocate();
	this->GetOutput()->FillBuffer(0.0);

	 // Access points
	typedef typename InternalMeshType::PointsContainer::Iterator     PointsIterator;

	PointsIterator  pointIterator = this->m_InternalMesh->GetPoints()->Begin();

	PointsIterator end = m_InternalMesh->GetPoints()->End();


	while( pointIterator != end ){
		typename InternalMeshType::PointType p = pointIterator.Value();  // access the point

		typename TOutputImage::IndexType index;
		this->GetOutput()->TransformPhysicalPointToIndex(p,index);
			 //outputImage->SetPixel(index,reader->GetOutput()->GetPixel(index));
		this->GetOutput()->SetPixel(index,1.0);

		++pointIterator;                                // advance to next point
	}

	//TODO fill output image

}


}


#endif /* MODULES_FUNCTIONS_IMAGEFUNCTIONS_SURFACEEXTRACTION_INCLUDE_TTTTISSUESURFACEIMAGEFILTER_HXX_ */
