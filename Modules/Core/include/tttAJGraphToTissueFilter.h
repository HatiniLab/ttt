/*
 * tttAJGraphToTissueFilter.h
 *
 *  Created on: Feb 4, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTAJGRAPHTOTISSUEFILTER_H_
#define MODULES_CORE_INCLUDE_TTTAJGRAPHTOTISSUEFILTER_H_


#include <tttTissueSource.h>

namespace ttt {

template<typename TInputAJGraph, typename TOutputTissue>
class AJGraphToTissueFilter: public TissueSource<TOutputTissue> {
public:
	typedef AJGraphToTissueFilter Self;
	typedef TissueSource<TOutputTissue> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro(Self);
	itkTypeMacro(AJGraphToTissueFilter, AJGraphSource)
	;

	typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;
	using Superclass::MakeOutput;
	itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx) ITK_OVERRIDE;

	typedef TInputAJGraph InputAJGraphType;

	typedef TOutputTissue OutputTissueType;
	typedef typename OutputTissueType::Pointer OutputTissuePointer;

	using Superclass::SetInput;

	void SetInput(unsigned int idx, const TInputAJGraph *input);

	void SetInput(const TInputAJGraph *input) {
		this->SetInput(0, input);
	}

	const TInputAJGraph * GetInput(unsigned int idx);
	const TInputAJGraph * GetInput() {
		return this->GetInput(0);
	}

	OutputTissueType * GetOutput();

	void GenerateOutputInformation() ITK_OVERRIDE;

protected:
	AJGraphToTissueFilter();
	~AJGraphToTissueFilter();

private:
	AJGraphToTissueFilter(const AJGraphToTissueFilter &) ITK_DELETE_FUNCTION;
	void operator=(const AJGraphToTissueFilter &) ITK_DELETE_FUNCTION;
};
} // end namespace ttt
#include "tttAJGraphToTissueFilter.hxx"


#endif /* MODULES_CORE_INCLUDE_TTTAJGRAPHTOTISSUEFILTER_H_ */
