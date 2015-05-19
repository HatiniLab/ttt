#include <gtest/gtest.h>
#include "itkFFTConvolutionImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "tttL1L2RichardsonLucyDeconvolutionImageFilter.h"
#include "itkSimpleFilterWatcher.h"
#include "itkRescaleIntensityImageFilter.h"

#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
class tttL1L2RichardsonLucyDeconvolutionTest : public ::testing::TestWithParam< std::tuple< std::string, std::string, double ,double , int , std::string > > {
protected:
	typedef float                              PixelType;
	static const unsigned int                         Dimension = 3;
	typedef itk::Image< PixelType, Dimension > ImageType;
	typedef itk::ImageFileReader< ImageType >  ReaderType;
	typedef itk::ImageFileWriter< ImageType >  WriterType;
	typedef itk::RescaleIntensityImageFilter<ImageType> Rescaler01FilterType;
	typedef ttt::L1L2RichardsonLucyDeconvolutionImageFilter< ImageType > DeconvolutionFilterType;

	typename ReaderType::Pointer m_KernelReader;
	typename ReaderType::Pointer m_InputReader;
	typename Rescaler01FilterType::Pointer m_Rescaler;
	typename DeconvolutionFilterType::Pointer m_Deconvoluter;
	typename WriterType::Pointer m_Writer;

protected:
	virtual void SetUp() {

		  m_KernelReader = ReaderType::New();
		  m_InputReader = ReaderType::New();

		  m_Rescaler = Rescaler01FilterType::New();
		  m_Rescaler->SetInput(m_InputReader->GetOutput());
		  m_Rescaler->SetOutputMinimum(0);
		  m_Rescaler->SetOutputMaximum(1);

		  m_Deconvoluter = DeconvolutionFilterType::New();
		  m_Deconvoluter->SetKernelImage( m_KernelReader->GetOutput() );
		  m_Deconvoluter->SetInput( m_Rescaler->GetOutput() );

		  m_Writer = WriterType::New();
		  m_Writer->SetInput( m_Deconvoluter->GetOutput() );

	}
	virtual void TearDown() {
		m_KernelReader=ITK_NULLPTR;
		m_InputReader=ITK_NULLPTR;
		m_Rescaler=ITK_NULLPTR;
		m_Deconvoluter=ITK_NULLPTR;
		m_Writer=ITK_NULLPTR;
	}
};

TEST_P(tttL1L2RichardsonLucyDeconvolutionTest,General){
	std::string inputFileName(std::get<0>(GetParam()));
	std::string kernelFileName(std::get<1>(GetParam()));

	std::cerr << "Running for input" << inputFileName << std::endl;
	m_InputReader->SetFileName(inputFileName);
	m_KernelReader->SetFileName(kernelFileName);

	double iterations = std::get<2>(GetParam());
	double alpha= std::get<3>(GetParam());
	double beta = std::get<4>(GetParam());

	m_Deconvoluter->SetAlpha(alpha);
	m_Deconvoluter->SetBeta(beta);
	m_Deconvoluter->NormalizeOn();
	m_Deconvoluter->SetNumberOfIterations(iterations);

	itk::SimpleFilterWatcher m_Deconvoluter(m_Deconvoluter);
    m_Writer->SetFileName(std::get<5>(GetParam()));
    try{
    	m_Writer->Update();
    }catch ( itk::ExceptionObject & e ){
        std::cerr << "Unexpected exception caught when writing deconvolution image: "
                  << e << std::endl;
    }
    EXPECT_EQ(0,1);
};

  // Write the deconvolution result

INSTANTIATE_TEST_CASE_P(instantiation,tttL1L2RichardsonLucyDeconvolutionTest,::testing::Values(std::make_tuple("input.mha","blur.mha",0.1,0.1,10,"output.mha"),std::make_tuple("input.mha","blur.mha",0.1,0.1,10,"output.mha"),std::make_tuple("input.mha","blur.mha",0.1,0.1,10,"output.mha")));
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
