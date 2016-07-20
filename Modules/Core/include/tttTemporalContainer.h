/*
 * tttTemporalContainer.h
 *
 *  Created on: Jul 19, 2016
 *      Author: morgan
 */

#ifndef MODULES_CORE_INCLUDE_TTTTEMPORALCONTAINER_H_
#define MODULES_CORE_INCLUDE_TTTTEMPORALCONTAINER_H_
#include <itkDataObject.h>
namespace ttt{
template<class TFrameContainer> class TemporalContainer : public itk::DataObject{
public:
	typedef TemporalContainer<TFrameContainer> Self;
	typedef itk::DataObject Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef TFrameContainer FrameContainerType;
	typedef typename TFrameContainer::Pointer FrameContainerPointer;

	itkNewMacro(Self);
	void SetNumberOfFrames(unsigned numFrames){
		m_Frames.resize(numFrames);
	}

	FrameContainerPointer GetFrame(unsigned numFrame){
		return m_Frames[numFrame];
	}
	void SetFrame(unsigned numFrame, const FrameContainerPointer & frame){
		m_Frames[numFrame]=frame;
	}

protected:
	TemporalContainer(){

	}
private:

	std::vector<FrameContainerPointer> m_Frames;
};

}




#endif /* MODULES_CORE_INCLUDE_TTTTEMPORALCONTAINER_H_ */
