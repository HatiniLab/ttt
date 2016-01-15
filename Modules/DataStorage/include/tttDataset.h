/*
 * tissuetrackingabstractproject.h
 *
 *  Created on: Apr 25, 2014
 *      Author: morgan
 */

#ifndef ABSTRACTDATAREPOSITORY_H_
#define ABSTRACTDATAREPOSITORY_H_

#include <vector>
#include <memory>
#include <fstream>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <vtk_jsoncpp.h>
#include "tttFrame.h"



namespace ttt{
/**
 * TODO
 */
class Dataset{

public:
	/**
	 * TODO
	 */
	typedef Dataset Self;
	/**
	 * TODO
	 */
	typedef std::shared_ptr<Self> Pointer;

	/**
	 * TODO
	 */
	typedef itk::Image<float,3> FloatImageType;

	/**
	 * TODO
	 */
    struct index_by_layer{};
    struct index_by_timestamp{};
    struct index_by_layerandtimestamp{};

    typedef Frame::LayerHandlerType LayerHandlerType;

    typedef Frame::TimestampType TimestampType;

    typedef boost::multi_index_container<Frame, boost::multi_index::indexed_by<
			boost::multi_index::ordered_unique<boost::multi_index::tag<index_by_layerandtimestamp>, boost::multi_index::composite_key<Frame, boost::multi_index::member<Frame, Frame::LayerHandlerType, &Frame::m_LayerHandler>,
												boost::multi_index::member<Frame, Frame::TimestampType, &Frame::m_Timestamp> > >,
    		boost::multi_index::ordered_non_unique<boost::multi_index::tag<index_by_layer>, boost::multi_index::member<Frame, Frame::LayerHandlerType, &Frame::m_LayerHandler> >,
			boost::multi_index::ordered_non_unique<boost::multi_index::tag<index_by_timestamp>, boost::multi_index::member<Frame, Frame::TimestampType, &Frame::m_Timestamp> >
            >
        > FrameContainer;

    /**
     * TODO
     */
    typedef typename FrameContainer::template index<index_by_layer>::type LayerIndex;
    typedef typename LayerIndex::iterator LayerIterator;
    typedef std::pair<LayerIterator,LayerIterator> LayerResultSet;
    /**
     * TODO
     */
    typedef typename FrameContainer::template index<index_by_timestamp>::type TimestampIndex;
    typedef typename TimestampIndex::iterator TimestampIterator;
    typedef std::pair<TimestampIterator,TimestampIterator> TimestampResultSet;

    typedef typename FrameContainer::template index<index_by_layerandtimestamp>::type TimestampAndLayerIndex;
    typedef typename TimestampAndLayerIndex::iterator TimestampAndLayerIterator;
    typedef std::pair<TimestampAndLayerIterator,TimestampAndLayerIterator> TimestampAndLayerResultSet;
    /**
     * TODO
     */
    FrameContainer m_Frames;



    /**
     * TODO
     */
    typedef std::vector<Layer::LayerHandlerType> LayerHandlerContainer;
    typedef LayerHandlerContainer::iterator LayerHandlerIterator;
    LayerHandlerContainer m_LayerHandlers;


    /**
     * TODO
     */
    typedef std::map<Layer::LayerHandlerType,Layer> LayerContainer;
    /**
     * TODO
     */
    LayerContainer m_Layers;

