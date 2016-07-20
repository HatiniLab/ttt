/*
 * tttPyramidSurfaceMeshSource.hxx
 *
 *  Created on: Jan 27, 2016
 *      Author: morgan
 */

#ifndef TTTPYRAMIDSURFACEMESHSOURCE_HXX_
#define TTTPYRAMIDSURFACEMESHSOURCE_HXX_

/** Generate the data */
template<typename TInputImage,typename TOutputMesh>
ttt::PyramidSurfaceMeshSource<TInputImage,TOutputMesh>
::PyramidSurfaceMeshSource() {

	m_PyramidFilter = PyramidFilterType::New();
	m_SurfaceFittingFilter = SurfaceFittingType::New();
	m_NumberOfLevels=4;
}


template<typename TInputImage,typename TOutputMesh>
ttt::PyramidSurfaceMeshSource<TInputImage,TOutputMesh>
::~PyramidSurfaceMeshSource() {

	m_PyramidFilter = PyramidFilterType::New();
}

/** Generate the data */
template<typename TInputImage,typename TOutputMesh>
void
ttt::PyramidSurfaceMeshSource<TInputImage,TOutputMesh>
::GenerateData() {

	m_PyramidFilter->SetInput(this->GetInput());
	m_PyramidFilter->SetNumberOfLevels(m_NumberOfLevels);
	m_PyramidFilter->Update();

	typename SurfaceFittingType::OutputMeshType::Pointer currentMesh = ITK_NULLPTR;

	for (unsigned int level = 0; level < m_NumberOfLevels; ++level) {
		m_SurfaceFittingFilter = SurfaceFittingType::New();

		m_SurfaceFittingFilter->SetInput(m_PyramidFilter->GetOutput(level));

		if (currentMesh != ITK_NULLPTR) {
			m_SurfaceFittingFilter->SetUseInitialMesh(true);
			m_SurfaceFittingFilter->SetInputMesh(currentMesh);
		}
		m_SurfaceFittingFilter->Update();

		currentMesh = m_SurfaceFittingFilter->GetOutput();
		currentMesh->DisconnectPipeline();
	}
	this->GetOutput()->Graft(currentMesh);
}

/** Generate the data */
template<typename TInputImage,typename TOutputMesh>
void
ttt::PyramidSurfaceMeshSource<TInputImage,TOutputMesh>
::PrintSelf(std::ostream & os, itk::Indent indent) const {

}
#endif /* MODULES_FUNCTIONS_IMAGEFUNCTIONS_SURFACEEXTRACTION_INCLUDE_TTTPYRAMIDSURFACEMESHSOURCE_HXX_ */
