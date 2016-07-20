/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __tttVoronoiTesselationImageFilter_hxx
#define __tttVoronoiTesselationImageFilter_hxx

#include "tttVoronoiTesselationImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"
#include <algorithm>

namespace ttt
{
template< typename TClusterImage,typename TInternalType>
VoronoiTesselationImageFilter< TClusterImage,TInternalType>
::VoronoiTesselationImageFilter():
  m_TrialHeap()
{

  this->ProcessObject::SetNumberOfRequiredInputs(0);

  ClusterImageSizeType outputSize;
  outputSize.Fill(16);
  typename LevelSetImageType::IndexType outputIndex;
  outputIndex.Fill(0);

  m_OutputRegion.SetSize(outputSize);
  m_OutputRegion.SetIndex(outputIndex);

  m_OutputOrigin.Fill(0.0);
  m_OutputSpacing.Fill(1.0);
  m_OutputDirection.SetIdentity();

  m_AlivePoints = NULL;
  m_OutsidePoints = NULL;
  m_TrialPoints = NULL;
  m_ProcessedPoints = NULL;

  m_SpeedConstant = 1.0;
  m_InverseSpeed = -1.0;
  m_LabelImage = LabelImageType::New();
  m_LevelSetImage=LevelSetImageType::New();

  m_LargeValue    = static_cast< LevelSetPixelType >( NumericTraits< LevelSetPixelType >::max() / 2.0 );
  m_StoppingValue = static_cast< double >( m_LargeValue );
  m_NormalizationFactor = 1.0;
}

template< typename TClusterImage,typename TInternalType> void
VoronoiTesselationImageFilter< TClusterImage,TInternalType>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Alive points: " << m_AlivePoints.GetPointer() << std::endl;
  os << indent << "Trial points: " << m_TrialPoints.GetPointer() << std::endl;
  os << indent << "Speed constant: " << m_SpeedConstant << std::endl;
  os << indent << "Stopping value: " << m_StoppingValue << std::endl;
  os << indent << "Large Value: "
     << static_cast< typename NumericTraits< LevelSetPixelType >::PrintType >( m_LargeValue )
     << std::endl;
  os << indent << "Normalization Factor: " << m_NormalizationFactor << std::endl;

  os << indent << "OutputRegion: " << m_OutputRegion << std::endl;
  os << indent << "OutputOrigin:  " << m_OutputOrigin << std::endl;
  os << indent << "OutputSpacing: " << m_OutputSpacing << std::endl;
  os << indent << "OutputDirection: " << m_OutputDirection << std::endl;
}

template<typename TClusterImage,class TInternalType>
void
VoronoiTesselationImageFilter<TClusterImage,TInternalType>
::GenerateOutputInformation() {
	// copy output information from input image
	Superclass::GenerateOutputInformation();

	// use user-specified output information

	typename TClusterImage::Pointer output = this->GetOutput();
	output->SetRegions(m_OutputRegion);
	output->SetOrigin(m_OutputOrigin);
	output->SetSpacing(m_OutputSpacing);
	output->SetDirection(m_OutputDirection);
}

template<typename TClusterImage,class TInternalType>
void
VoronoiTesselationImageFilter<TClusterImage,TInternalType>
::EnlargeOutputRequestedRegion(
  DataObject *output)
{
  // enlarge the requested region of the output
  // to the whole data set
	TClusterImage *imgData;

  imgData = dynamic_cast< TClusterImage * >( output );
  if ( imgData )
    {
    imgData->SetRequestedRegionToLargestPossibleRegion();
    }
  else
    {
    // Pointer could not be cast to TLevelSet *
    itkWarningMacro( << "itk::FastMarchingImageFilter"
                     << "::EnlargeOutputRequestedRegion cannot cast "
                     << typeid( output ).name() << " to "
                     << typeid( TClusterImage * ).name() );
    }
}

