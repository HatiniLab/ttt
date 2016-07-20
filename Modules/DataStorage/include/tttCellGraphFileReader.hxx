/*
 * tttCellGraphFileReader.hxx
 *
 *  Created on: Jun 28, 2016
 *      Author: morgan
 */

#ifndef MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEREADER_HXX_
#define MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEREADER_HXX_

#include "tttCellGraphFileReader.h"
#include <vtk_jsoncpp.h>
namespace ttt{

template<class TCellGraph> CellGraphFileReader<TCellGraph>::CellGraphFileReader(){

}

template<class TCellGraph> CellGraphFileReader<TCellGraph>::~CellGraphFileReader(){

}


template<class TCellGraph> void CellGraphFileReader<TCellGraph>::GenerateData(){

	Json::Reader reader;
	Json::Value root;

	std::ifstream jsonAJTissueFile(m_FileName);
	reader.parse(jsonAJTissueFile, root);

	unsigned int numverticesdual = root["vertices"].size();
	unsigned int numedgesdual = root["edges"].size();


	for (int i = 0; i < numverticesdual; i++) {
		auto cell = TCellGraph::CellType::New();

		typename TCellGraph::CellType::PointType position;
		position[0] = root["vertices"][i]["x"].asDouble();
		position[1] = root["vertices"][i]["y"].asDouble();
		position[2] = root["vertices"][i]["z"].asDouble();

		cell->SetArea(root["vertices"][i]["area"].asDouble());
		cell->SetPerimeterLength(root["vertices"][i]["perimeterLength"].asDouble());
		cell->SetEccentricity(root["vertices"][i]["eccentricity"].asDouble());

		this->GetOutput()->AddCell(cell);

		int perimeterLength = root["vertices"][i]["perimeter"].size();
		for (int j = 0; j < perimeterLength; j++) {
			cell->AddAJVertexToPerimeter(root["vertices"][i]["perimeter"][j].asUInt64());
		}
	}

	for (int i = 0; i < numedgesdual; i++) {
		typename TCellGraph::CellVertexHandler source = root["edges"][i]["src"].asUInt64();
		typename TCellGraph::CellVertexHandler target = root["edges"][i]["tgt"].asUInt64();
		this->GetOutput()->AddCellEdge(source, target);
	}
}

template<class TCellGraph> void CellGraphFileReader<TCellGraph>::PrintSelf(std::ostream & os, itk::Indent indent) const{

}

}


#endif /* MODULES_DATASTORAGE_INCLUDE_TTTCELLGRAPHFILEREADER_HXX_ */
