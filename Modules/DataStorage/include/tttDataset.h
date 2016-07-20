/*
 * tttDataset.h
 *
 *  Created on: Jun 7, 2016
 *      Author: morgan
 */

#ifndef IO_TTTDATASET_H_
#define IO_TTTDATASET_H_
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <tttCellGraph.h>
#include <tttCell.h>
#include <tttAJGraph.h>
#include <tttAJVertex.h>
#include <tttAJEdge.h>
#include <tttTissue.h>
#include "tttLayer.h"
#include "tttTemporalContainer.h"
#include <vtk_jsoncpp.h>

namespace ttt{

class Dataset{
public:

	const static unsigned int NumberOfDimensions=4;
	typedef itk::Image<float,NumberOfDimensions> Float3DMovieType;
	typedef itk::Image<unsigned char,NumberOfDimensions> Binary3DMovieType;
	typedef itk::Image<unsigned short,NumberOfDimensions> Integer3DMovieType;

	typedef itk::Image<float,NumberOfDimensions-1> Float3DImageType;
	typedef itk::Image<unsigned char,NumberOfDimensions-1> Binary3DImageType;
	typedef itk::Image<unsigned short,NumberOfDimensions-1> Integer3DImageType;

	typedef itk::Image<float,NumberOfDimensions-1> Float2DMovieType;
	typedef itk::Image<unsigned char,NumberOfDimensions-1> Binary2DMovieType;
	typedef itk::Image<unsigned short,NumberOfDimensions-1> Integer2DMovieType;

	typedef itk::Image<float,NumberOfDimensions-2> Float2DImageType;
	typedef itk::Image<unsigned char,NumberOfDimensions-2> Binary2DImageType;
	typedef itk::Image<unsigned short,NumberOfDimensions-2> Integer2DImageType;


	typedef itk::Image<itk::Vector<float,2>,3 > Registration2DMovieType;


	typedef ttt::Cell CellType;
	typedef ttt::CellGraph<CellType> CellGraphType;

	typedef ttt::AJGraph<ttt::AJVertex<2>,ttt::AJEdge> AJGraphType;
	typedef ttt::Tissue<AJGraphType,CellGraphType> TissueType;


	typedef ttt::TemporalContainer<CellGraphType> CellGraphMovieType;




	std::string m_Path;

	template<class TImage>  using ReaderType = itk::ImageFileReader<TImage>;
	template<class TImage>  using WriterType = itk::ImageFileWriter<TImage>;

	unsigned int m_NumberOfFrames;

	std::string m_Name;
	std::string m_Description;

	/**
	 * TODO
	 */
	typedef Dataset Self;
	typedef std::shared_ptr<Self> Pointer;

    /**
     * TODO
     */
    typedef std::map<ttt::Layer::LayerHandlerType,ttt::Layer> LayerContainer;
    /**
     * TODO
     */
    LayerContainer m_Layers;

    typedef ttt::Layer::LayerHandlerType LayerHandlerType;

    /**
     * TODO
     */
    typedef std::vector<Layer::LayerHandlerType> LayerHandlerContainer;
    typedef LayerHandlerContainer::iterator LayerHandlerIterator;
    LayerHandlerContainer m_LayerHandlers;


    static Pointer New(){
    	return std::make_shared<Dataset>();
    }
public:

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

	void SetPath(const std::string & path){
		m_Path=path;
	}

	void SetDatasetName(const std::string & name){
		m_Name=name;
	}
	void SetDatasetDescription(const std::string & description){
		m_Description=description;
	}

	LayerHandlerIterator BeginLayerHandlers(){
		return m_LayerHandlers.begin();
	}
	LayerHandlerIterator EndLayerHandlers(){
		return m_LayerHandlers.end();
	}

	unsigned int GetNumberOfFrames(){
		return m_NumberOfFrames;
	}


	template<class TReader> typename TReader::Pointer GetSource(const LayerHandlerType & layer){

		typename TReader::Pointer reader = TReader::New();

		std::stringstream fileNameStream;

		fileNameStream << m_Path << "/" << layer << ".mha";
		reader->SetFileName(fileNameStream.str());
		return reader;
	}

	template<class TWriter> typename TWriter::Pointer GetSink(const LayerHandlerType & layer){


		typename TWriter::Pointer writer = TWriter::New();

		std::stringstream fileNameStream;

		fileNameStream << m_Path << "/" << layer << ".mha";
		writer->SetFileName(fileNameStream.str());
		return writer;
	}

#if 0
	bool HasCellSeeds(){
		std::stringstream fileNameStream;
		fileNameStream << m_Path << "/" << "cellseeds.json";

		std::ifstream ifile(fileNameStream.str());
		return ifile.is_open();

	}

