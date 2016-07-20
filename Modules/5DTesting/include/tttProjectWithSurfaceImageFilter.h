
namespace ttt{

template<class TInputImage,class TSurfaceImage, class TOutputImage> class ProjectWithSurfaceImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage> {

public:
	typedef ProjectWithSurfaceImageFilter<TInputImage,TOutputImage> Self;
	typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro(Self);

	typedef TInputImage InputImageType;
	typedef TInputImage::RegionType InputRegionType;

	typedef TInputImage SurfaceImageType;
	typedef TOutputImage OutputImageType;
	typedef TInputImage::RegionType OutputRegionType;


protected:

	virtual void GenerateOutputInformation( ){
		InputRegionType input = this->GetInput()->GetLargestPossibleRegion();
		ouput =
		OutputRegionType output;

	}

	virtual void Enlarge
	virtual void GenerateData( ){

	}


private:


};

}
