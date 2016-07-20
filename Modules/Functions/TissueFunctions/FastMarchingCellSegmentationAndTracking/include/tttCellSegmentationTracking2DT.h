/*
 * tttCellSegmentationTracking2DT.h
 *
 *  Created on: Jun 29, 2016
 *      Author: morgan
 */

#ifndef MODULES_FUNCTIONS_TISSUEFUNCTIONS_FASTMARCHINGCELLSEGMENTATIONANDTRACKING_INCLUDE_TTTCELLSEGMENTATIONTRACKING2DT_H_
#define MODULES_FUNCTIONS_TISSUEFUNCTIONS_FASTMARCHINGCELLSEGMENTATIONANDTRACKING_INCLUDE_TTTCELLSEGMENTATIONTRACKING2DT_H_

#include <tttCellGraphToTissueFilter.h>
#include <tttFastMarching2DTImageFilter.h>

namespace ttt{
template<class TInputCellGraph,class TInputSpeedImage,class TInputRegistrationField, class TOutputTissue> class CellSegmentationTracking2DT : public CellGraphToTissueFilter<TInputCellGraph,TOutputTissue>{
public:
	typedef CellSegmentationTracking2DT<TInputCellGraph,TInputSpeedImage,TInputRegistrationField,TOutputTissue> Self;
	typedef CellGraphToTissueFilter<TInputCellGraph,TOutputTissue> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef TInputSpeedImage InputSpeedImageType;
	typedef TInputCellGraph InputCellGraphType;
	typedef TOutputTissue OuputTissueType;
	typedef TInputRegistrationField InputRegistrationFieldType;

	typedef itk::Image<double,3> LevelsetImageType;

	itkNewMacro(Self);

	virtual void SetForwardDisplacementField(const TInputRegistrationField * forward){
		this->SetNthInput(2,const_cast<TInputRegistrationField*>(forward));
	}
	virtual const TInputRegistrationField * GetForwardDisplacementField(){
		return static_cast<const TInputRegistrationField *>(this->itk::ProcessObject::GetInput(2));
	}

	virtual void SetBackwardDisplacementField(const TInputRegistrationField * backward){
		this->SetNthInput(3,const_cast<TInputRegistrationField*>(backward));
	}

	virtual const TInputRegistrationField * GetBackwardDisplacementField(){
		return static_cast<const TInputRegistrationField *>(this->itk::ProcessObject::GetInput(3));
	}

	virtual void SetSpeedImage(const TInputSpeedImage * image){
		this->SetNthInput(1,const_cast<TInputSpeedImage*>(image));
	}
	virtual const TInputSpeedImage * GetSpeedImage(){
		return static_cast<const TInputSpeedImage *>(this->itk::ProcessObject::GetInput(1));

	}
protected:

	CellSegmentationTracking2DT(){
		this->SetNumberOfRequiredInputs(2);
	}
	virtual ~CellSegmentationTracking2DT(){

	}
	virtual void GenerateData(){

		typedef ttt::FastMarching2DTImageFilter<InputSpeedImageType,InputRegistrationFieldType,LevelsetImageType> FastMarchingType;

		typename FastMarchingType::NodeContainerPointer nodeContainer = FastMarchingType::NodeContainer::New();

		typename FastMarchingType::Pointer fastMarchingFilter = FastMarchingType::New();
		fastMarchingFilter->SetInput(this->GetSpeedImage());


		int element=0;
		for(auto it = this->GetInput()->CellsBegin();it!=this->GetInput()->CellsEnd();++it){
			typename FastMarchingType::NodeIndexType index;
			index[0]=220;
			index[1]=120;
			index[2]=0;

			auto cell = this->GetInput()->GetCell(*it);

			auto position = cell->GetPosition();
			this->GetSpeedImage()->TransformPhysicalPointToIndex(position,index);
			typename FastMarchingType::NodeType node;
			node.SetIndex(index);
			node.SetValue(0.0);
			nodeContainer->InsertElement(element++,node);
		}


		typename FastMarchingType::Pointer fastMarching = FastMarchingType::New();
		fastMarching->SetTrialPoints(nodeContainer);
		fastMarching->SetInput(this->GetSpeedImage());
		fastMarching->SetForwardDisplacementField(this->GetForwardDisplacementField());
		fastMarching->SetBackwardDisplacementField(this->GetBackwardDisplacementField());
		fastMarching->Update();
	}


private:

};
}


#endif /* MODULES_FUNCTIONS_TISSUEFUNCTIONS_FASTMARCHINGCELLSEGMENTATIONANDTRACKING_INCLUDE_TTTCELLSEGMENTATIONTRACKING2DT_H_ */