    /**
     * TODO
     */
    std::string m_Path;
    /**
     * TODO
     */
    std::string m_ProjectName;
    /**
     * TODO
     */
    std::string m_ProjectDescription;
    /**
     * TODO
     */
    unsigned long m_NumberOfTimestamps;
    /**
     * TODO
     */
    unsigned long m_FirstTimestamp;
    /**
     * TODO
     */
    unsigned long m_LastTimestamp;
    /**
     * TODO
     */
    static Dataset::Pointer  New(){
    	return std::make_shared<Dataset>();
    }
    Dataset(){
    	m_FirstTimestamp=-1;
    	m_LastTimestamp=0;
    }
    /**
     * TODO
     */
    TimestampResultSet GetFramesAtTimestamp(const TimestampType & timestamp){
        return m_Frames.get<index_by_timestamp>().equal_range(timestamp);
    }
    /**
     * TODO
     */
    LayerResultSet GetLayerSequence(const LayerHandlerType & layer){
        return m_Frames.get<index_by_layer>().equal_range(layer);
    }
    /**
     * TODO
     */
	Frame GetFrame(const TimestampType & timestamp,const LayerHandlerType & layer){
        Frame result = (*m_Frames.get<index_by_layerandtimestamp>().find(boost::make_tuple(layer,timestamp)));
        return result;
	}
	/**
	 * TODO
	 */
	Frame NewFrame(const TimestampType & timestamp,const LayerHandlerType & layer){
		Frame result;
		if(HasFrame(timestamp,layer)){
			result = GetFrame(timestamp,layer);
		}else{
			Frame newFrame;
			newFrame.SetTimestamp(timestamp);
			newFrame.SetLayerHandler(layer);
			newFrame.SetFileName(this->GetFileName(timestamp,layer));
			m_Frames.insert(newFrame);
			result= newFrame;
			if(timestamp < m_FirstTimestamp){
				m_FirstTimestamp=timestamp;
			}
			if(timestamp > m_LastTimestamp){
				m_LastTimestamp = timestamp;
			}

			std::cout << "FIRST: " << this->m_FirstTimestamp << "\t LAST: " << this->m_LastTimestamp << std::endl;

			this->Store();
		}


		return result;
	}
	/**
	 * TODO
	 */
	bool HasFrame(const TimestampType & timestamp,const LayerHandlerType & layer){
		return m_Frames.get<index_by_layerandtimestamp>().find(boost::make_tuple(layer,timestamp))!=m_Frames.end();
	}
	/**
	 * TODO
	 */
	bool DeleteFrame(const TimestampType & timestamp,const LayerHandlerType & layer){
		//TODO
		return false;
	}
	/**
	 * TODO
	 */
	Layer GetLayer(const LayerHandlerType & layerHandler){
		return m_Layers[layerHandler];
	}
	/**
	 * TODO
	 */
	void AddLayer(const Layer & layer){
		m_LayerHandlers.push_back(layer.GetLayerID());
		m_Layers[layer.GetLayerID()]=layer;
		this->Store();
	}