template< typename TClusterImage,typename TInternalType>
void
VoronoiTesselationImageFilter<TClusterImage,TInternalType>
::Initialize()
{
  // allocate memory for the output buffer
   this->GetOutput()->Allocate();
   this->GetOutput()->FillBuffer(-1);
  // cache some buffered region information

  m_StartIndex = m_OutputRegion.GetIndex();
  m_LastIndex = m_StartIndex + m_OutputRegion.GetSize();

  typename ClusterImageType::OffsetType offset;
  offset.Fill(1);
  m_LastIndex -= offset;

  m_LevelSetImage->CopyInformation(this->GetOutput());
  m_LevelSetImage->SetRegions(this->GetOutput()->GetBufferedRegion());
  m_LevelSetImage->Allocate();
  m_LevelSetImage->FillBuffer(m_LargeValue);

  // allocate memory for the PointTypeImage
  m_LabelImage->CopyInformation(this->GetOutput());
  m_LabelImage->SetBufferedRegion( this->GetOutput()->GetBufferedRegion() );
  m_LabelImage->Allocate();
  m_LabelImage->FillBuffer(FarPoint);


  // process input alive points
  AxisNodeType node;
  NodeIndexType idx;

  int cluster=0;
  // process the input trial points
  if ( m_TrialPoints )
    {
    typename NodeContainer::ConstIterator pointsIter = m_TrialPoints->Begin();
    typename NodeContainer::ConstIterator pointsEnd = m_TrialPoints->End();

    while( pointsIter != pointsEnd )
      {
      // get node from trial points container
      node = pointsIter.Value();
      idx = node.GetIndex();

      // check if node index is within the output level set
      if ( m_OutputRegion.IsInside( idx ) )    {
        // make this an initial trial point
        m_LabelImage->SetPixel(idx, InitialTrialPoint);

        this->GetOutput()->SetPixel(idx,cluster++);

        LevelSetPixelType levelSetPixel = node.GetValue();
        m_LevelSetImage->SetPixel(idx, levelSetPixel);

        m_TrialHeap.push(node);
        }
      ++pointsIter;
      }
    }
}

template<typename TClusterImage,class TInternalType>
void
VoronoiTesselationImageFilter<TClusterImage,TInternalType>
::GenerateData()
{
  if( m_NormalizationFactor < vnl_math::eps )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation(ITK_LOCATION);
    err.SetDescription("Normalization Factor is null or negative");
    throw err;
    }

  this->Initialize();

  // process points on the heap
  AxisNodeType node;
  double       currentValue;
  double       oldProgress = 0;

  this->UpdateProgress(0.0);   // Send first progress event

  // CACHE
  while ( !m_TrialHeap.empty() )
    {
    // get the node with the smallest value
    node = m_TrialHeap.top();
    m_TrialHeap.pop();

    // does this node contain the current value ?
    currentValue = static_cast< double >( m_LevelSetImage->GetPixel( node.GetIndex() ) );

    if ( node.GetValue() == currentValue )
      {
      // is this node already alive ?
      if ( m_LabelImage->GetPixel( node.GetIndex() ) != AlivePoint )
        {
        if ( currentValue > m_StoppingValue )
          {
          this->UpdateProgress(1.0);
          break;
          }

        // set this node as alive
        m_LabelImage->SetPixel(node.GetIndex(), AlivePoint);

        // update its neighbors
        this->UpdateNeighbors(node.GetIndex());

        // Send events every certain number of points.
        const double newProgress = currentValue / m_StoppingValue;
        if ( newProgress - oldProgress > 0.01 )  // update every 1%
          {
          this->UpdateProgress(newProgress);
          oldProgress = newProgress;
          if ( this->GetAbortGenerateData() )
            {
            this->InvokeEvent( AbortEvent() );
            this->ResetPipeline();
            ProcessAborted e(__FILE__, __LINE__);
            e.SetDescription("Process aborted.");
            e.SetLocation(ITK_LOCATION);
            throw e;
            }
          }
        }
      }
    }
}

