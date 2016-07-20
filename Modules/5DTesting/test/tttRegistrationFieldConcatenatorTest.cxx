/*
 * tttRegistrationFieldConcatenator.cxx
 *
 *  Created on: May 31, 2016
 *      Author: morgan
 */

#include <itkVector.h>
#include <itkImage.h>
#include <itkImageSeriesReader.h>
#include <itkImageFileWriter.h>
#include <itkNumericSeriesFileNames.h>
#include <itkPasteImageFilter.h>

int main(int argc, char ** argv){

	typedef itk::Vector<float,3> VectorType;
	typedef itk::Image<VectorType,4> VectorImageType;

	typedef itk::ImageSeriesReader< VectorImageType >  ReaderType;
	typedef itk::ImageFileWriter<   VectorImageType >  WriterType;

	typedef itk::NumericSeriesFileNames NameGeneratorType;



	std::vector<std::string> fileNames;

	char * fileNameTemplate = argv[1];
	int beginIndex = atoi(argv[2]);
	int endIndex = atoi(argv[3]);

	int forward = atoi(argv[4]);

	if(forward){
		for(int t=beginIndex+1;t<=endIndex;t++){
			char c[10000];
			unsigned nchars = snprintf(c, sizeof(c), fileNameTemplate, t,t-1);
			std::cout << c << std::endl;
			fileNames.push_back(c);
		}
	}else{
		for(int t=beginIndex;t<endIndex;t++){
			char c[10000];
			unsigned nchars = snprintf(c, sizeof(c), fileNameTemplate, t,t+1);
			std::cout << c << std::endl;
			fileNames.push_back(c);
		}
	}



	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileNames(fileNames);
	reader->UpdateOutputInformation();

	typedef itk::PasteImageFilter<VectorImageType,VectorImageType> PasterType;
	typename PasterType::Pointer paster = PasterType::New();
	paster->SetInput(reader->GetOutput());

	VectorImageType::IndexType index;
	if(forward){
		index.Fill(0);
	}else{
		index[0]=0;
		index[1]=0;
		index[2]=1;
	}
	VectorImageType::Pointer vectorImage = VectorImageType::New();
	vectorImage->CopyInformation(reader->GetOutput());
	VectorImageType::RegionType region =reader->GetOutput()->GetLargestPossibleRegion();

	region.SetSize(3,region.GetSize(3)+1);

	vectorImage->SetRegions(region);
	vectorImage->Allocate();
	vectorImage->FillBuffer(0.0);

	paster->SetDestinationIndex(index);
	paster->SetSourceImage(reader->GetOutput());
	paster->SetSourceRegion(reader->GetOutput()->GetLargestPossibleRegion());
	paster->SetDestinationImage(vectorImage);


	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(paster->GetOutput());
	writer->SetFileName(argv[5]);
	writer->Update();
}
