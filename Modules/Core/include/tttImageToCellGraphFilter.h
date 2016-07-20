/*
 * tttImageToAJGraphFilter.h
 *
 *  Created on: Feb 3, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTIMAGETOCELLGRAPHFILTER_H_
#define MODULES_CORE_INCLUDE_TTTIMAGETOCELLGRAPHFILTER_H_
#include "tttCellGraphSource.h"
namespace ttt {

template<typename TInputImage, typename TOutputCellGraph>
class ImageToCellGraphFilter: public CellGraphSource<TOutputCellGraph> {
public:
	typedef ImageToCellGraphFilter Self;
	typedef CellGraphSource<TOutputCellGraph> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkTypeMacro(ImageToCellGraphFilter, CellGraphSource)
	;

	typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;
	using Superclass::MakeOutput;
	itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx) ITK_OVERRIDE;

	typedef TInputImage InputImageType;
	typedef typename InputImageType::Pointer InputImagePointer;
	typedef typename InputImageType::ConstPointer InputImageConstPointer;
	typedef typename InputImageType::RegionType InputImageRegionType;
	typedef typename InputImageType::PixelType InputImagePixelType;

	typedef TOutputCellGraph OutputCellGraphType;
	typedef typename OutputCellGraphType::Pointer OutputCellGraphPointer;

	using Superclass::SetInput;

	void SetInput(unsigned int idx, const InputImageType *input);

	void SetInput(const InputImageType *input) {
		this->SetInput(0, input);
	}

	const InputImageType * GetInput(unsigned int idx);
	const InputImageType * GetInput() {
		return this->GetInput(0);
	}

	OutputCellGraphType * GetOutput();

	void GenerateOutputInformation() ITK_OVERRIDE;

protected:
	ImageToCellGraphFilter();
	~ImageToCellGraphFilter();

private:
	ImageToCellGraphFilter(const ImageToCellGraphFilter &) ITK_DELETE_FUNCTION;
	void operator=(const ImageToCellGraphFilter &) ITK_DELETE_FUNCTION;
};
} // end namespace ttt

#include "tttImageToCellGraphFilter.hxx"
#endif /* MODULES_CORE_INCLUDE_TTTIMAGETOAJGRAPHFILTER_H_ */