	typename ttt::SeedContainer::Pointer GetCellSeeds(){

		ttt::SeedContainer::Pointer seeds = ttt::SeedContainer::New();

		typedef ttt::SeedContainer::SeedType SeedType;

		Json::Reader reader;
		Json::Value root;

		std::stringstream fileNameStream;
		fileNameStream << m_Path << "/" << "cellseeds.json";
		std::string projectConfigFile;
		fileNameStream >> projectConfigFile;

		std::cout << projectConfigFile << std::endl;



		std::ifstream jsonProjectConfigFile(projectConfigFile.c_str(),std::ifstream::in);
		reader.parse(jsonProjectConfigFile, root);

		unsigned numberOfSeeds = root["seeds"].size();

		for(int i=0;i<numberOfSeeds;i++){
			SeedType seed;
			seed[0]=root["seeds"][i]["x"].asUInt();
			seed[1]=root["seeds"][i]["y"].asUInt();
			seed[2]=root["seeds"][i]["t"].asUInt();

			seeds->AddSeed(seed);

		}
		return seeds;

	}
	void  SetCellSeeds(const ttt::SeedContainer::Pointer & seeds){
		Json::Value root;

		Json::StyledWriter writer;


		for(int i=0;i<seeds->GetNumberOfSeeds();i++){
			auto seed = seeds->GetSeed(i);
			root["seeds"][i]["x"]=(Json::UInt64)seed[0];
			root["seeds"][i]["y"]=(Json::UInt64)seed[1];
			root["seeds"][i]["t"]=(Json::UInt64)seed[2];
		}


	}
#endif
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

		this->m_Name = root["name"].asString();
		std::cout << "ProjectName: " << this->m_Name << std::endl;
		this->m_Description=root["description"].asString();

		this->m_NumberOfFrames=root["numberOfFrames"].asUInt();
		unsigned totalLayers=root["layers"].size();

		for(int layer=0;layer < totalLayers;layer++  ){
			Layer::LayerType type;
			if(root["layers"][layer]["type"].asString()=="float3dimage"){
				type = Layer::FLOAT3DIMAGE;
			}else if(root["layers"][layer]["type"].asString()=="integer3dimage"){
				type = Layer::INTEGER3DIMAGE;
			}else if(root["layers"][layer]["type"].asString()=="binary3dimage"){
				type = Layer::BINARY3DIMAGE;
			}else if(root["layers"][layer]["type"].asString()=="binary2dimage"){
				type = Layer::BINARY2DIMAGE;
			}else if(root["layers"][layer]["type"].asString()=="integer2dimage"){
				type = Layer::INTEGER2DIMAGE;
			}else if(root["layers"][layer]["type"].asString()=="float2dimage"){
				type = Layer::FLOAT2DIMAGE;
			}else if(root["layers"][layer]["type"].asString()=="cellgraph"){
				type = Layer::CELLGRAPH;

#if 0
			}else if(root["layers"][layer]["type"].asString()=="mesh"){
				type = Layer::MESH;

			}else if(root["layers"][layer]["type"].asString()=="ajgraph"){
				type = Layer::AJGRAPH;
			}else if(root["layers"][layer]["type"].asString()=="tissue"){
				type = Layer::TISSUE;
#endif
			}

			Layer::LayerHandlerType layerID = root["layers"][layer]["name"].asString();
			std::string description =root["layers"][layer]["description"].asString();
			Layer newLayer(layerID,description,type);

			m_LayerHandlers.push_back(layerID);
			m_Layers[layerID]=newLayer;

		}

		return true;
	}

	void Store(){
		std::stringstream fileNameStream;
		fileNameStream << m_Path << "/" << "dataset.json";
		std::string projectConfigFile;
		fileNameStream >> projectConfigFile;

		std::cout << projectConfigFile << std::endl;

		Json::Value root;

		Json::StyledWriter writer;

		root["name"]=this->m_Name;
		root["description"]=this->m_Description;
		root["numberOfFrames"]=this->m_NumberOfFrames;

		int k=0;
		for(auto layerHandlerIt=this->BeginLayerHandlers();layerHandlerIt!=this->EndLayerHandlers();++layerHandlerIt){

			ttt::Layer layer = this->GetLayer(*layerHandlerIt);
			root["layers"][k]["name"]=*layerHandlerIt;

			root["layers"][k]["description"]=layer.GetLayerDescription();
			switch(layer.GetLayerType()){
			case ttt::Layer::FLOAT3DIMAGE: {
				root["layers"][k]["type"] = std::string("float3dimage");
				break;
			}
			case ttt::Layer::BINARY3DIMAGE: {
				root["layers"][k]["type"] = std::string("binary3dimage");
				break;
			}
			case ttt::Layer::INTEGER3DIMAGE: {
				root["layers"][k]["type"] = std::string("integer3dimage");
				break;
			}
			case ttt::Layer::INTEGER2DIMAGE: {
				root["layers"][k]["type"] = std::string("integer2dimage");
				break;
			}
			case ttt::Layer::BINARY2DIMAGE: {
				root["layers"][k]["type"] = std::string("binary2dimage");
				break;
			}
			case ttt::Layer::FLOAT2DIMAGE: {
				root["layers"][k]["type"] = std::string("float2dimage");
				break;
			}
			case ttt::Layer::CELLGRAPH:{
				root["layers"][k]["type"] = std::string("cellgraph");
				break;
			}
#if 0
			case ttt::Layer::MESH:
			{
				root["layers"][k]["type"]=std::string("mesh");
				break;
			}
			case ttt::Layer::AJGRAPH:
			{
				root["layers"][k]["type"]=std::string("ajgraph");
				break;
			}
			case ttt::Layer::TISSUE:
			{
				root["layers"][k]["type"]=std::string("tissue");
				break;
			}
#endif
			}
			k++;
		}

		std::string jsoncontent = writer.write(root);
		std::ofstream file(projectConfigFile,std::ofstream::out | std::ofstream::trunc);

		file << jsoncontent;

		file.close();

	}
};
}


#endif /* IO_TTTDATASET_H_ */