	LayerHandlerIterator BeginLayerHandlers(){
		return m_LayerHandlers.begin();
	}
	LayerHandlerIterator EndLayerHandlers(){
		return m_LayerHandlers.end();
	}
	/**
	 * TODO
	 */
	void RemoveLayer(){

	}
	/**
	 * TODO
	 */
	unsigned long GetFirstTimestamp(){
		return m_FirstTimestamp;
	}
	/**
	 * TODO
	 */
	unsigned long GetLastTimestamp(){
		return m_LastTimestamp;
	}
	/**
	 * TODO
	 */
	unsigned long GetNumberOfTimestamps(){
		return m_NumberOfTimestamps;
	}
	/**
	 * TODO
	 */
	void SetPath(const std::string & path){
		m_Path=path;
	}
    /**
	 * Function to set up the name of the project
	 * @param name an std::string with the name of the project
	 */
	inline void SetDatasetName(const std::string & projectName){
		m_ProjectName=projectName;
		this->Store();
	}
	/**
	 * Function to obtain the name of the project
	 * @return an std::string containing the name of the project
	 */
	inline std::string GetDatasetName(){
		return m_ProjectName;
	}
	/**
	 * Function to set up the name of the project
	 * @param name an std::string with the name of the project
	 */
	inline void SetDatasetDescription(const std::string & projectDescription){
		m_ProjectDescription=projectDescription;
		this->Store();
	}
	/**
	 * Function to obtain the name of the project
	 * @return an std::string containing the name of the project
	 */
	inline std::string GetDatasetDescription(){
		return m_ProjectDescription;
	}
	/**
	 * Store the project to disk
	 */
	void Store(){
		std::stringstream fileNameStream;
		fileNameStream << m_Path << "/" << "dataset.json";
		std::string projectConfigFile;
		fileNameStream >> projectConfigFile;

		std::cout << projectConfigFile << std::endl;

		Json::Value root;

		Json::StyledWriter writer;

		root["name"]=this->m_ProjectName;
		root["description"]=this->m_ProjectDescription;


		int k=0;
		for(auto layerHandlerIt=this->BeginLayerHandlers();layerHandlerIt!=this->EndLayerHandlers();++layerHandlerIt){

			ttt::Layer layer = this->GetLayer(*layerHandlerIt);
			root["layers"][k]["name"]=*layerHandlerIt;

			root["layers"][k]["description"]=layer.GetLayerDescription();

			if(layer.GetLayerType()==ttt::Layer::IMAGE){
				root["layers"][k]["type"]=std::string("image");
			}else if(layer.GetLayerType()==ttt::Layer::AJGRAPH){
				root["layers"][k]["type"]=std::string("ajgraph");
			}
			k++;
		}

		k=0;
		for(auto frameIt = m_Frames.begin();frameIt!=m_Frames.end();++frameIt){
			TimestampType timestamp = frameIt->GetTimestamp();
			LayerHandlerType layer = frameIt->GetLayerHandler();
			root["frame"][k]["timestamp"]=(Json::UInt64)timestamp;
			root["frame"][k]["layer"]=layer;
			k++;
		}

		std::string jsoncontent = writer.write(root);
		std::ofstream file(projectConfigFile.c_str(), std::ofstream::out | std::ofstream::trunc);

		file << jsoncontent;

		file.close();
	}
	/**
	 * Load the project from disk
	 */
	bool Load(){
		Json::Reader reader;
		Json::Value root;

		std::stringstream fileNameStream;
		fileNameStream << m_Path << "/" << "dataset.json";
		std::string projectConfigFile;
		fileNameStream >> projectConfigFile;

		std::cout << projectConfigFile << std::endl;

		std::ifstream jsonProjectConfigFile(projectConfigFile.c_str(),std::ifstream::in);
		reader.parse(jsonProjectConfigFile, root);

		this->m_ProjectName = root["name"].asString();
		std::cout << "ProjectName: " << this->m_ProjectName << std::endl;
		this->m_ProjectDescription=root["description"].asString();

		unsigned totalLayers=root["layers"].size();


		for(int layer=0;layer < totalLayers;layer++  ){
			Layer::LayerType type;
			if(root["layers"][layer]["type"].asString()=="image"){
				type = Layer::IMAGE;
			}else if(root["layers"][layer]["type"].asString()=="ajgraph"){
				type = Layer::AJGRAPH;
			}
			Layer::LayerHandlerType layerID = root["layers"][layer]["name"].asString();
			std::string description =root["layers"][layer]["description"].asString();
			Layer newLayer(layerID,description,type);

			m_LayerHandlers.push_back(layerID);
			m_Layers[layerID]=newLayer;
		}


		unsigned totalFrames=root["frame"].size();

		for(int frame = 0; frame < totalFrames;frame++){
			TimestampType timestamp = root["frame"][frame]["timestamp"].asUInt64();
			LayerHandlerType layer = root["frame"][frame]["layer"].asString();

			Frame newFrame;
			newFrame.SetTimestamp(timestamp);
			newFrame.SetLayerHandler(layer);

			std::string fileName;
			std::stringstream fileNameStream;
			fileNameStream << m_Path << "/" << layer << "_T" << timestamp << ".mha";

			fileNameStream >> fileName;

			newFrame.SetFileName(fileName);
			m_Frames.insert(newFrame);
		}

		auto lowest=std::min_element(m_Frames.begin(),m_Frames.end(),[&](const Frame & a, const Frame & b){return a.m_Timestamp < b.m_Timestamp;});
		auto highest=std::max_element(m_Frames.begin(),m_Frames.end(),[&](const Frame & a, const Frame & b){return a.m_Timestamp < b.m_Timestamp;});

		this->m_FirstTimestamp=lowest->m_Timestamp;
		this->m_LastTimestamp=highest->m_Timestamp;

		return true;
	}

private:

	std::string GetFileName(const TimestampType & timestamp,const LayerHandlerType & layer){
		std::string fileName;
		std::stringstream fileNameStream;
		fileNameStream << m_Path << "/" << layer << "_T" << timestamp << ".mha";
		fileNameStream >> fileName;
		return fileName;
	}

#if 0

	typedef AJGraph<AJVertex,AJEdge> AdherensJunctionGraphType;
	typedef TissueDescriptor<AdherensJunctionGraphType,CellGraph<Cell> > TissueType;

	typedef ttt::CellCorrespondences<TissueType> CellCorrespondencesType;
#endif

#if 0
	typedef typename FloatImageType::SpacingType SpacingType;
    typedef typename FloatImageType::RegionType RegionType;
#endif
protected:
#if 0
	std::string m_ProjectName;
	std::string m_ProjectDescription;

    //SizeType m_Size;
	double m_SamplingPeriod;
    std::vector<RegionType> m_Regions;
    std::vector<SpacingType> m_Spacings;

