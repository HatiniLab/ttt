#include "itkImageToImageFilter.h"
#include "itkLevelSet.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodIterator.h"
#include "itkConstantBoundaryCondition.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkImageFileWriter.h"
#include <queue>
namespace ttt {
template<class TInputImage, class TLabelImage> class WatershedImageFilter: public itk::ImageToImageFilter<TInputImage,
TLabelImage> {

public:
	typedef WatershedImageFilter Self;
	typedef itk::ImageToImageFilter<TInputImage, TLabelImage> Superclass;
	typedef itk::SmartPointer<WatershedImageFilter> Pointer;

	itkNewMacro(Self)
	;itkTypeMacro(Self,Superclass)

	typedef TInputImage InputImageType;
	typedef typename TInputImage::Pointer InputImagePointer;
	typedef typename TInputImage::ConstPointer InputImageConstPointer;
	typedef typename TInputImage::IndexType IndexType;
	/** Enum of Fast Marching algorithm point types. FarPoints represent far
	 * away points; TrialPoints represent points within a narrowband of the
	 * propagating front; and AlivePoints represent points which have already
	 * been processed. */


	/** LabelImage typedef support. */
	typedef TLabelImage LabelImageType;
	typedef typename LabelImageType::Pointer LabelImagePointer;


protected:

	virtual void Successors(const IndexType & y,const std::vector<IndexType> & L,std::vector<IndexType> & successors){
		successors.clear();

		typename IndexType::OffsetType offset;

		offset.Fill(0);

		for(int i=-1;i<=1;i++){
			offset[0]=i;
			for(int j=-1;j<=1;j++){
				offset[1]=j;
				if(i!=0 && j!=0){

					auto z = y + offset;

					if(this->GetInput()->GetLargestPossibleRegion().IsInside(z)){

						if(std::find(L.begin(),L.end(),z)==L.end()){

							double minAbsValue = std::abs(this->GetInput()->GetPixel(z)-this->GetInput()->GetPixel(y));
							if(this->m_F->GetPixel(y)==minAbsValue){
								successors.push_back(z);
							}
						}
					}
				}
			}
		}

	}
	virtual itk::IdentifierType Stream(const IndexType & x, std::vector<IndexType> & L){
		L.clear();

		std::deque<IndexType> Lp;

		L.push_back(x);
		Lp.push_back(x);

		while(Lp.size()>0){

			auto y = Lp.front();
			Lp.pop_front();

			bool bf = true;

			std::vector<IndexType> successors;

			Successors(y,L,successors);

			while(bf && successors.size()>0){


				auto z = successors.front();
				if(this->GetOutput()->GetPixel(z)!=static_cast<itk::IdentifierType>(-1)){
					return this->GetOutput()->GetPixel(z);
				}else if(m_F->GetPixel(z)<m_F->GetPixel(y)){
					L.push_back(z);
					Lp.clear();
					Lp.push_back(z);
					bf=false;
				}else{
					L.push_back(z);
					Lp.push_back(z);
				}
				Successors(y,L,successors);
			}
		}
		return -1;
	}


	virtual void GenerateData() {

		//1. Create F
		 m_F= InputImageType::New();
		 m_F->CopyInformation(this->GetInput());
		 m_F->SetRegions(this->GetInput()->GetLargestPossibleRegion());
		 m_F->Allocate();
		 m_F->FillBuffer(0);
		 m_NextIdentifier=0;

		 this->GetOutput()->SetRegions(this->GetInput()->GetLargestPossibleRegion());
		 this->GetOutput()->CopyInformation(this->GetInput());
		 this->GetOutput()->Allocate();
		 this->GetOutput()->FillBuffer(-1);

		typedef itk::ConstantBoundaryCondition<InputImageType>  BoundaryConditionType;
		typedef itk::ConstNeighborhoodIterator<InputImageType, BoundaryConditionType > NeighborIteratorType;
		typename InputImageType::SizeType size;
		BoundaryConditionType bc;
		bc.SetConstant(itk::NumericTraits<float>::max());
		size.Fill(1);

		NeighborIteratorType neighborhoodIterator(size,this->GetInput(),this->GetInput()->GetLargestPossibleRegion());

		neighborhoodIterator.SetBoundaryCondition(bc);

		while(!neighborhoodIterator.IsAtEnd()){
			auto index = neighborhoodIterator.GetIndex();
			float minAbsDifference=itk::NumericTraits<float>::max();

			for(int i=0;i<neighborhoodIterator.Size();i++){
				if(i!=neighborhoodIterator.Size()/2){
					float value =neighborhoodIterator.GetPixel(i);
					minAbsDifference = std::min(minAbsDifference,std::abs(value-neighborhoodIterator.GetCenterPixel()));
				}
			}
			m_F->SetPixel(index,minAbsDifference);
			++neighborhoodIterator;
		}

		typedef itk::ImageFileWriter<InputImageType> WriterType;
		typename WriterType::Pointer writer = WriterType::New();
		writer->SetInput(m_F);
		writer->SetFileName("./F.mha");
		writer->Update();

		typedef itk::ImageRegionConstIteratorWithIndex<InputImageType> IteratorType;

		IteratorType iterator(this->GetInput(),this->GetInput()->GetLargestPossibleRegion());

		iterator.GoToBegin();
		while(!iterator.IsAtEnd()){
			auto index = iterator.GetIndex();
			std::cout << index << std::endl;
			if(this->GetOutput()->GetPixel(index)==-1){
				std::vector<IndexType> L;
				itk::IdentifierType label = this->Stream(index,L);

				if(label==-1){
					label = m_NextIdentifier;
					m_NextIdentifier++;
				}

				for(IndexType i : L){
					std::cout << "\t" << i << "\t" << label << std::endl;
					this->GetOutput()->SetPixel(i,label);
				}
			}

			++iterator;
		}
	}



	WatershedImageFilter() {

		m_NextIdentifier = itk::NumericTraits<itk::IdentifierType>::ZeroValue();

		this->SetNumberOfIndexedInputs(3);

	}
	virtual ~WatershedImageFilter() {

	}

private:
	typename InputImageType::Pointer m_F;
	itk::IdentifierType m_NextIdentifier;

};
}
