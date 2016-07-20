/*
 * tttAJGraphToAJGraphFilter.h
 *
 *  Created on: Feb 3, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTAJGRAPHTOAJGRAPHFILTER_H_
#define MODULES_CORE_INCLUDE_TTTAJGRAPHTOAJGRAPHFILTER_H_

#include <tttAJGraphSource.h>

namespace ttt {

template<typename TInputAJGraph, typename TOutputAJGraph>
class AJGraphToAJGraphFilter: public AJGraphSource<TOutputAJGraph> {
public:
	typedef AJGraphToAJGraphFilter Self;
	typedef AJGraphSource<TOutputAJGraph> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkTypeMacro(ImageToAJGraphFilter, AJGraphSource)
	;

	typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;
	using Superclass::MakeOutput;
	itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx) ITK_OVERRIDE;

	typedef TInputAJGraph InputAJGraphType;

	typedef TOutputAJGraph OutputAJGraphType;
	typedef typename OutputAJGraphType::Pointer OutputAJGraphPointer;

	using Superclass::SetInput;

	void SetInput(unsigned int idx, const TInputAJGraph *input);

	void SetInput(const TInputAJGraph *input) {
		this->SetInput(0, input);
	}

	const TInputAJGraph * GetInput(unsigned int idx);
	const TInputAJGraph * GetInput() {
		return this->GetInput(0);
	}

	OutputAJGraphType * GetOutput();

	void GenerateOutputInformation() ITK_OVERRIDE;

protected:
	AJGraphToAJGraphFilter();
	~AJGraphToAJGraphFilter();

private:
	AJGraphToAJGraphFilter(const AJGraphToAJGraphFilter &) ITK_DELETE_FUNCTION;
	void operator=(const AJGraphToAJGraphFilter &) ITK_DELETE_FUNCTION;
};
} // end namespace ttt
#include "tttAJGraphToAJGraphFilter.hxx"
#endif /* MODULES_CORE_INCLUDE_TTTAJGRAPHTOAJGRAPHFILTER_H_ */
