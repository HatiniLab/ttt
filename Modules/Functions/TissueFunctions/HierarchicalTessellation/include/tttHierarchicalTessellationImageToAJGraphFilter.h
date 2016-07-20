/*
 * tttHierarchicalTesselationImageToAJGraphFilter.h
 *
 *  Created on: Jul 11, 2016
 *      Author: morgan
 */

#ifndef MODULES_FUNCTIONS_TISSUEFUNCTIONS_HIERARCHICALTESSELLATION_INCLUDE_TTTHIERARCHICALTESSELLATIONIMAGETOAJGRAPHFILTER_H_
#define MODULES_FUNCTIONS_TISSUEFUNCTIONS_HIERARCHICALTESSELLATION_INCLUDE_TTTHIERARCHICALTESSELLATIONIMAGETOAJGRAPHFILTER_H_

#include "tttImageToAJGraphFilter.h"
#include <vector>
#include <queue>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/algorithms/covered_by.hpp>


#include <itkLinearInterpolateImageFunction.h>
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkLineConstIterator.h>
#include <itkGradientImageFilter.h>
#include "itkGradientVectorFlowImageFilter.h"
#include "VNLIterativeSparseSolverTraits.h"

namespace bg = boost::geometry;

namespace ttt{

template<class TInputImage,class TAJGraph> class HierarchicalTessellationImageToAJGraphFilter : public ttt::ImageToAJGraphFilter<TInputImage,TAJGraph>{

public:
	typedef HierarchicalTessellationImageToAJGraphFilter<TInputImage,TAJGraph> Self;
	typedef ttt::ImageToAJGraphFilter<TInputImage,TAJGraph> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;


	typedef  TInputImage InputImageType;
	typedef typename TInputImage::IndexType IndexType;
	typedef typename TInputImage::OffsetType OffsetType;
	typedef typename TInputImage::RegionType RegionType;
	typedef typename TInputImage::SizeType SizeType;
	typedef typename TInputImage::PointType PointType;


	typedef itk::Image<float,2> WeightImageType;
	typedef itk::Image<IndexType,2> OriginsImageType;

	 typedef bg::model::point<double, 2, bg::cs::cartesian> ContourPointType;
	 typedef bg::model::polygon<ContourPointType> ContourType;

	 typedef bg::model::linestring<ContourPointType> PathType;

	 typedef itk::LinearInterpolateImageFunction<InputImageType> InputInterpolatorType;

	 typedef itk::GradientImageFilter<InputImageType> GradientFilterType;
	 typedef typename GradientFilterType::OutputImageType GradientImageType;
	 typedef itk::LinearInterpolateImageFunction<GradientImageType> GradientInterpolatorType;

#if 0
	 typedef struct PathContainer{
		 PathType path;
		 double cost;
		 double length;
	 }PathContainerType;

	 bool dominates(const PathContainer &a, const PathContainer &b) const{
		 return a.cost < b.cost && a.length>b.length;
	 }


	 template<class TIterator> bool isDominated(const PathContainer &a, const TIterator & begin, const TIterator & end ){

		 return std::accumulate(begin,end,false,[&](bool result,PathContainer & b){
			return result || dominates(b,a);
		 });
	 }
	 template<class TIterator,class TInsertionIterator> bool nonDominated( TIterator & begin,  TIterator & end,  TInsertionIterator & result){
		 return std::copy_if(begin,end,result,[&begin,&end](PathContainer & a){
			return !isDominated(a,begin,end);
		 });
	 }
#endif
	itkNewMacro(Self);
	itkTypeMacro(HierarchicalTessellationImageToAJGraphFilter,HierarchicalTessellationImageToAJGraphFilter);
protected:

	HierarchicalTessellationImageToAJGraphFilter(){
		m_MinimumStrength=0;
		m_Alpha=0.01;
		//m_Alpha=0.1;
		m_Beta=0.4;
		//m_Beta=1;
		m_Gamma=0.05;

	}
	~HierarchicalTessellationImageToAJGraphFilter(){

	}
	virtual void SubdivideContour(const ContourType & contour){

	}
#if 0
	virtual void ResetImages(){

		IndexType index;
		index.Fill(-1.0);
		this->m_Origins->FillBuffer(index);

		this->m_Weights->FillBuffer(std::numeric_limits<float>::max());
	}
	virtual void AllocateImages(){
		this->m_Weights=WeightImageType::New();

		this->m_Weights->SetRegions(this->GetInput()->GetLargestPossibleRegion());
		this->m_Weights->CopyInformation(this->GetInput());
		this->m_Weights->Allocate();

		this->m_Origins=OriginsImageType::New();
		this->m_Origins->SetRegions(this->GetInput()->GetLargestPossibleRegion());
		this->m_Origins->CopyInformation(this->GetInput());
		this->m_Origins->Allocate();


	}
#endif
	virtual ContourType GetInitialContour(){

		ContourType contour;
#if 1
		RegionType region=this->GetInput()->GetLargestPossibleRegion();


		for(int c=0;c<region.GetSize(1);c++){
			bg::append(contour.outer(), ContourPointType(0,c));
		}

		for(int r=1;r<region.GetSize(0);r++){
			bg::append(contour.outer(), ContourPointType(r, region.GetSize(1)-1));
		}

		for(int c =region.GetSize(1)-2;c>=0;c--){
			bg::append(contour.outer(),ContourPointType(region.GetSize(0)-1,c));
		}
		for(int r=region.GetSize(0)-2;r>=1;r--){
			bg::append(contour.outer(),ContourPointType(r,0));
		}
#endif

#if 0
		int n=50;

		double cx=268;
		double cy=279;
		double rx=60;
		double ry=60;

		for (int i=0; i<n; i++)	{
			bg::append(contour.outer(),ContourPointType(cx + rx*cos(2*vnl_math::pi*i/n), cy + ry*sin(2*vnl_math::pi*i/n)));
		}
#endif
		return contour;
	}
	typedef struct ContourTreeNode {
		ContourType contour;
		struct ContourTreeNode * left;
		ContourTreeNode * right;
		ContourTreeNode * parent;
	} ContourTreeNodeType;


	typedef struct DijkstraNode{
		IndexType index;
		float value;
		bool operator>(const DijkstraNode & node) const
		  { return value > node.value; }
	}DijkstraNodeType;

#if 0
	virtual std::vector<PathContainerType> GetAllNonDominatedShortestPaths(const ContourPointType & origin, const ContourType & contour){

		this->ResetImages();

		std::list<IndexType>  contourIndices(contour.outer().size());
		std::transform(contour.outer().begin(),contour.outer().end(),std::back_inserter(contourIndices),[](const ContourPointType & point){
			IndexType index;
			index[0]=point.get<0>();
			index[1]=point.get<1>();
			return index;

		});
		auto unreachedIndices=contourIndices;
		IndexType originIndex;
		originIndex[0]=origin.get<0>();
		originIndex[1]=origin.get<1>();
		std::cout << "Backtracking from " << originIndex << std::endl;

		unreachedIndices.remove(originIndex);


		typedef std::vector<DijkstraNodeType> HeapContainer;
		typedef std::greater<DijkstraNodeType> NodeComparer;

		typedef std::priority_queue<DijkstraNodeType, HeapContainer, NodeComparer> HeapType;

		DijkstraNodeType initialNode;
		initialNode.value=0;
		initialNode.index=originIndex;
		this->m_Weights->SetPixel(initialNode.index,initialNode.value);

		HeapType trialHeap;
		trialHeap.push(initialNode);

		while(!trialHeap.empty() && unreachedIndices.size()>0){

			DijkstraNodeType currentNode = trialHeap.top();
			trialHeap.pop();

			unreachedIndices.remove(currentNode.index);

			auto currentValue = currentNode.value;
			if(currentValue==this->m_Weights->GetPixel(currentNode.index)){
				std::cout << currentNode.index << "\t" << currentNode.value << "\t" <<unreachedIndices.size()<< std::endl;
				// update its neighbors
				OffsetType offset;
				auto centerValue = this->GetInput()->GetPixel(currentNode.index);
				PointType centerImagePoint;
				this->GetInput()->TransformIndexToPhysicalPoint(currentNode.index,centerImagePoint);

				for(int r=-1;r<=1;r++){
					offset[0]=r;
					for(int c =-1;c<=1;c++){
						if(r==0 && c==0) continue;
						offset[1]=c;

						IndexType neighbor=currentNode.index + offset;

						ContourPointType neighPoint(neighbor[0],neighbor[1]);
						//if(std::find(contourIndices.begin(),contourIndices.end(),neighbor)!=contourIndices.end()) continue;
						if(bg::covered_by(neighPoint,contour) ){
							auto neighValue = this->GetInput()->GetPixel(currentNode.index);


								PointType neighImagePoint;
								this->GetInput()->TransformIndexToPhysicalPoint(neighbor,neighImagePoint);

								auto increment = 1.0/(1+.5*(centerValue+neighValue))*(neighImagePoint-centerImagePoint).GetNorm();

								double cost = currentValue + increment;
								double currentCost = this->m_Weights->GetPixel(neighbor);
								//std::cout << "\tExploring " << neighbor << " with cost " << cost << "\t" << currentCost << std::endl;

								if(cost < currentCost){
									DijkstraNodeType newNode;
									newNode.index=neighbor;
									newNode.value=cost;
									this->m_Weights->SetPixel(newNode.index,newNode.value);
									this->m_Origins->SetPixel(newNode.index,currentNode.index);
									trialHeap.push(newNode);
								}
						}
					}
				}
			}
		}

		std::vector<PathContainerType> paths;

		for(auto it=contour.outer().begin();it!=contour.outer().end();++it){


			IndexType current;
			current[0]=it->get<0>();
			current[1]=it->get<1>();
			if(this->m_Weights->GetPixel(current)<std::numeric_limits<double>::max()){

				PathContainerType pathContainer;
				bg::append(pathContainer.path,*it);
				pathContainer.cost=this->m_Weights->GetPixel(current);
				double length=0;
				while(current!=originIndex){
					auto next=this->m_Origins->GetPixel(current);

					PointType currentPoint, nextPoint;
					this->GetInput()->TransformIndexToPhysicalPoint(current,currentPoint);
					this->GetInput()->TransformIndexToPhysicalPoint(next,nextPoint);

					length=length+(currentPoint-nextPoint).GetNorm();

					bg::append(pathContainer.path,ContourPointType(next[0],next[1]));
					current=next;
				}
				pathContainer.length=length;
				paths.push_back(pathContainer);
			}
		}
		//TODO EXTRACT NON DOMINATED SUBSET
		std::vector<PathContainerType> nonDominatedPaths;

		//nonDominated(paths.begin(),paths.end(),std::back_inserter(nonDominatedPaths));
		std::copy_if(paths.begin(),paths.end(),std::back_inserter(nonDominatedPaths),[&](const PathContainer & a){
					return !isDominated(a,paths.begin(),paths.end());
		});

		for(auto it = nonDominatedPaths.begin();it!=nonDominatedPaths.end();++it){
			std::cout << it->length << "\t" << it->cost <<std::endl;
		}
#if 0
		std::sort(paths.begin(),paths.end(),[](const PathContainerType & a,const PathContainerType & b){
			return a.cost < b.cost && a.length > b.length;
		});
#endif




		typedef itk::ImageFileWriter<WeightImageType> FileWriterType;
		FileWriterType::Pointer fileWriter = FileWriterType::New();
		fileWriter->SetInput(this->m_Weights);
		fileWriter->SetFileName("./weights.mha");
		fileWriter->Update();

		return nonDominatedPaths;


	}
#endif

	virtual ContourType resampleContour(const ContourType & contour){

		ContourType splited,merged;


		auto currentIt = contour.outer().begin();
		auto nextIt = contour.outer().begin()+1;

		double splitThreshold=std::sqrt(2);
		double mergeThreshold=0.1;

		while(nextIt!=contour.outer().end()){
			auto point = *currentIt;
			auto nextPoint= *nextIt;
			double d = bg::distance(point,nextPoint);
			if(d>splitThreshold){
				auto dx = nextPoint.get<0>() - point.get<0>();
				auto dy = nextPoint.get<1>() - point.get<1>();

				auto m = point;
				m.set<0>(point.get<0>()+0.5*dx);
				m.set<1>(point.get<1>()+0.5*dy);

				bg::append(splited.outer(),point);
				bg::append(splited.outer(),m);
			}else{
				bg::append(splited.outer(),point);
			}
			currentIt++;
			nextIt++;
		}
		bg::append(splited.outer(),*currentIt);
#if 0
		else if(d<mergeThreshold){

						auto dx = nextPoint.get<0>() - point.get<0>();
						auto dy = nextPoint.get<1>() - point.get<1>();

						auto m = point;
						m.set<0>(point.get<0>()+dx);
						m.set<1>(point.get<1>()+dy);

						bg::append(resample.outer(),point);
						bg::append(resample.outer(),m);

					}
#endif

		return splited;


	}
	virtual void subdivideContour(const ContourType & contour, const PathType & path, ContourType & left, ContourType & right){

		ContourPointType a,b;

		a = path[0];
		b = path[path.size()-1];
		std::cout << "A\t" << a.get<0>() << ","<<  a.get<1>() << std::endl;
		std::cout << "B\t" << b.get<0>() << ","<<  b.get<1>() << std::endl;

		bool aBeforeB;


		auto aLocation = contour.outer().begin();
		auto bLocation = contour.outer().begin();

		bool aFound=false;

		do{
			ContourPointType point = *aLocation;
			if( (point.get<0>()) == a.get<0>() && (point.get<1>()) == a.get<1>() ){
				aFound=true;
			}else{
				aLocation++;
			}
		}while(!aFound);

		bool bFound=false;

		do{
			ContourPointType point = *bLocation;
			if( (point.get<0>()) == b.get<0>() && (point.get<1>()) == b.get<1>() ){
				bFound=true;
			}else{
				bLocation++;
			}
		}while(!bFound);

 		if(aLocation<bLocation){
			//LEFT
 			auto polygoniterator = contour.outer().begin();

			while(polygoniterator!=aLocation){
				bg::append(left.outer(),*polygoniterator);
				polygoniterator++;
			}

			for(auto lineit=path.begin();lineit!=path.end();++lineit){
				bg::append(left.outer(),*lineit);
			}
			polygoniterator=bLocation+1;
			while(polygoniterator!=contour.outer().end()){
				bg::append(left.outer(),*polygoniterator);
				polygoniterator++;
			}
			//RIGHT
			polygoniterator= aLocation;
			while(polygoniterator!=bLocation){
				bg::append(right.outer(),*polygoniterator);
				polygoniterator++;
			}
			for(auto lineit=path.rbegin();lineit!=path.rend()-1;++lineit){
				bg::append(right.outer(),*lineit);
			}
		}else{
			//LEFT
			auto polygoniterator = contour.outer().begin();

			while(polygoniterator!=bLocation){
				bg::append(left.outer(),*polygoniterator);
				polygoniterator++;
			}

			for(auto lineit=path.rbegin()+1;lineit!=path.rend();++lineit){
				bg::append(left.outer(),*lineit);
			}
			polygoniterator=aLocation+1;
			while(polygoniterator!=contour.outer().end()){
				bg::append(left.outer(),*polygoniterator);
				polygoniterator++;
			}
			//B
			polygoniterator= bLocation;
			while(polygoniterator!=aLocation){
				bg::append(right.outer(),*polygoniterator);
				polygoniterator++;
			}
			for(auto lineit=path.begin()+1;lineit!=path.end()-1;++lineit){
				bg::append(right.outer(),*lineit);
			}
		}

		std::cout << "LEFT" << std::endl;
		for(auto it =left.outer().begin();it!=left.outer().end();++it){
			ContourPointType point = *it;

			std::cout << point.get<0>() << ","<<  point.get<1>() << std::endl;
		}

		std::cout << "RIGHT" << std::endl;
		for(auto it =right.outer().begin();it!=right.outer().end();++it){
			ContourPointType point = *it;

			std::cout << point.get<0>() << ","<<  point.get<1>() << std::endl;
		}

	}



	virtual PathType initSnake(const ContourType & contour){
#if 0
		ContourPointType a = contour.outer().front();
		ContourPointType b = contour.outer()[contour.outer().size()/2 ];

		PathType initialPath;
		bg::append(initialPath,a);
		bg::append(initialPath,b);
		ContourType left, right;
		subdivideContour(contour,initialPath,left,right);

		double leftArea = bg::area(left);
		double rightArea = bg::area(right);


		typedef itk::LineConstIterator<InputImageType> IteratorType;
		IndexType aIndex;
		aIndex[0]=a.get<0>();
		aIndex[1]=a.get<1>();
		IndexType bIndex;
		bIndex[0]=b.get<0>();
		bIndex[1]=b.get<1>();
		IteratorType iterator(this->GetInput(),aIndex,bIndex);
		initialPath.clear();
		while(!iterator.IsAtEnd()){
			bg::append(initialPath,ContourPointType(iterator.GetIndex()[0],iterator.GetIndex()[1]));
			++iterator;
		}
#endif

	}
	virtual double evaluateSnake(const ContourType & contour, const PathType & initialSnake){

		 double externalEnergy=std::accumulate(initialSnake.begin(),initialSnake.end(),0.0,[&](double total,const ContourPointType & point){
			 PointType itkPoint;
			 itkPoint[0]=point.get<0>();
			 itkPoint[1]=point.get<1>();
			 if(this->m_InputInterpolator->IsInsideBuffer(itkPoint)){
				 return total+this->m_InputInterpolator->Evaluate(itkPoint);
			 }else{
				 return total;
			 }

		 });
		 return externalEnergy;

	}
	virtual void SOR(vnl_sparse_matrix<double> & A, const vnl_vector<double> & B, vnl_vector<double> & x ){
			double w =0.85;

			unsigned int rows = A.rows();
			unsigned int cols = A.cols();

			bool converged = false;

			double tol=0.01;
			unsigned int iteration=0;

			double previousResidual = std::numeric_limits<double>::max();
			while(!converged){
				for(unsigned int r=0;r<rows;r++){
					double sigma =0;
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
				//std::cout << x << std::endl;
				vnl_vector<double> result(B.size());
				A.mult(x,result);
				auto residualnorm = (B-result).magnitude();


				double improveFactor =(previousResidual-residualnorm)/residualnorm;
				if(improveFactor < tol){
					converged=true;
				}
				//std::cout << "SOR IT " << iteration << "\t" << residualnorm << "\t" << improveFactor << std::endl;
				previousResidual = residualnorm;
				iteration++;

			}
	}


	virtual vnl_sparse_matrix<double> getA(double alpha,double beta,double gamma,double N){
		vnl_sparse_matrix<double> A(N, N);

		double a = gamma * (2 * alpha + 6 * beta) + 1;
		double b = gamma * (-alpha - 4 * beta);
		double c = gamma * beta;

		for (int r = 0; r < N; r++) {
			A(r, r) = a;
		}

		for (int r = 0; r < N - 1; r++) {
			A(r + 1, r) = b;
			A(r, r + 1) = b;
		}
		A(0,N- 1) = b;
		A(N - 1, 0) = b;

		for (int r = 0; r < N - 2; r++) {
			A(r + 2, r) = c;
			A(r, r + 2) = c;
		}
		A(0,N - 2) = c;
		A(1, N - 1) = c;
		A(N - 2, 0) = c;
		A(N - 1, 1) = c;

		return A;

	}


	virtual ContourType optimizeContour(const ContourType & contour,double gamma){

		ContourType nextSnake=contour;
		typedef typename GradientImageType::PixelType VectorType;

		vnl_vector<double> fx(contour.outer().size());
		vnl_vector<double> fy(contour.outer().size());


		vnl_vector<double> x(contour.outer().size());
		vnl_vector<double> y(contour.outer().size());


		for (int i = 0; i < contour.outer().size(); i++) {
			PointType itkPoint;
			itkPoint[0]=contour.outer()[i].get<0>();
			itkPoint[1]=contour.outer()[i].get<1>();
			VectorType gradient =this->m_GradientInterpolator->Evaluate(itkPoint);
			fx[i]=gradient[0];
			fy[i]=gradient[1];
			x[i]=contour.outer()[i].get<0>();
			y[i]=contour.outer()[i].get<1>();
		}



		auto A = getA(m_Alpha,m_Beta,m_Gamma,contour.outer().size());

		auto xnext=x;
		this->SOR(A,x+gamma*fx,xnext);

		auto ynext=y;
		this->SOR(A,y+gamma*fy,ynext);

		ContourType regularizedSnake=contour;

		for(int i=0;i<regularizedSnake.outer().size();i++){
			regularizedSnake.outer()[i].set<0>(xnext[i]);
			regularizedSnake.outer()[i].set<1>(ynext[i]);
		}

		{
						WeightImageType::Pointer pathImage = WeightImageType::New();
						pathImage->SetRegions(this->GetInput()->GetLargestPossibleRegion());
						pathImage->CopyInformation(this->GetInput());
						pathImage->Allocate();
						pathImage->FillBuffer(0.0);


						std::for_each(regularizedSnake.outer().begin(),regularizedSnake.outer().end(),[&pathImage](ContourPointType & point){
							IndexType index;
							index[0]=point.get<0>();
							index[1]=point.get<1>();
							if(pathImage->GetLargestPossibleRegion().IsInside(index)){
								pathImage->SetPixel(index,1.0);
							}


						});

						typedef itk::ImageFileWriter<WeightImageType> FileWriterType;
						FileWriterType::Pointer fileWriter = FileWriterType::New();
						fileWriter->SetInput(pathImage);
						fileWriter->SetFileName("./paths.mha");
						fileWriter->Update();
					}




		return regularizedSnake;

	}
#if 0
	virtual PathType optimizeSnake(const ContourType & contour, const PathType & initialSnake,double gamma){

		PathType nextSnake=initialSnake;
		typedef typename GradientImageType::PixelType VectorType;

		std::vector<VectorType> energyGradients;

		std::transform(initialSnake.begin(),initialSnake.end(),std::back_inserter(energyGradients),[&](const ContourPointType & point){
			PointType itkPoint;
			itkPoint[0]=point.get<0>();
			itkPoint[1]=point.get<1>();
			VectorType gradient =this->m_GradientInterpolator->Evaluate(itkPoint);
			return gradient;
		});

		vnl_vector<double> x(initialSnake.size());
		vnl_vector<double> y(initialSnake.size());

		for(int i=0;i<initialSnake.size();i++){
			PointType itkPoint;
			itkPoint[0]=initialSnake[i].get<0>();
			itkPoint[1]=initialSnake[i].get<1>();

			x[i]=gamma*itkPoint[0] - energyGradients[i][0];
			y[i]=gamma*itkPoint[1] - energyGradients[i][1];
		}



		vnl_matrix<double> A(initialSnake.size(),initialSnake.size());
		A.set_identity();
		//A(0,0)=1;
		//A(1,1)=1;
		int lastColumn=initialSnake.size()-1;

		A(0,lastColumn-1)=m_Beta;
		A(0,lastColumn)=-(m_Alpha+4*m_Beta);
		A(0,0)=2*m_Alpha + 6*m_Beta;
		A(0,1)=-(m_Alpha+4*m_Beta);
		A(0,2)=beta;


		A(1,lastColumn)=m_Beta;
		A(1,0)=-(m_Alpha+4*m_Beta);
		A(1,1)=2*m_Alpha + 6*m_Beta;
		A(1,2)=-(m_Alpha+4*m_Beta);
		A(1,3)=m_Beta;

		for(int r=2;r<initialSnake.size()-3;r++){
			A(r,r-2)=beta;
			A(r,r-1)=-(alpha +4*beta);
			A(r,r)=2*alpha + 6*beta;
			A(r,r+1)=-(alpha +4*beta);
			A(r,r+2)=beta;
		}

		A(lastColumn-1,lastColumn-3)=beta;
		A(lastColumn-1,lastColumn-2)=-(alpha+4*beta);
		A(lastColumn-1,lastColumn-1)=2*alpha + 6*beta;
		A(lastColumn-1,lastColumn)=-(alpha+4*beta);
		A(lastColumn-1,0)=beta;


		A(lastColumn,lastColumn-2)=beta;
		A(lastColumn,lastColumn-1)=-(alpha+4*beta);
		A(lastColumn,lastColumn)=2*alpha + 6*beta;
		A(lastColumn,0)=-(alpha+4*beta);
		A(lastColumn,1)=beta;



		vnl_matrix<double> I(initialSnake.size(),initialSnake.size());
		I.set_identity();
		A = A + gamma*I;
		std::cout << A << std::endl;
		vnl_matrix_inverse<double> inverse(A);
		auto xnext=inverse.inverse()*x;
		auto ynext=inverse.inverse()*y;
		PathType regularizedSnake=initialSnake;

		for(int i=0;i<nextSnake.size();i++){
			regularizedSnake[i].set<0>(xnext[i]);
			regularizedSnake[i].set<1>(ynext[i]);
			std::cout << initialSnake[i].get<0>() <<"," << initialSnake[i].get<1>() << "\t" << regularizedSnake[i].get<0>() <<"," << regularizedSnake[i].get<1>() <<  std::endl;
		}

		return regularizedSnake;

	}
#endif
	virtual PathType resampleSnake(const ContourType & contour, const PathType & initialSnake){
		PathType resampledSnake=initialSnake;
		return resampledSnake;
	}
	virtual double SubdivideContour(const ContourType & contour, ContourType & left, ContourType & right){

		int maxIt =1000;
		int currentIt=0;
		auto currentContour=contour;
		auto nextContour=contour;
		while(currentIt<maxIt){
			currentContour = optimizeContour(currentContour,m_Gamma);
			auto resampledContour = resampleContour(currentContour);
			std::cout << resampledContour.outer().size() << "\t" << currentContour.outer().size() << std::endl;
			currentContour=resampledContour;
			currentIt++;
		}
		for(int i=0;i<nextContour.outer().size();i++){
			std::cout << contour.outer()[i].get<0>() <<"," << contour.outer()[i].get<1>() << "\t" << currentContour.outer()[i].get<0>() <<"," << currentContour.outer()[i].get<1>() <<  std::endl;
		}





#if 0
		auto numVertices=contour.outer().size();
		auto firstPoint = contour.outer().front();

		auto currentSnake=initSnake(contour);


		bool convergence=false;

		double currentValue =evaluateSnake(contour,currentSnake);
		std::cout << currentValue << std::endl;
		while(!convergence){
			double alpha=0.001;

			auto nextSnake=optimizeSnake(contour,currentSnake,alpha);
			//auto resampledSnake = resampleSnake(contour,nextSnake);
			double nextValue = evaluateSnake(contour,nextSnake);

			{
							WeightImageType::Pointer pathImage = WeightImageType::New();
							pathImage->SetRegions(this->GetInput()->GetLargestPossibleRegion());
							pathImage->CopyInformation(this->GetInput());
							pathImage->Allocate();
							pathImage->FillBuffer(0.0);


							std::for_each(nextSnake.begin(),nextSnake.end(),[&pathImage](ContourPointType & point){
								IndexType index;
								index[0]=point.get<0>();
								index[1]=point.get<1>();
								pathImage->SetPixel(index,1.0);

							});

							typedef itk::ImageFileWriter<WeightImageType> FileWriterType;
							FileWriterType::Pointer fileWriter = FileWriterType::New();
							fileWriter->SetInput(pathImage);
							fileWriter->SetFileName("./paths.mha");
							fileWriter->Update();
						}

			while(nextValue<currentValue){
				alpha=alpha/2;
				auto nextSnake=optimizeSnake(contour,currentSnake,alpha);
				double nextValue = evaluateSnake(contour,nextSnake);
			}
			std::cout << nextValue << std::endl;

			if(nextValue<currentValue){

			}else{
				convergence=false;
				currentValue=nextValue;
				currentSnake=nextSnake;
			}






		}
#endif
		//auto forwardPaths=GetAllNonDominatedShortestPaths(firstPoint,contour);

		//std::sort(forwardPaths.begin(),forwardPaths.end(),[](const PathContainerType & a,const PathContainerType & b){
		//	return a.cost/a.length> b.cost/b.length;
		//});
#if 0




		auto lastPoint = forwardPaths.front().path[0];

		auto backwardPaths=GetAllNonDominatedShortestPaths(lastPoint,contour);

		std::sort(backwardPaths.begin(),backwardPaths.end(),[](const PathContainerType & a,const PathContainerType & b){

			return a.cost/a.length> b.cost/b.length;
		});



		PathContainerType selectedPath=backwardPaths[0];


		return selectedPath.cost/selectedPath.length;
#endif
		return 0;
	}
	virtual void GenerateData(){

		//this->AllocateImages();
		m_InputInterpolator = InputInterpolatorType::New();
		m_InputInterpolator->SetInputImage(this->GetInput());

		this->m_GradientFilter = GradientFilterType::New();
		m_GradientFilter->SetInput(this->GetInput());
		m_GradientFilter->Update();
#if 1
		typedef itk::GradientVectorFlowImageFilter<GradientImageType,GradientImageType> GradientVectorFlowFilterType;

		typename GradientVectorFlowFilterType::Pointer gradientVectorFlow = GradientVectorFlowFilterType::New();

		gradientVectorFlow->SetInput(m_GradientFilter->GetOutput());
		gradientVectorFlow->SetIterationNum(300);
		gradientVectorFlow->SetNoiseLevel(10000000000000);

		gradientVectorFlow->Update();
		{
			typedef itk::ImageFileWriter<GradientImageType> GradientWriterType;

			typename GradientWriterType::Pointer gradientWriter = GradientWriterType::New();
			gradientWriter->SetInput(m_GradientFilter->GetOutput());
			gradientWriter->SetFileName("gradient.mha");
			gradientWriter->Update();

			typename GradientWriterType::Pointer gvfWriter = GradientWriterType::New();
			gvfWriter->SetInput(gradientVectorFlow->GetOutput());
			gvfWriter->SetFileName("gvf.mha");
			gvfWriter->Update();
		}
#endif
		m_GradientInterpolator = GradientInterpolatorType::New();
		m_GradientInterpolator->SetInputImage(gradientVectorFlow->GetOutput());


		ContourTreeNodeType * root = new ContourTreeNodeType;
		root->parent=0;
		root->right=0;
		root->left=0;

		root->contour=this->GetInitialContour();

		std::queue<ContourTreeNodeType*> pendingSubdivisions;

		pendingSubdivisions.push(root);

		while(!pendingSubdivisions.empty()){

			ContourTreeNodeType * current = pendingSubdivisions.front();
			pendingSubdivisions.pop();

			ContourType  leftContour;
			ContourType  rightContour;

			double strength=this->SubdivideContour(current->contour,leftContour,rightContour);

			if(strength > m_MinimumStrength){

				ContourTreeNodeType * leftContourTreeNode = new ContourTreeNodeType;
				leftContourTreeNode->contour=leftContour;
				leftContourTreeNode->parent=current;
				current->left=leftContourTreeNode;
				pendingSubdivisions.push(leftContourTreeNode);

				ContourTreeNodeType * rightContourTreeNode = new ContourTreeNodeType;
				rightContourTreeNode->contour=rightContour;
				rightContourTreeNode->parent=current;
				current->right=rightContourTreeNode;

				pendingSubdivisions.push(rightContourTreeNode);


			}
		}
	}
private:

	float m_MinimumStrength;
	typename GradientInterpolatorType::Pointer m_GradientInterpolator;
	typename InputInterpolatorType::Pointer m_InputInterpolator;
	typename GradientFilterType::Pointer m_GradientFilter;

	double m_Alpha;
	double m_Beta;
	double m_Gamma;
#if 0
	typename WeightImageType::Pointer m_Weights;
	typename OriginsImageType::Pointer m_Origins;
#endif
};
}



#endif /* MODULES_FUNCTIONS_TISSUEFUNCTIONS_HIERARCHICALTESSELLATION_INCLUDE_TTTHIERARCHICALTESSELLATIONIMAGETOAJGRAPHFILTER_H_ */
