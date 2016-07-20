/*
 * AJGraphFileReader.hxx
 *
 *  Created on: Feb 4, 2016
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHFILEREADER_HXX_
#define MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHFILEREADER_HXX_

#include "tttAJGraphFileReader.h"
#include <vtk_jsoncpp.h>
#include <fstream>
namespace ttt {

template<class TOutputAJGraph>
AJGraphFileReader<TOutputAJGraph>::AJGraphFileReader() {

}

template<class TOutputAJGraph>
AJGraphFileReader<TOutputAJGraph>::~AJGraphFileReader() {

}

template<class TOutputAJGraph>
void AJGraphFileReader<TOutputAJGraph>::GenerateData() {
	Json::Reader reader;
	Json::Value root;

	std::ifstream jsonAJTissueFile(m_FileName);
	reader.parse(jsonAJTissueFile, root);

	int numverticesprimal = root["vertices"].size();
	int numedgesprimal = root["edges"].size();

	for (int i = 0; i < numverticesprimal; i++) {

		auto vertex = TOutputAJGraph::AJVertexType::New();

		typename TOutputAJGraph::AJVertexType::PointType point;

		point[0] = root["vertices"][i]["x"].asDouble();
		point[1] = root["vertices"][i]["y"].asDouble();
		point[2] = root["vertices"][i]["z"].asDouble();
		vertex->SetPosition(point);

		auto vertexHandler = this->GetOutput()->AddAJVertex(vertex);

		assert(vertexHandler == root["vertices"][i]["id"].asUInt64());

	}

	for (int i = 0; i < numedgesprimal; i++) {
		auto src = root["edges"][i]["src"].asUInt64();
		auto tgt = root["edges"][i]["tgt"].asUInt64();
		this->GetOutput()->AddAJEdge(src, tgt);
	}

}

template<class TOutputAJGraph>
void AJGraphFileReader<TOutputAJGraph>::PrintSelf(std::ostream & os, itk::Indent indent) const {

}

}

#endif /* MODULES_DATASTORAGE_INCLUDE_TTTAJGRAPHFILEREADER_HXX_ */
