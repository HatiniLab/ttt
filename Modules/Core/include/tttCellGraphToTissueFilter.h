/*
 * tttCellGraphToTissueFilter.h
 *
 *  Created on: Jun 29, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTCELLGRAPHTOTISSUEFILTER_H_
#define MODULES_CORE_INCLUDE_TTTCELLGRAPHTOTISSUEFILTER_H_
#include <tttTissueSource.h>
namespace ttt{


template<typename TInputCellGraph, typename TOutputTissue>
class CellGraphToTissueFilter: public TissueSource<TOutputTissue> {
public:
	typedef CellGraphToTissueFilter Self;
	typedef TissueSource<TOutputTissue> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro(Self);
	itkTypeMacro(CellGraphToTissueFilter, TissueSource);

	typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;
	using Superclass::MakeOutput;
	itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx) ITK_OVERRIDE;

	typedef TInputCellGraph InputCellGraphType;

	typedef TOutputTissue OutputTissueType;
	typedef typename OutputTissueType::Pointer OutputTissuePointer;

	using Superclass::SetInput;

	void SetInput(unsigned int idx, const TInputCellGraph *input);

	void SetInput(const TInputCellGraph *input) {
		this->SetInput(0, input);
	}

	const TInputCellGraph * GetInput(unsigned int idx);
	const TInputCellGraph * GetInput() {
		return this->GetInput(0);
	}

	OutputTissueType * GetOutput();

	void GenerateOutputInformation() ITK_OVERRIDE;

protected:
	CellGraphToTissueFilter();
	~CellGraphToTissueFilter();

private:
	CellGraphToTissueFilter(const CellGraphToTissueFilter &) ITK_DELETE_FUNCTION;
	void operator=(const CellGraphToTissueFilter &) ITK_DELETE_FUNCTION;
};
} // end namespace ttt
#include "tttCellGraphToTissueFilter.hxx"



#endif /* MODULES_CORE_INCLUDE_TTTCELLGRAPHTOTISSUEFILTER_H_ */
