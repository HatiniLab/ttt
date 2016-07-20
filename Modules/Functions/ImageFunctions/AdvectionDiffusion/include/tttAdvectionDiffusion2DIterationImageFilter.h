/*
 * ttt2DAvectionDiffusionIterationImageFilter.h
 *
 *  Created on: Jun 29, 2016
 *      Author: morgan
 */

#ifndef MODULES_FUNCTIONS_IMAGEFUNCTIONS_ADVECTIONDIFFUSION_INCLUDE_TTTADVECTIONDIFFUSION2DITERATIONIMAGEFILTER_H_
#define MODULES_FUNCTIONS_IMAGEFUNCTIONS_ADVECTIONDIFFUSION_INCLUDE_TTTADVECTIONDIFFUSION2DITERATIONIMAGEFILTER_H_

#include "itkImageToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "VNLIterativeSparseSolverTraits.h"
namespace ttt{

template<class TInputImage,class TOutputImage> class AdvectiveDiffusion2DIterationImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>{

public:
	typedef AdvectiveDiffusion2DIterationImageFilter<TInputImage,TOutputImage> Self;
	typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef TInputImage InputImageType;
	typedef typename TInputImage::SpacingType SpacingType;
	typedef typename TInputImage::RegionType InputImageRegionType;

	typedef TOutputImage OutputImageType;
	typedef typename TOutputImage::RegionType OutputImageRegionType;
	typedef typename TOutputImage::IndexType IndexType;
	typedef typename TOutputImage::SizeType SizeType;

	typedef typename TInputImage::PixelType TReal;

	itkNewMacro(Self);
	itkTypeMacro(Self,Superclass);

	itkGetMacro(Delta,TReal);
	itkSetMacro(Delta,TReal);

	itkGetMacro(Mu,TReal);
	itkSetMacro(Mu,TReal);

	itkGetMacro(Tau,TReal);
	itkSetMacro(Tau,TReal);

protected:

	AdvectiveDiffusion2DIterationImageFilter(){
		m_Mu=0.00125;

		//m_Delta=std::pow(2,-2);
		//m_Delta=1.0;
		m_Delta=1.0;
		m_Tau=0.0125;
		m_PixelArea=0;
		m_NumberOfPixels=0;
		m_EpsilonAdvection=1e-6;
		m_EpsilonDiffusion=1;
	}
	~AdvectiveDiffusion2DIterationImageFilter(){

	}

	virtual void Update2D(const IndexType & index){
		IndexType north=index;
		north[0]++;

		IndexType northEast=index;
		northEast[0]++;
		northEast[1]++;

		IndexType northWest=index;
		northWest[0]++;
		northWest[1]--;

		IndexType south=index;
		south[0]--;

		IndexType southEast=index;
		southEast[0]--;
		southEast[1]++;

		IndexType southWest=index;
		southWest[0]--;
		southWest[1]--;


		IndexType east=index;
		east[1]++;


		IndexType west=index;
		west[1]--;


		TReal centerValue,northValue,southValue,westValue,eastValue;
		TReal northEastValue, northWestValue, southEastValue, southWestValue;

		InputImageRegionType inputRegion = this->GetInput()->GetLargestPossibleRegion();

		centerValue = this->GetInput()->GetPixel(index);

		if(inputRegion.IsInside(north)){
			northValue=this->GetInput()->GetPixel(north);
		}else{
			northValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(northEast)){
			northEastValue=this->GetInput()->GetPixel(northEast);
		}else{
			northEastValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(northWest)){
			northWestValue=this->GetInput()->GetPixel(northWest);
		}else{
			northWestValue=this->GetInput()->GetPixel(index);
		}


		if(inputRegion.IsInside(south)){
			southValue=this->GetInput()->GetPixel(south);
		}else{
			southValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(southEast)){
			southEastValue=this->GetInput()->GetPixel(southEast);
		}else{
			southEastValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(southWest)){
			southWestValue=this->GetInput()->GetPixel(southWest);
		}else{
			southWestValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(east)){
			eastValue=this->GetInput()->GetPixel(east);
		}else{
			eastValue=this->GetInput()->GetPixel(index);
		}


		if(inputRegion.IsInside(west)){
			westValue=this->GetInput()->GetPixel(west);
		}else{
			westValue=this->GetInput()->GetPixel(index);
		}

		TReal u11 = (centerValue + northValue + eastValue + northEastValue)/4.0;
		TReal u1_1 = (centerValue + northValue + westValue + northWestValue)/4.0;
		TReal u_11 = (centerValue +  southValue + eastValue + southEastValue)/4.0;
		TReal u_1_1 = (centerValue + southValue + westValue + southWestValue)/4.0;


		TReal du10x=(northValue-centerValue)/this->m_Spacing[0];
		TReal du10y=(u11-u1_1)/this->m_Spacing[1];

		TReal du_10x=-(southValue-centerValue)/this->m_Spacing[0];
		TReal du_10y=(u_11-u_1_1)/this->m_Spacing[1];


		TReal du01x=(u11 - u_11)/this->m_Spacing[0];
		TReal du01y=(eastValue - centerValue)/this->m_Spacing[1];


		TReal du0_1x=(u1_1 - u_1_1)/this->m_Spacing[0];
		TReal du0_1y=-(westValue - centerValue)/this->m_Spacing[1];;


		TReal Q10 = std::sqrt(std::pow(du10x,2) + std::pow(du10y,2));
		TReal Q_10 = std::sqrt(std::pow(du_10x,2) + std::pow(du_10y,2));

		TReal Q01 = std::sqrt(std::pow(du01x,2) + std::pow(du01y,2) );
		TReal Q0_1 = std::sqrt(std::pow(du0_1x,2) + std::pow(du0_1y,2) );


		TReal QEA10 = std::sqrt(std::pow(this->m_EpsilonAdvection,2) + std::pow(Q10,2));
		TReal QEA_10 = std::sqrt(std::pow(this->m_EpsilonAdvection,2) + std::pow(Q_10,2));
		TReal QEA01 = std::sqrt(std::pow(this->m_EpsilonAdvection,2) + std::pow(Q01,2));
		TReal QEA0_1 = std::sqrt(std::pow(this->m_EpsilonAdvection,2) + std::pow(Q0_1,2));

		TReal QED10 = std::sqrt(std::pow(this->m_EpsilonDiffusion,2) + std::pow(Q10,2));
		TReal QED_10 = std::sqrt(std::pow(this->m_EpsilonDiffusion,2) + std::pow(Q_10,2));
		TReal QED01 = std::sqrt(std::pow(this->m_EpsilonDiffusion,2) + std::pow(Q01,2));
		TReal QED0_1 = std::sqrt(std::pow(this->m_EpsilonDiffusion,2) + std::pow(Q0_1,2));

		if(std::abs(QEA10 - Q10)>1){
			std::cout << "MARRON" << std::endl;
		}

		if(std::abs(QEA_10 - Q_10)>1){
			std::cout << "MARRON" << std::endl;
		}
		if(std::abs(QEA01 - Q01)>1){
			std::cout << "MARRON" << std::endl;
		}
		if(std::abs(QEA0_1 - Q0_1)>1){
			std::cout << "MARRON" << std::endl;
		}

		TReal Q = (Q10+Q_10 + Q01 + Q0_1 )/4.0;
		TReal QEA = std::sqrt(std::pow(this->m_EpsilonAdvection,2) + (std::pow(Q10,2) + std::pow(Q_10,2) + std::pow(Q01,2)+std::pow(Q0_1,2))/4);
		TReal QED = std::sqrt(std::pow(this->m_EpsilonDiffusion,2) + (std::pow(Q10,2) + std::pow(Q_10,2) + std::pow(Q01,2)+std::pow(Q0_1,2))/4);


		TReal v10,v01,v_10,v0_1;

		v10=this->m_Spacing[1]*(this->m_Delta*(northValue-centerValue)/(QEA10*this->m_Spacing[0]));
		v_10=this->m_Spacing[1]*(this->m_Delta*(southValue-centerValue)/(QEA_10*this->m_Spacing[0]));
		v01=this->m_Spacing[0]*(this->m_Delta*(eastValue-centerValue)/(QEA01*this->m_Spacing[1]));
		v0_1=this->m_Spacing[0]*(this->m_Delta*(westValue-centerValue)/(QEA0_1*this->m_Spacing[1]));

		TReal abduction=0;

		if(v10<0){
			abduction+=v10*(centerValue-northValue);
		}

		if(v_10<0){
			abduction+=v_10*(centerValue-southValue);
		}
		if(v01<0){
			abduction+=v01*(centerValue-eastValue);
		}
		if(v0_1<0){
			abduction+=v0_1*(centerValue-westValue);
		}
		abduction = abduction*this->m_Tau + this->m_PixelArea*centerValue;

		auto centerOffset = this->GetOutput()->ComputeOffset(index);
		assert(!std::isnan(centerOffset));

		m_BLock.Lock();
		m_B[centerOffset]=abduction;
		m_BLock.Unlock();


		TReal centercoefficient = this->m_PixelArea;

		centercoefficient += this->m_Tau*this->m_Mu*Q*this->m_Spacing[1]/(QED10*this->m_Spacing[0]);
		TReal northcoefficient= -this->m_Tau*this->m_Mu*Q*this->m_Spacing[1]/(QED10*this->m_Spacing[0]);

		centercoefficient += this->m_Tau*this->m_Mu*Q*this->m_Spacing[1]/(QED_10*this->m_Spacing[0]);
		TReal southcoefficient= -this->m_Tau*this->m_Mu*Q*this->m_Spacing[1]/(QED_10*this->m_Spacing[0]);

		centercoefficient += this->m_Tau*this->m_Mu*Q*this->m_Spacing[0]/(QED01*this->m_Spacing[1]);
		TReal eastcoefficient= -this->m_Tau*this->m_Mu*Q*this->m_Spacing[0]/(QED01*this->m_Spacing[1]);

		centercoefficient += this->m_Tau*this->m_Mu*Q*this->m_Spacing[0]/(QED0_1*this->m_Spacing[1]);
		TReal westcoefficient= -this->m_Tau*this->m_Mu*Q*this->m_Spacing[0]/(QED0_1*this->m_Spacing[1]);

		//std::cout << centercoefficient << "\t" << northcoefficient << "\t" << southcoefficient << "\t" << eastcoefficient << "\t" << westcoefficient << std::endl;
		auto northOffset = this->GetOutput()->ComputeOffset(north);
		auto southOffset = this->GetOutput()->ComputeOffset(south);
		auto eastOffset = this->GetOutput()->ComputeOffset(east);
		auto westOffset = this->GetOutput()->ComputeOffset(west);

		m_ALock.Lock();

		assert(!std::isnan(centercoefficient));

		VNLIterativeSparseSolverTraits<TReal>::AddToMatrix(m_A,centerOffset,centerOffset,centercoefficient);

		assert(!std::isnan(northcoefficient));
		assert(!std::isnan(southcoefficient));
		assert(!std::isnan(eastcoefficient));
		assert(!std::isnan(westcoefficient));


		if(northOffset>=0 && northOffset < this->m_NumberOfPixels) VNLIterativeSparseSolverTraits<TReal>::AddToMatrix(m_A,centerOffset,northOffset,northcoefficient);
		if(southOffset>=0 && southOffset < this->m_NumberOfPixels) VNLIterativeSparseSolverTraits<TReal>::AddToMatrix(m_A,centerOffset,southOffset,southcoefficient);
		if(eastOffset>=0 && eastOffset < this->m_NumberOfPixels) VNLIterativeSparseSolverTraits<TReal>::AddToMatrix(m_A,centerOffset,eastOffset,eastcoefficient);
		if(westOffset>=0 && westOffset < this->m_NumberOfPixels) VNLIterativeSparseSolverTraits<TReal>::AddToMatrix(m_A,centerOffset,westOffset,westcoefficient);
		m_ALock.Unlock();

	}


	virtual void BeforeThreadedGenerateData(){
		m_Spacing = this->GetOutput()->GetSpacing();
		m_Size = this->GetOutput()->GetLargestPossibleRegion().GetSize();

		m_PixelArea = m_Spacing[0]*m_Spacing[1];

		m_NumberOfPixels = this->GetInput()->GetLargestPossibleRegion().GetSize(0)*this->GetInput()->GetLargestPossibleRegion().GetSize(1);
		m_A = VNLIterativeSparseSolverTraits<TReal>::InitializeSparseMatrix(m_NumberOfPixels);
		m_B = VNLIterativeSparseSolverTraits<TReal>::InitializeVector(m_NumberOfPixels);
	}
	virtual void ThreadedGenerateData (const OutputImageRegionType &outputRegionForThread, itk::ThreadIdType threadId){
		typedef itk::ImageRegionIteratorWithIndex<OutputImageType> IteratorType;

		IteratorType iterator(this->GetOutput(),outputRegionForThread);
		while(!iterator.IsAtEnd()){

			IndexType index = iterator.GetIndex();

			this->Update2D(index);

			++iterator;
		}
	}


	virtual void AfterThreadedGenerateData(){

		vnl_vector<TReal> x(this->m_NumberOfPixels);

		typedef itk::ImageRegionConstIteratorWithIndex<OutputImageType> ConstIteratorType;
		ConstIteratorType inputIterator(this->GetInput(),this->GetInput()->GetLargestPossibleRegion());

		while(!inputIterator.IsAtEnd()){
			x[this->GetInput()->ComputeOffset(inputIterator.GetIndex())]=inputIterator.Get();
			++inputIterator;
		}
		m_A.reset();

		this->SOR(m_A,m_B,x);

		typedef itk::ImageRegionIteratorWithIndex<OutputImageType> IteratorType;

		IteratorType iterator(this->GetOutput(),this->GetOutput()->GetLargestPossibleRegion());

		while(!iterator.IsAtEnd()){

			iterator.Set(x[this->GetOutput()->ComputeOffset(iterator.GetIndex())]);
			++iterator;
		}
	}

	virtual void SOR(vnl_sparse_matrix<TReal> & A, const vnl_vector<TReal> & B, vnl_vector<TReal> & x ){
		TReal w =0.85;

		unsigned int rows = A.rows();
		unsigned int cols = A.cols();

		bool converged = false;

		double tol=0.01;
		unsigned int iteration=0;

		double previousResidual = std::numeric_limits<double>::max();
		while(!converged){
			for(unsigned int r=0;r<rows;r++){
				TReal sigma =0;
				auto row = A.get_row(r);

				for(int j = 0;j<row.size();j++){

					unsigned int c = row[j].first;
					if(r!=c){
						if(c>=A.cols()){
							std::cout << row[j].first << " " << row[j].second << std::endl;
						}
						assert(j<row.size());
						assert(c<A.cols());

						sigma = sigma+row[j].second*x[c];
					}
				}
				x[r]=(1-w)*x[r] + (w/A.get(r,r))*(B[r]-sigma);

			}
			vnl_vector<TReal> result(B.size());
			m_A.mult(x,result);
			auto residualnorm = (B-result).magnitude();


			double improveFactor =(previousResidual-residualnorm)/residualnorm;
			if(improveFactor < tol){
				converged=true;
			}
			std::cout << "SOR IT " << iteration << "\t" << residualnorm << "\t" << improveFactor << std::endl;
			previousResidual = residualnorm;
			iteration++;

		}


	}

private:

	SpacingType m_Spacing;
	SizeType m_Size;
	TReal m_PixelArea;

	itk::SimpleFastMutexLock m_ALock;
	vnl_sparse_matrix<TReal> m_A;
	itk::SimpleFastMutexLock m_BLock;
	vnl_vector<TReal> m_B;

	TReal m_Mu;
	TReal m_Tau;
	TReal m_Delta;
	TReal m_EpsilonAdvection;
	TReal m_EpsilonDiffusion;
	unsigned long m_NumberOfPixels;


};
}






#endif /* MODULES_FUNCTIONS_IMAGEFUNCTIONS_ADVECTIONDIFFUSION_INCLUDE_TTTADVECTIONDIFFUSION2DITERATIONIMAGEFILTER_H_ */
