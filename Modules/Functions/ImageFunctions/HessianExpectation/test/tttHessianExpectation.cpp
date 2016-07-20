
#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionConstIteratorWithIndex.h>

int main(int argc, char ** argv){

	typedef itk::Image<float,3> ImageType;

	typedef itk::ImageFileReader<ImageType> ReaderType;

	typedef itk::ImageFileWriter<ImageType> WriterType;

	ReaderType::Pointer reader = ReaderType::New();

	reader->SetFileName(argv[1]);

	typedef itk::HessianRecursiveGaussianImageFilter<ImageType> HessianFilterType;
	typedef HessianFilterType::OutputImageType HessianImageType;
	typedef HessianImageType::PixelType HessianType;

	HessianFilterType::Pointer hessianFilter = HessianFilterType::New();

	hessianFilter->SetSigma(0.05);
	hessianFilter->SetInput(reader->GetOutput());

	hessianFilter->Update();

	HessianImageType::Pointer hessianImage = hessianFilter->GetOutput();
	hessianImage->DisconnectPipeline();

	typedef itk::ImageRegionConstIteratorWithIndex<HessianImageType> IteratorType;

	IteratorType hessianIterator(hessianImage,hessianImage->GetLargestPossibleRegion());

	typedef HessianType::EigenValuesArrayType EigenvalueArrayType;
	EigenvalueArrayType expectation;
	expectation.Fill(0.0);
	unsigned count=0;

	ImageType::Pointer small = ImageType::New();
	small->CopyInformation(reader->GetOutput());

	small->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
	small->Allocate();
	small->FillBuffer(0.0);
	ImageType::Pointer medium = ImageType::New();
	medium->CopyInformation(reader->GetOutput());
	medium->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());

	medium->Allocate();
	medium->FillBuffer(0.0);
	ImageType::Pointer large = ImageType::New();
	large->CopyInformation(reader->GetOutput());
	large->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
	large->Allocate();
	large->FillBuffer(0.0);

	while(!hessianIterator.IsAtEnd()){

		auto hessian = hessianIterator.Get();
		EigenvalueArrayType eigenValues;
		hessian.ComputeEigenValues(eigenValues);

		std::sort(eigenValues.Begin(),eigenValues.End(),[](double & a, double &b){
			return std::abs(a) > std::abs(b);
		});

		//std::cout << eigenValues[0] << "\t"<< eigenValues[1] << "\t" << eigenValues[2] <<std::endl;

		if(eigenValues[0]<0){
			expectation[0]+=eigenValues[0];
			expectation[1]+=eigenValues[1];
			expectation[2]+=eigenValues[2];

			count++;

			large->SetPixel(hessianIterator.GetIndex(),std::abs(eigenValues[0]));
			medium->SetPixel(hessianIterator.GetIndex(),std::abs(eigenValues[1]));
			small->SetPixel(hessianIterator.GetIndex(),std::abs(eigenValues[2]));
		}
		++hessianIterator;
	}
	expectation[0]=expectation[0]/count;
	expectation[1]=expectation[1]/count;
	expectation[2]=expectation[2]/count;

	WriterType::Pointer smallWriter = WriterType::New();
	smallWriter->SetInput(small);
	smallWriter->SetFileName("small.mha");
	smallWriter->Update();

	WriterType::Pointer mediumWriter = WriterType::New();
	mediumWriter->SetInput(medium);
	mediumWriter->SetFileName("medium.mha");
	mediumWriter->Update();

	WriterType::Pointer largeWriter = WriterType::New();
	largeWriter->SetInput(large);
	largeWriter->SetFileName("large.mha");
	largeWriter->Update();

	std::cout << "Expectation: \t" << expectation<< std::endl;

}
