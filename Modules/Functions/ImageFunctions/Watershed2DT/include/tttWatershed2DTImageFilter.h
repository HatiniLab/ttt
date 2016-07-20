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
template<class TInputImage, class TDisplacementField, class TLabelImage> class Watershed2DTImageFilter: public itk::ImageToImageFilter<TInputImage,
TLabelImage> {

public:
	typedef Watershed2DTImageFilter Self;
	typedef itk::ImageToImageFilter<TInputImage, TLabelImage> Superclass;
	typedef itk::SmartPointer<Watershed2DTImageFilter> Pointer;

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

	inline void SetForwardDisplacementField(const TDisplacementField * field) {
		this->itk::ProcessObject::SetNthInput(1, const_cast<TDisplacementField*>(field));
	}

	inline typename TDisplacementField::ConstPointer GetForwardDisplacementField() {
		return static_cast<const TDisplacementField*>(this->itk::ProcessObject::GetInput(1));

	}
	inline void SetBackwardDisplacementField(const TDisplacementField * field) {
		this->itk::ProcessObject::SetNthInput(2, const_cast<TDisplacementField*>(field));
	}

	inline typename TDisplacementField::ConstPointer GetBackwardDisplacementField() {
		return static_cast<const TDisplacementField*>(this->itk::ProcessObject::GetInput(2));
	}

	inline typename LabelImageType::Pointer GetLabelImage(){
		return static_cast< LabelImageType*>(this->itk::ProcessObject::GetOutput(1));
	}

protected:
#if 0

	void UpdateNeighbors(const NodeIndexType & index, const SpeedImageType *speedImage, LevelSetImageType *output) {
		NodeIndexType neighIndex = index;
		unsigned char label;

		int cluster = this->GetClusterImage()->GetPixel(index);

		for (unsigned int j = 0; j < 2; j++) {
			// update left neighbor
			if (index[j] > m_StartIndex[j]) {
				neighIndex[j] = index[j] - 1;
			}

			label = m_LabelImage->GetPixel(neighIndex);
			if ((label != AlivePoint) && (label != InitialTrialPoint) && (label != OutsidePoint)) {

				this->UpdateValue(neighIndex, speedImage, output, cluster);
			}

			// update right neighbor
			if (index[j] < m_LastIndex[j]) {
				neighIndex[j] = index[j] + 1;
			}

			label = m_LabelImage->GetPixel(neighIndex);

			if ((label != AlivePoint) && (label != InitialTrialPoint) && (label != OutsidePoint)) {
				this->UpdateValue(neighIndex, speedImage, output, cluster);
			}

			//reset neighIndex
			neighIndex[j] = index[j];
		}

		//Update forward neighbor

#if 1
		neighIndex[2]=index[2];

		if(index[2]< m_LastIndex[2]){
			OutputPointType point;
			this->GetInput()->TransformIndexToPhysicalPoint(index,point);
			auto displacement = this->GetForwardDisplacementField()->GetPixel(index);
			point[0]=point[0]-displacement[0];
			point[1]=point[1]-displacement[1];

			this->GetInput()->TransformPhysicalPointToIndex(point,neighIndex);
			neighIndex[2]++;
		}
		label = m_LabelImage->GetPixel(neighIndex);
			if ((label != AlivePoint) && (label != InitialTrialPoint) && (label != OutsidePoint)) {

				this->UpdateValue(neighIndex, speedImage, output, cluster);
			}
#endif
#if 1
		neighIndex[2]=index[2];

		//Updata backward neighbor
		if(index[2]> m_StartIndex[2]){
			OutputPointType point;
			this->GetInput()->TransformIndexToPhysicalPoint(index,point);
			auto displacement = this->GetBackwardDisplacementField()->GetPixel(index);
			point[0]=point[0]-displacement[0];
			point[1]=point[1]-displacement[1];

			this->GetInput()->TransformPhysicalPointToIndex(point,neighIndex);
			neighIndex[2]--;
		}

		if(m_LabelImage->GetLargestPossibleRegion().IsInside(neighIndex)){
			label = m_LabelImage->GetPixel(neighIndex);
				if ((label != AlivePoint) && (label != InitialTrialPoint) && (label != OutsidePoint)) {
					this->UpdateValue(neighIndex, speedImage, output, cluster);
				}

		}
#endif
	}
#endif
#if 0
	double

	UpdateValue(const NodeIndexType & index, const SpeedImageType *speedImage, LevelSetImageType *output, int cluster) {
		NodeIndexType neighIndex = index;

		PixelType neighValue;

		// just to make sure the index is initialized (really cautious)
		AxisNodeType node;
		node.SetIndex(index);

		for (unsigned int j = 0; j < 2; j++) {
			node.SetValue(m_LargeValue);

			// find smallest valued neighbor in this dimension
			for (int s = -1; s < 2; s = s + 2) {
				neighIndex[j] = index[j] + s;

				// make sure neighIndex is not outside from the image
				if ((neighIndex[j] > m_LastIndex[j]) || (neighIndex[j] < m_StartIndex[j])) {
					continue;
				}

				if (m_LabelImage->GetPixel(neighIndex) == AlivePoint) {
					neighValue = static_cast<PixelType>(output->GetPixel(neighIndex));

					// let's find the minimum value given a direction j
					if (node.GetValue() > neighValue) {
						node.SetValue(neighValue);
						node.SetIndex(neighIndex);
					}
				}
			}

			// put the minimum neighbor onto the heap
			m_NodesUsed[j] = node;
			m_NodesUsed[j].SetAxis(j);

			// reset neighIndex
			neighIndex[j] = index[j];
		}

		node.SetAxis(2);
		node.SetValue(m_LargeValue);


		if(neighIndex[2]>m_StartIndex[2]){

			OutputPointType point;
			this->GetInput()->TransformIndexToPhysicalPoint(index,point);
			auto displacement = this->GetBackwardDisplacementField()->GetPixel(index);
			point[0]=point[0]-displacement[0];
			point[1]=point[1]-displacement[1];

			this->GetInput()->TransformPhysicalPointToIndex(point,neighIndex);
			neighIndex[2]--;

			neighValue = static_cast<PixelType>(output->GetPixel(neighIndex));

			if (node.GetValue() > neighValue) {
				node.SetValue(neighValue);
				node.SetIndex(neighIndex);
			}

		}

		if(neighIndex[2]<m_LastIndex[2]-1){

			OutputPointType point;
			this->GetInput()->TransformIndexToPhysicalPoint(index,point);
			auto displacement = this->GetForwardDisplacementField()->GetPixel(index);
			point[0]=point[0]-displacement[0];
			point[1]=point[1]-displacement[1];

			this->GetInput()->TransformPhysicalPointToIndex(point,neighIndex);
			neighIndex[2]++;

			neighValue = static_cast<PixelType>(output->GetPixel(neighIndex));

			if (node.GetValue() > neighValue) {
				node.SetValue(neighValue);
				node.SetIndex(neighIndex);
			}

		}
		m_NodesUsed[2] = node;
		m_NodesUsed[2].SetAxis(2);


		// sort the local list
		std::sort(m_NodesUsed, m_NodesUsed + 3);

		// solve quadratic equation
		double solution = static_cast<double>(m_LargeValue);

		double aa(0.0);
		double bb(0.0);
		double cc;

		if (speedImage) {
			cc = static_cast<double>(speedImage->GetPixel(index)) / m_NormalizationFactor;
			cc = -1.0 * vnl_math_sqr(1.0 / cc);
		}

		OutputSpacingType spacing = /* this->GetOutput() */output->GetSpacing();

		double discrim;

		for (unsigned int j = 0; j < 3; j++) {
			node = m_NodesUsed[j];
			const double value = static_cast<double>(node.GetValue());

			if (solution >= value) {
				const int axis = node.GetAxis();
				// spaceFactor = \frac{1}{spacing[axis]^2}
				const double spaceFactor = vnl_math_sqr(1.0 / spacing[axis]);
				aa += spaceFactor;
				bb += value * spaceFactor;
				cc += vnl_math_sqr(value) * spaceFactor;

				discrim = vnl_math_sqr(bb) - aa * cc;
				if (discrim < 0.0) {
					// Discriminant of quadratic eqn. is negative
					itk::ExceptionObject err(__FILE__, __LINE__);
					err.SetLocation(ITK_LOCATION);
					err.SetDescription("Discriminant of quadratic equation is negative");
					throw err;
				}

				solution = ( vcl_sqrt(discrim) + bb ) / aa;

			}
			else
			{
				break;
			}
		}
		if(solution < m_CurrentValue){
			std::cout << "not possible" << std::endl;
		}

		if (solution < m_LargeValue) {
			// write solution to m_OutputLevelSet
			PixelType outputPixel = static_cast<PixelType>(solution);
			output->SetPixel(index, outputPixel);

			// insert point into trial heap
			m_LabelImage->SetPixel(index, TrialPoint);
			this->GetClusterImage()->SetPixel(index, cluster);

			std::cout << "\t " << index << "\t" << solution << std::endl;

			node.SetValue(outputPixel);
			node.SetIndex(index);
			m_TrialHeap.push(node);
		}

		return solution;
	}
#endif

	virtual void Successors(const IndexType & y,const std::vector<IndexType> & L,std::vector<IndexType> & successors){
		successors.clear();

		typename IndexType::OffsetType offset;

		offset.Fill(0);

		for(int i=-1;i<=1;i++){
			offset[0]=i;
			for(int j=-1;j<=1;j++){
				offset[1]=j;
				for(int k=-1;k<=1;k++){
					offset[2]=k;

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
				if(this->GetOutput()->GetPixel(z)!=-1){
					return this->GetOutput()->GetPixel(z);
				}else if(this->GetOutput()->GetPixel(z)<this->GetOutput()->GetPixel(y)){
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
		size[2]=0;

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
					this->GetOutput()->SetPixel(i,label);
				}
			}

			++iterator;
		}
	}



	Watershed2DTImageFilter() {

		m_NextIdentifier = itk::NumericTraits<itk::IdentifierType>::ZeroValue();

		this->SetNumberOfIndexedInputs(3);

	}
	virtual ~Watershed2DTImageFilter() {

	}

private:
	typename InputImageType::Pointer m_F;
	itk::IdentifierType m_NextIdentifier;

};
}
