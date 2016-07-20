
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkMultiplyImageFilter.h>
#include <itkAccumulateImageFilter.h>
#include <itkExtractImageFilter.h>
#include <itkUnaryFunctorImageFilter.h>

class XYProjectFunctor{

	typedef itk::Vector<float,3> InputVectorType;
	typedef itk::Vector<float,2> OutputVectorType;
public:
	XYProjectFunctor(){

	}

	OutputVectorType operator()(const InputVectorType & input){
		OutputVectorType output;
		output[0]=input[0];
		output[1]=input[1];
#if 0
		if(input.GetNorm()>1000){
			std::cout << input.GetNorm()<< std::endl;
		}
#endif
		//std::cout << input << "\t" << output << std::endl;
		return output;
	}
};

class NormFunctor{

	typedef itk::Vector<float,3> InputVectorType;

public:
	NormFunctor(){

	}

	float operator()(const InputVectorType & input){
		return input.GetNorm();
	}
};



int main(int argc, char ** argv){

	typedef itk::Vector<float,3> Vector3DType;
	typedef itk::Vector<float,2> Vector2DType;


	typedef itk::Image<Vector3DType,4> Input3DVectorImageType;
	typedef itk::Image<float,4> SurfaceImageType;

	typedef itk::Image<Vector3DType,3> Vector3D2DTImageType;

	typedef itk::Image<Vector2DType,3> Vector2D2DTImageType;


	typedef itk::ImageFileReader<SurfaceImageType> SurfaceReader;
	typedef itk::ImageFileReader<Input3DVectorImageType> InputReader;

	typedef itk::ImageFileWriter<Vector2D2DTImageType> OutputWriter;

	typename InputReader::Pointer inputReader = InputReader::New();
	inputReader->SetFileName(argv[1]);
	//inputReader->Update();
	//std::cout << "Input" <<inputReader->GetOutput()->GetLargestPossibleRegion() << std::endl;
	typename SurfaceReader::Pointer surfaceReader = SurfaceReader::New();
	surfaceReader->SetFileName(argv[2]);
	//surfaceReader->Update();
	//std::cout << "Surface" <<surfaceReader->GetOutput()->GetLargestPossibleRegion() << std::endl;




	typedef itk::UnaryFunctorImageFilter<Input3DVectorImageType,SurfaceImageType,NormFunctor> NormImageFilterType;

	NormImageFilterType::Pointer normFilter = NormImageFilterType::New();
	normFilter->SetInput(inputReader->GetOutput());


	typedef itk::ImageFileWriter<SurfaceImageType> NormWriterType;
	NormWriterType::Pointer normWriter = NormWriterType::New();
	normWriter->SetInput(normFilter->GetOutput());
	normWriter->SetFileName("normImage.mha");
	normWriter->Update();





	typedef itk::MultiplyImageFilter<Input3DVectorImageType,SurfaceImageType,Input3DVectorImageType> MultiplierType;
	typename MultiplierType::Pointer multiplier = MultiplierType::New();
	multiplier->SetInput1(inputReader->GetOutput());
	multiplier->SetInput2(surfaceReader->GetOutput());
	//multiplier->Update();
	//std::cout << "Multiplier" <<multiplier->GetOutput()->GetLargestPossibleRegion() << std::endl;


	typedef itk::AccumulateImageFilter<Input3DVectorImageType,Input3DVectorImageType> AccumulatorType;
	AccumulatorType::Pointer accumulator = AccumulatorType::New();
	accumulator->SetAccumulateDimension(2);
	accumulator->SetInput(multiplier->GetOutput());

	accumulator->UpdateOutputInformation();

	typedef itk::ExtractImageFilter<Input3DVectorImageType,Vector3D2DTImageType> ExtractorType;
	ExtractorType::Pointer extractor = ExtractorType::New();

	Input3DVectorImageType::RegionType largestRegion = accumulator->GetOutput()->GetLargestPossibleRegion();

	Input3DVectorImageType::RegionType extractionRegion;
	extractionRegion.SetIndex(0,largestRegion.GetIndex(0));
	extractionRegion.SetIndex(1,largestRegion.GetIndex(1));
	extractionRegion.SetIndex(2,largestRegion.GetIndex(2));
	extractionRegion.SetIndex(3,largestRegion.GetIndex(3));

	extractionRegion.SetSize(0,largestRegion.GetSize(0));
	extractionRegion.SetSize(1,largestRegion.GetSize(1));
	extractionRegion.SetSize(2,0);
	extractionRegion.SetSize(3,largestRegion.GetSize(3));


	extractor->SetExtractionRegion(extractionRegion);
	extractor->SetDirectionCollapseToIdentity();
	extractor->SetInput(accumulator->GetOutput());

	typedef itk::ImageFileWriter<Vector3D2DTImageType> ExtractedWriter;
	ExtractedWriter::Pointer extracted = ExtractedWriter::New();
	extracted->SetInput(extractor->GetOutput());
	extracted->SetFileName("tmpextracted.mha");
	extracted->Update();

	typedef itk::UnaryFunctorImageFilter<Vector3D2DTImageType,Vector2D2DTImageType,XYProjectFunctor> XYProjectorType;

	typename XYProjectorType::Pointer xyprojector = XYProjectorType::New();
	xyprojector->SetInput(extractor->GetOutput());


	typename OutputWriter::Pointer outputWriter = OutputWriter::New();
	outputWriter->SetFileName(argv[3]);
	outputWriter->SetInput(xyprojector->GetOutput());
	outputWriter->SetNumberOfStreamDivisions(33);
	outputWriter->Update();

}