	std::vector<int> m_PlatenessSteps;
	std::vector<double> m_HighestPlatenessScale;
	std::vector<double> m_LowestPlatenessScale;

#if 0
	std::vector<int> m_VertexnessSteps;
	std::vector<double> m_HighestVertexnessScale;
	std::vector<double> m_LowestVertexnessScale;
#endif
	int m_NumberOfFrames;

public:

	Dataset(){
        m_NumberOfFrames=0;
        this->m_SamplingPeriod=0;
        this->m_ProjectName="";
        this->m_ProjectDescription="";

    }
    virtual RegionType GetRegion(unsigned int frame){
        return this->m_Regions[frame];
    }


    virtual void SetRegion(unsigned int frame,const RegionType & region){
        this->m_Regions[frame]=region;
        this->StoreFrameInfo(frame);
    }

    virtual SpacingType GetSpacing(unsigned int frame){
        return this->m_Spacings[frame];

    }

    virtual void SetSpacing(unsigned int frame, const SpacingType & spacing){
        this->m_Spacings[frame]=spacing;
        this->StoreFrameInfo(frame);
    }


	/**
	 * Function to set the sampling period of the time lapse
	 * @param samplingPeriod a positive real value with the time passed between frames in the time-lapse
	 */
	inline void SetSamplingPeriod(double samplingPeriod){
		m_SamplingPeriod=samplingPeriod;
		this->StoreProjectInfo();
	}
	/**
	 * Function to get the sampling period of the time lapse
	 * @return a positive real value with the time passed between frames in the time-lapse
	 */
	inline double GetSamplingPeriod(){
		return m_SamplingPeriod;
	}

	/**
	 * Function to obtain the number of sampling points of the scale space employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness steps value is to be obtained
	 * @return a positive integer for the number of sampling points.
	 */
	inline int GetPlatenessSteps(int frame){
		return m_PlatenessSteps[frame];

	}
	/**
	 * Function to set the number of sampling points of the scale space employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness steps value wants to be set
	 * @param platenessSteps a positive integer for the number of sampling points.
	 */
	inline void SetPlatenessSteps(int frame,int platenessSteps){
		m_PlatenessSteps[frame]=platenessSteps;
		this->StoreFrameInfo(frame);
	}

	/**
	 * Function to obtain the highest scale employed of the interval of scales employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness highest value wants to be obtained
	 * @return a double positive value with the highest scale employed
	 */
	inline double GetHighestPlatenessScale(int frame){
		return m_HighestPlatenessScale[frame];
	}
	/**
	 * Function to set the highest scale employed of the interval of scales employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness highest value wants to be set
	 * @param highestPlatenessStep a double positive value with the highest scale employed for plateness detection
	 */
	inline void SetHighestPlatenessScale(int frame,double highestPlatenessStep){
		m_HighestPlatenessScale[frame]=highestPlatenessStep;
		this->StoreFrameInfo(frame);
	}

	/**
	 * Function to obtain the lowest scale employed of the interval of scales employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness lowest value wants to be set
	 * @return a double positive value with the lowest scale employed
	 */
	inline double GetLowestPlatenessScale(int frame){
		return m_LowestPlatenessScale[frame];
	}
	/**
	 * Function to set the lowest scale employed of the interval of scales employed to detect Adherens Junction Structures in a given frame
	 * @param frame the number of frame whose plateness lowest value wants to be obtained
	 * @param lowestPlatenessStep a double positive value with the lowest scale employed for plateness detection
	 */
	inline void SetLowestPlatenessScale(int frame,double lowestPlatenessStep){
		m_LowestPlatenessScale[frame]=lowestPlatenessStep;
		this->StoreFrameInfo(frame);
	}
#if 0
	/**
	 * Function to obtain the number of sampling points of the scale space employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness steps value is to be obtained
	 * @return a positive integer for the number of sampling points.
	 */
	inline int GetVertexnessSteps(int frame){
		return m_VertexnessSteps[frame];
	}
	/**
	 * Function to set the number of sampling points of the scale space employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness steps value wants to be set
	 * @param vertexnessSteps a positive integer for the number of sampling points.
	 */
	inline void SetVertexnessSteps(int frame,int vertexnessSteps){
		m_VertexnessSteps[frame]=vertexnessSteps;
		this->StoreFrameInfo(frame);
	}

