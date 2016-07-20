#include "itkImageToImageFilter.h"
#include "itkLevelSet.h"
#include "itkImageRegionIterator.h"
#include <queue>
namespace ttt {
template<class TSpeedImage, class TDisplacementField, class TLevelSet> class FastMarching2DTImageFilter: public itk::ImageToImageFilter<TSpeedImage,
		TLevelSet> {

public:
	typedef FastMarching2DTImageFilter Self;
	typedef itk::ImageToImageFilter<TSpeedImage, TLevelSet> Superclass;
	typedef itk::SmartPointer<FastMarching2DTImageFilter> Pointer;

	itkNewMacro(Self)
	;itkTypeMacro(Self,Superclass)

	/** Typedef support of level set method types. */
	typedef itk::LevelSetTypeDefault<TLevelSet> LevelSetType;
	typedef typename LevelSetType::LevelSetImageType LevelSetImageType;
	typedef typename LevelSetType::LevelSetPointer LevelSetPointer;
	typedef typename LevelSetType::PixelType PixelType;
	typedef typename LevelSetType::NodeType NodeType;
	typedef typename NodeType::IndexType NodeIndexType;
	typedef typename LevelSetType::NodeContainer NodeContainer;
	typedef typename LevelSetType::NodeContainerPointer NodeContainerPointer;

	typedef typename LevelSetImageType::SizeType OutputSizeType;
	typedef typename LevelSetImageType::RegionType OutputRegionType;
	typedef typename LevelSetImageType::SpacingType OutputSpacingType;
	typedef typename LevelSetImageType::DirectionType OutputDirectionType;
	typedef typename LevelSetImageType::PointType OutputPointType;

	typedef TSpeedImage SpeedImageType;
	typedef typename TSpeedImage::Pointer SpeedImagePointer;
	typedef typename TSpeedImage::ConstPointer SpeedImageConstPointer;
	/** Enum of Fast Marching algorithm point types. FarPoints represent far
	 * away points; TrialPoints represent points within a narrowband of the
	 * propagating front; and AlivePoints represent points which have already
	 * been processed. */
	enum LabelType {
		FarPoint = 0, AlivePoint, TrialPoint, InitialTrialPoint, OutsidePoint
	};

	/** LabelImage typedef support. */
	typedef itk::Image<unsigned char, 3> LabelImageType;
	typedef typename LabelImageType::Pointer LabelImagePointer;

	typedef itk::Image<int, 3> ClusterImageType;

	typedef typename ClusterImageType::Pointer ClusterImagePointer;

	/** Set the container of Trial Points representing the initial front.
	 * Trial points are represented as a VectorContainer of LevelSetNodes. */
	void SetTrialPoints(NodeContainer *points) {
		m_TrialPoints = points;
		this->Modified();
	}

	/** Get the container of Trial Points representing the initial front. */
	NodeContainerPointer GetTrialPoints() {
		return m_TrialPoints;
	}

	/** Get the point type label image. */
	LabelImagePointer GetLabelImage() const {
		return m_LabelImage;
	}


	class AxisNodeType: public NodeType {
	public:
		int GetAxis() const {
			return m_Axis;
		}
		void SetAxis(int axis) {
			m_Axis = axis;
		}
		const AxisNodeType & operator=(const NodeType & node) {
			this->NodeType::operator=(node);
			return *this;
		}

	private:
		int m_Axis;
	};

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

	inline typename ClusterImageType::Pointer GetClusterImage(){
		return static_cast< ClusterImageType*>(this->itk::ProcessObject::GetOutput(1));
	}

protected:

	virtual void Initialize(LevelSetImageType *output) {
		// allocate memory for the output buffer
		output->SetBufferedRegion(output->GetRequestedRegion());
		output->Allocate();

		// cache some buffered region information
		m_BufferedRegion = output->GetBufferedRegion();
		m_StartIndex = m_BufferedRegion.GetIndex();
		m_LastIndex = m_StartIndex + m_BufferedRegion.GetSize();
		typename LevelSetImageType::OffsetType offset;
		offset.Fill(1);
		m_LastIndex -= offset;

		m_LabelImage = LabelImageType::New();
		// allocate memory for the PointTypeImage
		m_LabelImage->CopyInformation(output);
		m_LabelImage->SetBufferedRegion(output->GetBufferedRegion());
		m_LabelImage->Allocate();

		ClusterImageType::Pointer clusterImage = this->GetClusterImage();
		clusterImage->CopyInformation(output);
		clusterImage->SetBufferedRegion(output->GetBufferedRegion());
		clusterImage->Allocate();
		clusterImage->FillBuffer(-1);

		// set all output value to infinity
		typedef itk::ImageRegionIterator<LevelSetImageType> OutputIterator;

		OutputIterator outIt(output, output->GetBufferedRegion());

		PixelType outputPixel;
		outputPixel = m_LargeValue;

		outIt.GoToBegin();

		while (!outIt.IsAtEnd()) {
			outIt.Set(outputPixel);
			++outIt;
		}

		// set all points type to FarPoint
		typedef itk::ImageRegionIterator<LabelImageType> LabelIterator;

		LabelIterator typeIt(m_LabelImage, m_LabelImage->GetBufferedRegion());

		typeIt.GoToBegin();
		while (!typeIt.IsAtEnd()) {
			typeIt.Set(FarPoint);
			++typeIt;
		}

		// process input alive points
		AxisNodeType node;
		NodeIndexType idx;

		if (m_AlivePoints) {
			typename NodeContainer::ConstIterator pointsIter = m_AlivePoints->Begin();
			typename NodeContainer::ConstIterator pointsEnd = m_AlivePoints->End();

			while (pointsIter != pointsEnd) {
				// get node from alive points container
				node = pointsIter.Value();
				idx = node.GetIndex();

				// check if node index is within the output level set
				if (m_BufferedRegion.IsInside(idx)) {
					// make this an alive point
					m_LabelImage->SetPixel(idx, AlivePoint);

					outputPixel = node.GetValue();
					output->SetPixel(idx, outputPixel);
				}

				++pointsIter;
			}
		}

		if (m_OutsidePoints) {
			typename NodeContainer::ConstIterator pointsIter = m_OutsidePoints->Begin();
			typename NodeContainer::ConstIterator pointsEnd = m_OutsidePoints->End();

			while (pointsIter != pointsEnd) {
				// get node from alive points container
				node = pointsIter.Value();
				idx = node.GetIndex();

				// check if node index is within the output level set
				if (m_BufferedRegion.IsInside(idx)) {
					// make this an alive point
					m_LabelImage->SetPixel(idx, OutsidePoint);

					outputPixel = node.GetValue();
					output->SetPixel(idx, outputPixel);
				}

				++pointsIter;
			}
		}

		// make sure the heap is empty
		while (!m_TrialHeap.empty()) {
			m_TrialHeap.pop();
		}

		int cluster = 0;
		// process the input trial points
		if (m_TrialPoints) {
			typename NodeContainer::ConstIterator pointsIter = m_TrialPoints->Begin();
			typename NodeContainer::ConstIterator pointsEnd = m_TrialPoints->End();

			while (pointsIter != pointsEnd) {
				// get node from trial points container
				node = pointsIter.Value();
				idx = node.GetIndex();

				// check if node index is within the output level set
				if (m_BufferedRegion.IsInside(idx)) {
					// make this an initial trial point
					m_LabelImage->SetPixel(idx, InitialTrialPoint);

					clusterImage->SetPixel(idx, cluster++);

					outputPixel = node.GetValue();
					output->SetPixel(idx, outputPixel);

					m_TrialHeap.push(node);
				}
				++pointsIter;
			}
		}
	}

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
	virtual void GenerateData() {
		if (m_NormalizationFactor < vnl_math::eps) {
			itk::ExceptionObject err(__FILE__, __LINE__);
			err.SetLocation(ITK_LOCATION);
			err.SetDescription("Normalization Factor is null or negative");
			throw err;
		}

		LevelSetPointer output = this->GetOutput();
		SpeedImageConstPointer speedImage = this->GetInput();

		this->Initialize(output);

		// process points on the heap
		AxisNodeType node;
		double oldProgress = 0;

		this->UpdateProgress(0.0);   // Send first progress event

		// CACHE
		while (!m_TrialHeap.empty()) {
			// get the node with the smallest value
			node = m_TrialHeap.top();
			m_TrialHeap.pop();


			// does this node contain the current value ?
			m_CurrentValue = static_cast<double>(output->GetPixel(node.GetIndex()));

			std::cout << node.GetIndex() << "\t" << m_CurrentValue << std::endl;

			if (node.GetValue() == m_CurrentValue) {
				// is this node already alive ?
				if (m_LabelImage->GetPixel(node.GetIndex()) != AlivePoint) {
					if (m_CurrentValue > m_StoppingValue) {
						this->UpdateProgress(1.0);
						break;
					}

					// set this node as alive
					m_LabelImage->SetPixel(node.GetIndex(), AlivePoint);

					// update its neighbors
					this->UpdateNeighbors(node.GetIndex(), speedImage, output);

					// Send events every certain number of points.
					const double newProgress = m_CurrentValue / m_StoppingValue;
					if (newProgress - oldProgress > 0.01)  // update every 1%
							{
						this->UpdateProgress(newProgress);
						oldProgress = newProgress;
						if (this->GetAbortGenerateData()) {
							this->InvokeEvent(itk::AbortEvent());
							this->ResetPipeline();
							itk::ProcessAborted e(__FILE__, __LINE__);
							e.SetDescription("Process aborted.");
							e.SetLocation(ITK_LOCATION);
							throw e;
						}
					}
				}
			}
		}


		std::cout << "end" << std::endl;
	}



	typename itk::DataObject::Pointer MakeOutput(unsigned int idx){
	  itk::DataObject::Pointer output;

	  switch ( idx )
	    {
	    case 0:
	      output = ( LevelSetImageType::New() ).GetPointer();
	      break;
	    case 1:
	      output = ( ClusterImageType::New() ).GetPointer();
	      break;
	    default:
	      std::cerr << "No output " << idx << std::endl;
	      output = NULL;
	      break;
	    }
	  return output.GetPointer();
	}
	FastMarching2DTImageFilter() {
		m_StoppingValue=200;
		m_CurrentValue=itk::NumericTraits<PixelType>::min();
		m_NormalizationFactor=1;
		m_LargeValue=static_cast< PixelType >( itk::NumericTraits< PixelType >::max() / 2.0 );

		this->SetNumberOfIndexedInputs(3);
		this->SetNumberOfRequiredOutputs(2);

		this->SetNthOutput(0,this->MakeOutput(0));
		this->SetNthOutput(1,this->MakeOutput(1));
	}
	virtual ~FastMarching2DTImageFilter() {

	}

private:
	typedef std::vector<AxisNodeType> HeapContainer;
	typedef std::greater<AxisNodeType> NodeComparer;
	typedef std::priority_queue<AxisNodeType, HeapContainer, NodeComparer> HeapType;

	HeapType m_TrialHeap;

	typename LabelImageType::Pointer m_LabelImage;


	NodeContainerPointer m_TrialPoints;
	NodeContainerPointer m_AlivePoints;
	NodeContainerPointer m_OutsidePoints;

	double m_NormalizationFactor;

	double m_StoppingValue;
	double m_CurrentValue;
	NodeIndexType m_StartIndex;
	NodeIndexType m_LastIndex;

	OutputRegionType m_BufferedRegion;

	double m_LargeValue;

	AxisNodeType m_NodesUsed[3];

};
}
