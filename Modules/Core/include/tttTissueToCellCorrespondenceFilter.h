/*
 * tttTissueToCellCorrespondenceFilter.h
 *
 *  Created on: Feb 4, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTTISSUETOCELLCORRESPONDENCEFILTER_H_
#define MODULES_CORE_INCLUDE_TTTTISSUETOCELLCORRESPONDENCEFILTER_H_

#include "tttCellCorrespondencesSource.h"
namespace ttt{

template<class TInputTissue, class TOutputCellCorrespondences>
class TissueToCellCorrespondencesFilter : public CellCorrespondenceSource<TOutputCellCorrespondences>
{
public:
	typedef TissueToCellCorrespondencesFilter Self;
	typedef CellCorrespondenceSource<TOutputCellCorrespondences> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro(Self);
	itkTypeMacro(TissueToCellCorrespondencesFilter, CellCorrespondenceSource)
	;

	typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;
	using Superclass::MakeOutput;
	itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx) ITK_OVERRIDE;

	typedef TInputTissue InputTissueType;

	typedef TOutputCellCorrespondences OutputCellCorrespondencesType;
	typedef typename TOutputCellCorrespondences::Pointer OutputCellCorrespondencesPointer;

	using Superclass::SetInput;

	void SetInput(unsigned int idx, const TInputTissue *input);

	void SetInput0(const TInputTissue *input) {
		this->SetInput(0, input);
	}

	void SetInput1(const TInputTissue *input) {
		this->SetInput(1, input);
	}

	const TInputTissue * GetInput(unsigned int idx);
	const TInputTissue * GetInput0() {
		return this->GetInput(0);
	}

	const TInputTissue * GetInput1() {
		return this->GetInput(1);
	}

	OutputCellCorrespondencesType * GetOutput();

	void GenerateOutputInformation() ITK_OVERRIDE;

protected:
	TissueToCellCorrespondencesFilter();
	~TissueToCellCorrespondencesFilter();

private:
	TissueToCellCorrespondencesFilter(const TissueToCellCorrespondencesFilter &) ITK_DELETE_FUNCTION;
	void operator=(const TissueToCellCorrespondencesFilter &) ITK_DELETE_FUNCTION;
};
} // end namespace ttt
#include "tttTissueToCellCorrespondenceFilter.hxx"


#endif /* MODULES_CORE_INCLUDE_TTTTISSUETOCELLCORRESPONDENCEFILTER_H_ */
