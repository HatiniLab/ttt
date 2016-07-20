
#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionConstIteratorWithIndex.h>

int main(int argc, char ** argv){

	typedef itk::Image<double,3> EigenvalueImageType;
	typedef itk::SymmetricSecondRankTensor<double,3> HessianType;
	typedef itk::Image<HessianType,3> HessianImageType;


	typedef itk::ImageFileReader<HessianImageType> ReaderType;

	typedef itk::ImageFileWriter<EigenvalueImageType> WriterType;

	ReaderType::Pointer reader = ReaderType::New();

	reader->SetFileName(argv[1]);

	reader->Update();

	HessianImageType::Pointer hessianImage = reader->GetOutput();
	hessianImage->DisconnectPipeline();

	typedef itk::ImageRegionConstIteratorWithIndex<HessianImageType> IteratorType;

	IteratorType hessianIterator(hessianImage,hessianImage->GetLargestPossibleRegion());

	typedef HessianType::EigenValuesArrayType EigenvalueArrayType;
	EigenvalueArrayType expectation;
	expectation.Fill(0.0);
	unsigned count=0;

	EigenvalueImageType::Pointer small = EigenvalueImageType::New();
	small->CopyInformation(hessianImage);

	small->SetRegions(hessianImage->GetLargestPossibleRegion());
	small->Allocate();
	small->FillBuffer(0.0);
	EigenvalueImageType::Pointer medium = EigenvalueImageType::New();
	medium->CopyInformation(hessianImage);
	medium->SetRegions(hessianImage->GetLargestPossibleRegion());

	medium->Allocate();
	medium->FillBuffer(0.0);
	EigenvalueImageType::Pointer large = EigenvalueImageType::New();
	large->CopyInformation(hessianImage);
	large->SetRegions(hessianImage->GetLargestPossibleRegion());
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
