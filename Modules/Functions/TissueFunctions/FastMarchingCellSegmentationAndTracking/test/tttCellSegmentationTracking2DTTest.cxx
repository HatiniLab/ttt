/*
 * tttCellSegmentationTracking2DTTest.cxx
 *
 *  Created on: Jun 29, 2016
 *      Author: morgan
 */

#include "tttCellSegmentationTracking2DT.h"
#include <tttCell.h>
#include <tttCellGraph.h>
#include <tttAJVertex.h>
#include <tttAJEdge.h>
#include <tttAJGraph.h>
#include <tttTissue.h>
#include <itkImage.h>
#include <itkVector.h>
#include <itkImageFileReader.h>
#include <tttCellGraphFileReader.h>
#include <tttTissueFileWriter.h>
int main(int argc,char ** argv){

	typedef ttt::Cell CellType;
	typedef ttt::CellGraph<CellType> CellGraphType;
	typedef ttt::AJVertex<2> AJVertexType;
	typedef ttt::AJEdge AJEdgeType;
	typedef ttt::AJGraph<AJVertexType,AJEdgeType> AJGraphType;
	typedef ttt::Tissue<AJGraphType,CellGraphType> TissueType;
	typedef itk::Image<float,3> FloatImageType;

	typedef itk::Image<itk::Vector<float,2>,3> RegistrationFieldType;
	typedef ttt::CellSegmentationTracking2DT<CellGraphType,FloatImageType,RegistrationFieldType,TissueType> CellSegmentationTrackingFilterType;


	typedef itk::ImageFileReader<FloatImageType> SpeedReaderType;

	SpeedReaderType::Pointer speedReader = SpeedReaderType::New();

	typedef ttt::CellGraphFileReader<CellGraphType> CellGraphFileReaderType;
	typedef ttt::TissueFileWriter<TissueType> TissueFileWriterType;

	TissueFileWriterType::Pointer tissueFileWriter = TissueFileWriterType::New();

	CellGraphFileReaderType::Pointer cellGraphFileReader = CellGraphFileReaderType::New();

	CellSegmentationTrackingFilterType::Pointer segmentationTrackingFilterType = CellSegmentationTrackingFilterType::New();

	segmentationTrackingFilterType->SetInput(cellGraphFileReader->GetOutput());
	segmentationTrackingFilterType->SetSpeedImage(speedReader->GetOutput());

	tissueFileWriter->SetInput(segmentationTrackingFilterType->GetOutput());
	tissueFileWriter->Update();
}