template<typename TClusterImage,class TInternalType>
void
VoronoiTesselationImageFilter<TClusterImage,TInternalType>
::UpdateNeighbors(
  const IndexType & index)
{

  IndexType neighIndex = index;
  unsigned char label;

  int cluster = this->GetOutput()->GetPixel(index);


  for ( unsigned int j = 0; j < SetDimension; j++ )
    {
    // update left neighbor
    if ( index[j] > m_StartIndex[j] )
      {
      neighIndex[j] = index[j] - 1;
      }

    label = m_LabelImage->GetPixel(neighIndex);

    if ( ( label != AlivePoint ) &&
         ( label != InitialTrialPoint ) &&
         ( label != OutsidePoint ) )
      {
      this->UpdateValue(neighIndex, cluster);
      }

    // update right neighbor
    if ( index[j] < m_LastIndex[j] )
      {
      neighIndex[j] = index[j] + 1;
      }

    label = m_LabelImage->GetPixel(neighIndex);

    if ( ( label != AlivePoint ) &&
         ( label != InitialTrialPoint ) &&
         ( label != OutsidePoint ) )
      {
      this->UpdateValue(neighIndex, cluster);
      }

    //reset neighIndex
    neighIndex[j] = index[j];
    }
}

template<typename TClusterImage,class TInternalType>
double
VoronoiTesselationImageFilter<TClusterImage,TInternalType>
::UpdateValue(
  const IndexType & index,int cluster)
{
  IndexType neighIndex = index;

  LevelSetPixelType neighValue;

  // just to make sure the index is initialized (really cautious)
  AxisNodeType node;
  node.SetIndex( index );

  for ( unsigned int j = 0; j < SetDimension; j++ )
    {
    node.SetValue(m_LargeValue);

    // find smallest valued neighbor in this dimension
    for ( int s = -1; s < 2; s = s + 2 )
      {
      neighIndex[j] = index[j] + s;

      // make sure neighIndex is not outside from the image
      if ( ( neighIndex[j] > m_LastIndex[j] ) ||
           ( neighIndex[j] < m_StartIndex[j] ) )
        {
        continue;
        }

      if ( m_LabelImage->GetPixel( neighIndex ) == AlivePoint )
        {
        neighValue = static_cast< LevelSetPixelType >(m_LevelSetImage->GetPixel(neighIndex) );

        // let's find the minimum value given a direction j
        if ( node.GetValue() > neighValue )
          {
          node.SetValue(neighValue);
          node.SetIndex(neighIndex);
          }
        }
      }

    // put the minimum neighbor onto the heap
    m_NodesUsed[j] = node;
    m_NodesUsed[j].SetAxis(j);

    // reset neighIndex
    neighIndex[j] = index[j];
    }

  // sort the local list
  std::sort(m_NodesUsed, m_NodesUsed + SetDimension);

  // solve quadratic equation
  double solution = static_cast< double >( m_LargeValue );

  double aa( 0.0 );
  double bb( 0.0 );
  double cc( m_InverseSpeed );


  ClusterImageSpacingType spacing = /* this->GetOutput() */ m_OutputSpacing;

  double discrim;

  for ( unsigned int j = 0; j < SetDimension; j++ )
    {
    node = m_NodesUsed[j];
    const double value = static_cast< double >( node.GetValue() );

    if ( solution >= value )
      {
      const int    axis = node.GetAxis();
      // spaceFactor = \frac{1}{spacing[axis]^2}
      const double spaceFactor = vnl_math_sqr(1.0 / spacing[axis]);
      aa += spaceFactor;
      bb += value * spaceFactor;
      cc += vnl_math_sqr(value) * spaceFactor;

      discrim = vnl_math_sqr(bb) - aa * cc;
      if ( discrim < 0.0 )
        {
        // Discriminant of quadratic eqn. is negative
        ExceptionObject err(__FILE__, __LINE__);
        err.SetLocation(ITK_LOCATION);
        err.SetDescription("Discriminant of quadratic equation is negative");
        throw err;
        }

      solution = ( vcl_sqrt(discrim) + bb ) / aa;
      }
    else
      {
      break;
      }
    }

  if ( solution < m_LargeValue )
    {
    // write solution to m_OutputLevelSet
    LevelSetPixelType levelSetPixel = static_cast< LevelSetPixelType >( solution );
    m_LevelSetImage->SetPixel(index, levelSetPixel);


    // insert point into trial heap
    m_LabelImage->SetPixel(index, TrialPoint);
    this->GetOutput()->SetPixel(index,cluster);
    node.SetValue( levelSetPixel );
    node.SetIndex( index );
    m_TrialHeap.push(node);
    }

  return solution;
}
} // namespace itk

#endif
