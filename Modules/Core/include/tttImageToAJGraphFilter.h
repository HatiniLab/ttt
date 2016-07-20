/*
 * tttImageToAJGraphFilter.h
 *
 *  Created on: Feb 3, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTIMAGETOAJGRAPHFILTER_H_
#define MODULES_CORE_INCLUDE_TTTIMAGETOAJGRAPHFILTER_H_
#include "tttAJGraphSource.h"
namespace ttt {

template<typename TInputImage, typename TOutputAJGraph>
class ImageToAJGraphFilter: public AJGraphSource<TOutputAJGraph> {
public:
	typedef ImageToAJGraphFilter Self;
	typedef AJGraphSource<TOutputAJGraph> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkTypeMacro(ImageToAJGraphFilter, AJGraphSource)
	;

	typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;
	using Superclass::MakeOutput;
	itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx) ITK_OVERRIDE;

	typedef TInputImage InputImageType;
	typedef typename InputImageType::Pointer InputImagePointer;
	typedef typename InputImageType::ConstPointer InputImageConstPointer;
	typedef typename InputImageType::RegionType InputImageRegionType;
	typedef typename InputImageType::PixelType InputImagePixelType;

	typedef TOutputAJGraph OutputAJGraphType;
	typedef typename OutputAJGraphType::Pointer OutputAJGraphPointer;

	using Superclass::SetInput;

	void SetInput(unsigned int idx, const InputImageType *input);

	void SetInput(const InputImageType *input) {
		this->SetInput(0, input);
	}

	const InputImageType * GetInput(unsigned int idx);
	const InputImageType * GetInput() {
		return this->GetInput(0);
	}

	OutputAJGraphType * GetOutput();

	void GenerateOutputInformation() ITK_OVERRIDE;

protected:
	ImageToAJGraphFilter();
	~ImageToAJGraphFilter();

private:
	ImageToAJGraphFilter(const ImageToAJGraphFilter &) ITK_DELETE_FUNCTION;
	void operator=(const ImageToAJGraphFilter &) ITK_DELETE_FUNCTION;
};
} // end namespace ttt

#include "tttImageToAJGraphFilter.hxx"
#endif /* MODULES_CORE_INCLUDE_TTTIMAGETOAJGRAPHFILTER_H_ */
