/*
 * tttAdvectiveDiffusionImageFilter.h
 *
 *  Created on: May 10, 2016
 *      Author: morgan
 */

#ifndef MODULES_FUNCTIONS_IMAGEFUNCTIONS_ADVECTIONDIFFUSION_INCLUDE_TTTADVECTIVEDIFFUSIONITERATIONIMAGEFILTER_H_
#define MODULES_FUNCTIONS_IMAGEFUNCTIONS_ADVECTIONDIFFUSION_INCLUDE_TTTADVECTIVEDIFFUSIONITERATIONIMAGEFILTER_H_

#include "itkImageToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
namespace ttt{

template<class TInputImage,class TOutputImage> class AdvectiveDiffusionIterationImageFilter : public itk::ImageToImageFilter<TInputImage,TOutputImage>{

public:
	typedef AdvectiveDiffusionIterationImageFilter<TInputImage,TOutputImage> Self;
	typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	typedef TInputImage InputImageType;
	typedef typename TInputImage::SpacingType SpacingType;
	typedef typename TInputImage::RegionType InputImageRegionType;

	typedef TOutputImage OutputImageType;
	typedef typename TOutputImage::RegionType OutputImageRegionType;
	typedef typename TOutputImage::IndexType IndexType;



	itkNewMacro(Self);
	itkTypeMacro(Self,Superclass);


protected:

	AdvectiveDiffusionIterationImageFilter(){
		m_Mu=std::pow(2,-5);
		m_Tau=std::pow(2,-3);
		m_Delta=std::pow(2,-2);
		m_VoxelVolume=0;
	}
	~AdvectiveDiffusionIterationImageFilter(){

	}

	virtual double Update2D(const IndexType & index){
		IndexType north=index;
		north[0]++;

		IndexType northEast=index;
		northEast[0]++;
		northEast[1]++;

		IndexType northWest=index;
		northWest[0]++;
		northWest[1]--;

		IndexType south=index;
		south[0]--;

		IndexType southEast=index;
		southEast[0]--;
		southEast[1]++;

		IndexType southWest=index;
		southWest[0]--;
		southWest[1]--;


		IndexType east=index;
		east[1]++;


		IndexType west=index;
		west[1]--;


		double centerValue,northValue,southValue,westValue,eastValue;
		double northEastValue, northWestValue, southEastValue, southWestValue;

		InputImageRegionType inputRegion = this->GetInput()->GetLargestPossibleRegion();

		centerValue = this->GetInput()->GetPixel(index);

		if(inputRegion.IsInside(north)){
			northValue=this->GetInput()->GetPixel(north);
		}else{
			northValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(northEast)){
			northEastValue=this->GetInput()->GetPixel(northEast);
		}else{
			northEastValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(northWest)){
			northWestValue=this->GetInput()->GetPixel(northWest);
		}else{
			northWestValue=this->GetInput()->GetPixel(index);
		}


		if(inputRegion.IsInside(south)){
			southValue=this->GetInput()->GetPixel(south);
		}else{
			southValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(southEast)){
			southEastValue=this->GetInput()->GetPixel(southEast);
		}else{
			southEastValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(southWest)){
			southWestValue=this->GetInput()->GetPixel(southWest);
		}else{
			southWestValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(east)){
			eastValue=this->GetInput()->GetPixel(east);
		}else{
			eastValue=this->GetInput()->GetPixel(index);
		}


		if(inputRegion.IsInside(west)){
			westValue=this->GetInput()->GetPixel(west);
		}else{
			westValue=this->GetInput()->GetPixel(index);
		}

		double u11 = (centerValue + northValue + eastValue + northEastValue)/4.0;
		double u1_1 = (centerValue + northValue + westValue + northWestValue)/4.0;
		double u_11 = (centerValue +  southValue + eastValue + southEastValue)/4.0;
		double u_1_1 = (centerValue + southValue + westValue + southWestValue)/4.0;


		double du10x=(northValue-centerValue)/this->m_Spacing[0];
		double du10y=(u11-u1_1)/this->m_Spacing[1];

		double du_10x=-(southValue-centerValue)/this->m_Spacing[0];
		double du_10y=(u_11-u_1_1)/this->m_Spacing[1];


		double du01x=(u11 - u_11)/this->m_Spacing[0];
		double du01y=(eastValue - centerValue)/this->m_Spacing[1];


		double du0_1x=(u1_1 - u_1_1)/this->m_Spacing[0];
		double du0_1y=-(westValue - centerValue)/this->m_Spacing[1];;


		double Q10 = std::sqrt(std::pow(du10x,2) + std::pow(du10y,2));
		double Q_10 = std::sqrt(std::pow(du_10x,2) + std::pow(du_10y,2));

		double Q01 = std::sqrt(std::pow(du01x,2) + std::pow(du01y,2) );
		double Q0_1 = std::sqrt(std::pow(du0_1x,2) + std::pow(du0_1y,2) );

		double Q = (Q10+Q_10 + Q01 + Q0_1 )/4.0;


		double v10,v01,v_10,v0_1;

		v10=this->m_Spacing[1]*(this->m_Delta*(northValue-centerValue)/(Q10*this->m_Spacing[0]));
		v_10=this->m_Spacing[1]*(this->m_Delta*(southValue-centerValue)/(Q_10*this->m_Spacing[0]));
		v01=this->m_Spacing[0]*(this->m_Delta*(eastValue-centerValue)/(Q01*this->m_Spacing[1]));
		v0_1=this->m_Spacing[0]*(this->m_Delta*(westValue-centerValue)/(Q0_1*this->m_Spacing[1]));



#if 0
		double D10, D_10, D01, D0_1;

		D10=std::pow(std::min((northValue-centerValue)/m_Spacing[0],0.0),2);
		D_10=std::pow(std::min((southValue-centerValue)/m_Spacing[0],0.0),2);

		D01=std::pow(std::min((eastValue-centerValue)/m_Spacing[1],0.0),2);
		D0_1=std::pow(std::min((westValue-centerValue)/m_Spacing[1],0.0),2);



		double v10, doubl e



		double M10 = std::max(D10,D_10);
		double M01 = std::max(D01,D0_1);







		double Q10 = std::sqrt(std::pow(du10x,2) + std::pow(du10y,2));
		double Q_10 = std::sqrt(std::pow(du_10x,2) + std::pow(du_10y,2));

		double Q01 = std::sqrt(std::pow(du01x,2) + std::pow(du01y,2) );
		double Q0_1 = std::sqrt(std::pow(du0_1x,2) + std::pow(du0_1y,2) );

		double Q = (Q10+Q_10 + Q01 + Q0_1 )/4.0;


		double A10=(this->m_Spacing[1])/(this->m_Spacing[0]*Q10);
		double A_10=(this->m_Spacing[1])/(this->m_Spacing[0]*Q_10);

		double A01=(this->m_Spacing[0])/(this->m_Spacing[1]*Q01);
		double A0_1=(this->m_Spacing[0])/(this->m_Spacing[1]*Q0_1);

		double num = this->m_VoxelVolume*centerValue + this->m_Tau*this->m_Delta*this->m_VoxelVolume*std::sqrt(M10 + M01) + this->m_Tau*this->m_Mu*Q*(A100*northValue + A_100*southValue + A010*eastValue + A0_10*westValue + A001*upValue + A00_1*downValue);

		double den = this->m_VoxelVolume + this->m_Tau*this->m_Mu*Q*(A10+A_10 +A01 + A0_1 );
		return num/den;
#endif
	}

	virtual double ComputeUpdate(const IndexType & index){

		IndexType north=index;
		north[0]++;

		IndexType northEast=index;
		northEast[0]++;
		northEast[1]++;

		IndexType northWest=index;
		northWest[0]++;
		northWest[1]--;

		IndexType south=index;
		south[0]--;

		IndexType southEast=index;
		southEast[0]--;
		southEast[1]++;

		IndexType southWest=index;
		southWest[0]--;
		southWest[1]--;

		IndexType northUp=index;
		northUp[0]++;
		northUp[2]++;

		IndexType northDown=index;
		northDown[0]++;
		northDown[2]--;

		IndexType southUp=index;
		southUp[0]--;
		southUp[2]++;

		IndexType southDown=index;
		southDown[0]--;
		southDown[2]--;


		IndexType east=index;
		east[1]++;

		IndexType eastUp=index;
		eastUp[1]++;
		eastUp[2]++;

		IndexType eastDown=index;
		eastDown[1]++;
		eastDown[2]--;

		IndexType west=index;
		west[1]--;

		IndexType westUp=index;
		westUp[1]--;
		westUp[2]++;

		IndexType westDown=index;
		westDown[1]--;
		westDown[2]--;

		IndexType up=index;
		up[2]++;

		IndexType down=index;
		down[2]--;

		double centerValue,northValue,southValue,westValue,eastValue,upValue,downValue;

		double northEastValue, northWestValue, southEastValue, southWestValue,northUpValue,northDownValue, southUpValue,southDownValue, eastUpValue, eastDownValue, westUpValue,westDownValue;

		InputImageRegionType inputRegion = this->GetInput()->GetLargestPossibleRegion();

		centerValue = this->GetInput()->GetPixel(index);

		if(inputRegion.IsInside(north)){
			northValue=this->GetInput()->GetPixel(north);
		}else{
			northValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(northEast)){
			northEastValue=this->GetInput()->GetPixel(northEast);
		}else{
			northEastValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(northWest)){
			northWestValue=this->GetInput()->GetPixel(northWest);
		}else{
			northWestValue=this->GetInput()->GetPixel(index);
		}


		if(inputRegion.IsInside(south)){
			southValue=this->GetInput()->GetPixel(south);
		}else{
			southValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(southEast)){
			southEastValue=this->GetInput()->GetPixel(southEast);
		}else{
			southEastValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(southWest)){
			southWestValue=this->GetInput()->GetPixel(southWest);
		}else{
			southWestValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(northUp)){
			northUpValue=this->GetInput()->GetPixel(northUp);
		}else{
			northUpValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(northDown)){
			northDownValue=this->GetInput()->GetPixel(northDown);
		}else{
			northDownValue=this->GetInput()->GetPixel(index);
		}


		if(inputRegion.IsInside(southUp)){
			southUpValue=this->GetInput()->GetPixel(southUp);
		}else{
			southUpValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(southDown)){
			southDownValue=this->GetInput()->GetPixel(southDown);
		}else{
			southDownValue=this->GetInput()->GetPixel(index);
		}


		if(inputRegion.IsInside(east)){
			eastValue=this->GetInput()->GetPixel(east);
		}else{
			eastValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(eastUp)){
			eastUpValue=this->GetInput()->GetPixel(eastUp);
		}else{
			eastUpValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(eastDown)){
			eastDownValue=this->GetInput()->GetPixel(eastDown);
		}else{
			eastDownValue=this->GetInput()->GetPixel(index);
		}


		if(inputRegion.IsInside(west)){
			westValue=this->GetInput()->GetPixel(west);
		}else{
			westValue=this->GetInput()->GetPixel(index);
		}


		if(inputRegion.IsInside(westUp)){
			westUpValue=this->GetInput()->GetPixel(westUp);
		}else{
			westUpValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(westDown)){
			westDownValue=this->GetInput()->GetPixel(westDown);
		}else{
			westDownValue=this->GetInput()->GetPixel(index);
		}



		if(inputRegion.IsInside(up)){
			upValue=this->GetInput()->GetPixel(up);
		}else{
			upValue=this->GetInput()->GetPixel(index);
		}

		if(inputRegion.IsInside(down)){
			downValue=this->GetInput()->GetPixel(down);
		}else{
			downValue=this->GetInput()->GetPixel(index);
		}

		double D100, D_100, D010, D0_10, D001, D00_1;

		D100=std::pow(std::min((northValue-centerValue)/m_Spacing[0],0.0),2);
		D_100=std::pow(std::min((southValue-centerValue)/m_Spacing[0],0.0),2);

		D010=std::pow(std::min((eastValue-centerValue)/m_Spacing[1],0.0),2);
		D0_10=std::pow(std::min((westValue-centerValue)/m_Spacing[1],0.0),2);

		D001=std::pow(std::min((upValue-centerValue)/m_Spacing[2],0.0),2);
		D00_1=std::pow(std::min((downValue-centerValue)/m_Spacing[2],0.0),2);

		double M100 = std::max(D100,D_100);
		double M010 = std::max(D010,D0_10);
		double M001 = std::max(D001,D00_1);


		double u110 = (centerValue + northValue + eastValue + northEastValue)/4.0;
		double u1_10 = (centerValue + northValue + westValue + northWestValue)/4.0;
		double u_110 = (centerValue +  southValue + eastValue + southEastValue)/4.0;
		double u_1_10 = (centerValue + southValue + westValue + southWestValue)/4.0;

		double u101 = (centerValue + northValue + upValue + northUpValue)/4.0;
		double u10_1= (centerValue + northValue + downValue + northDownValue)/4.0;
		double u_101= (centerValue + southValue + upValue + southUpValue)/4.0;
		double u_10_1= (centerValue + southValue + downValue + southDownValue)/4.0;

		double u011=(centerValue + eastValue + upValue + eastUpValue)/4.0;
		double u01_1=(centerValue + eastValue + downValue + eastDownValue)/4.0;
		double u0_11=(centerValue + westValue + upValue + westUpValue)/4.0;
		double u0_1_1=(centerValue + westValue + downValue + westDownValue)/4.0;


		double du100x=(northValue-centerValue)/this->m_Spacing[0];
		double du100y=(u110-u1_10)/this->m_Spacing[1];
		double du100z=(u101-u10_1)/this->m_Spacing[2];

		double du_100x=-(southValue-centerValue)/this->m_Spacing[0];
		double du_100y=(u_110-u_1_10)/this->m_Spacing[1];
		double du_100z=(u_101-u_10_1)/this->m_Spacing[2];

		double du010x=(u110 - u_110)/this->m_Spacing[0];
		double du010y=(eastValue - centerValue)/this->m_Spacing[1];
		double du010z=(u011 - u01_1)/this->m_Spacing[2];

		double du0_10x=(u1_10 - u_1_10)/this->m_Spacing[0];
		double du0_10y=-(westValue - centerValue)/this->m_Spacing[1];;
		double du0_10z=(u0_11 - u0_1_1)/this->m_Spacing[2];;

		double du001x=(u101 - u_101)/this->m_Spacing[0];
		double du001y=(u011 - u0_11)/this->m_Spacing[1];
		double du001z=(upValue - centerValue)/this->m_Spacing[2];

		double du00_1x=(u10_1 - u_10_1)/this->m_Spacing[0];
		double du00_1y=(u01_1 - u0_1_1)/this->m_Spacing[1];;
		double du00_1z=-(downValue - centerValue)/this->m_Spacing[2];

		double Q100 = std::sqrt(std::pow(du100x,2) + std::pow(du100y,2) + std::pow(du100z,2));
		double Q_100 = std::sqrt(std::pow(du_100x,2) + std::pow(du_100y,2) + std::pow(du_100z,2));

		double Q010 = std::sqrt(std::pow(du010x,2) + std::pow(du010y,2) + std::pow(du010z,2));
		double Q0_10 = std::sqrt(std::pow(du0_10x,2) + std::pow(du0_10y,2) + std::pow(du0_10z,2));

		double Q001 = std::sqrt(std::pow(du001x,2) + std::pow(du001y,2) + std::pow(du001z,2));
		double Q00_1= std::sqrt(std::pow(du00_1x,2) + std::pow(du00_1y,2) + std::pow(du00_1z,2));

		double Q = (Q100+Q_100 + Q010 + Q0_10 + Q001 + Q00_1)/6.0;


		double A100=(this->m_Spacing[1]*this->m_Spacing[2])/(this->m_Spacing[0]*Q100);
		double A_100=(this->m_Spacing[1]*this->m_Spacing[2])/(this->m_Spacing[0]*Q_100);

		double A010=(this->m_Spacing[0]*this->m_Spacing[2])/(this->m_Spacing[1]*Q010);
		double A0_10=(this->m_Spacing[0]*this->m_Spacing[2])/(this->m_Spacing[1]*Q0_10);

		double A001=(this->m_Spacing[0]*this->m_Spacing[1])/(this->m_Spacing[2]*Q001);
		double A00_1=(this->m_Spacing[0]*this->m_Spacing[1])/(this->m_Spacing[2]*Q00_1);

		double num = this->m_VoxelVolume*centerValue + this->m_Tau*this->m_Delta*this->m_VoxelVolume*std::sqrt(M100 + M010 + M001) + this->m_Tau*this->m_Mu*Q*(A100*northValue + A_100*southValue + A010*eastValue + A0_10*westValue + A001*upValue + A00_1*downValue);

		double den = this->m_VoxelVolume + this->m_Tau*this->m_Mu*Q*(A100+A_100 +A010 + A0_10 + A001+A00_1);
		return num/den;

		//return centerValue + this->m_Delta*std::sqrt(M100 + M010 + M001);
	}
	virtual void BeforeThreadedGenerateData(){
		m_Spacing = this->GetInput()->GetSpacing();
		m_VoxelVolume = m_Spacing[0]*m_Spacing[1]*m_Spacing[2];
	}
	virtual void ThreadedGenerateData (const OutputImageRegionType &outputRegionForThread, itk::ThreadIdType threadId){
		typedef itk::ImageRegionIteratorWithIndex<OutputImageType> IteratorType;

		IteratorType iterator(this->GetOutput(),outputRegionForThread);
		while(!iterator.IsAtEnd()){

			IndexType index = iterator.GetIndex();

			iterator.Set(this->ComputeUpdate(index));

			++iterator;
		}
	}


private:

	SpacingType m_Spacing;
	double m_VoxelVolume;

	double m_Mu;
	double m_Tau;
	double m_Delta;


};
}


#endif /* MODULES_FUNCTIONS_IMAGEFUNCTIONS_ADVECTIONDIFFUSION_INCLUDE_TTTADVECTIVEDIFFUSIONITERATIONIMAGEFILTER_H_ */