	/**
	 * Function to obtain the highest scale employed of the interval of scales employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness highest value wants to be obtained
	 * @return a double positive value with the highest scale employed
	 */
	inline double GetHighestVertexnessScale(int frame){
		return m_HighestVertexnessScale[frame];
	}
	/**
	 * Function to set the highest scale employed of the interval of scales employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness highest value wants to be set
	 * @param highestVertenessStep a double positive value with the highest scale employed for vertexness detection
	 */
	inline void SetHighestVertexnessScale(int frame,double highestVertexnessStep){
		m_HighestVertexnessScale[frame]=highestVertexnessStep;
		this->StoreFrameInfo(frame);
	}

	/**
	 * Function to obtain the lowest scale employed of the interval of scales employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness lowest value wants to be set
	 * @return a double positive value with the lowest scale employed
	 */
	inline double GetLowestVertexnessScale(int frame){
		return m_LowestVertexnessScale[frame];
	}
	/**
	 * Function to set the lowest scale employed of the interval of scales employed to detect Adherens Junction Vertices in a given frame
	 * @param frame the number of frame whose vertexness lowest value wants to be obtained
	 * @param lowestVertexnessStep a double positive value with the lowest scale employed for vertexness detection
	 */
	inline void SetLowestVertexnessScale(int frame,double lowestVertexnessStep){
		m_LowestVertexnessScale[frame]=lowestVertexnessStep;
		this->StoreFrameInfo(frame);
	}
#endif
	/**
	 * Function to obtain the number of frames contained in the time-lapse stored in the project
	 * @return a positive integer with the number of frames in the project
	 */
	unsigned int GetNumberOfFrames(){
		return m_NumberOfFrames;
	}
	/**
	 * Resizes the project to contain the specified number of frames. Use with caution
	 * @numFrames new number of frames the project must content
	 */
	void SetNumFrames(unsigned int numFrames){
		assert(numFrames>=0);
		this->m_NumberOfFrames=numFrames;
		this->m_Spacings.resize(numFrames);
		this->m_Regions.resize(numFrames);
		this->m_PlatenessSteps.resize(numFrames);
		this->m_HighestPlatenessScale.resize(numFrames);
		this->m_LowestPlatenessScale.resize(numFrames);
#if 0


		m_VertexnessSteps.resize(numFrames);
		m_HighestVertexnessScale.resize(numFrames);
		m_LowestVertexnessScale.resize(numFrames);
#endif

	}


#if 0
    virtual typename TissueDescriptor<AJGraph<AJVertex,AJEdge>,CellGraph<Cell> >::Pointer GetTissueDescriptor(const std::string & name,unsigned int frame)=0;
    virtual void SetTissueDescriptor(const std::string & name,unsigned int frame,const typename TissueDescriptor<AJGraph<AJVertex,AJEdge>,CellGraph<Cell> >::Pointer & tissue)=0;
    virtual bool HasTissue(const std::string & name,unsigned int frame)=0;

    virtual void SetCellCorrespondences(const std::string & name,unsigned int frame0, unsigned int frame1,const typename CellCorrespondencesType::Pointer & correspondences)=0;
    virtual typename CellCorrespondencesType::Pointer GetCellCorrespondences(const std::string & name,unsigned int frame0, unsigned int frame1)=0;
#endif


	/**
	 * Opens the project, calling the backend to load metadata information
	 */
	virtual void Open(){
		LoadProjectInfo();
		this->SetNumFrames(m_NumberOfFrames);
		for(int t=0;t<m_NumberOfFrames;t++){
			LoadFrameInfo(t);
		}

	}

	/**
	 * Closes the project
	 */
	void Close(){
		this->Flush();
	}

	/**
	 * Commits all the pending changes to the storage
	 */
	void Flush(){
		this->StoreProjectInfo();

		for(int i=0;i<this->m_NumberOfFrames;i++){
			this->StoreFrameInfo(i);
		}
	}

	static Dataset<FrameDimensions> * New(){
		return new Dataset<FrameDimensions>;
	}

protected:
	virtual void LoadProjectInfo()=0;
	virtual void StoreProjectInfo()=0;
	virtual void LoadFrameInfo(unsigned int frame)=0;
	virtual void StoreFrameInfo(unsigned int frame)=0;
#endif
};
}


#endif /* TISSUETRACKINGABSTRACTPROJECT_H_ */
