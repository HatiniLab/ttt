//    This file is part of TTT Tissue Tracker.
//
//    TTT Tissue Tracker is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    TTT Tissue Tracker is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with TTT Tissue Tracker.  If not, see <http://www.gnu.org/licenses/>.

/** \addtogroup TTTAnisotropicDiffusion
 *  @{
 */
#ifndef __itkSymmetricEigenVectorAnalysisImageFilter_h
#define __itkSymmetricEigenVectorAnalysisImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkSymmetricEigenAnalysis.h"


namespace itk
{

// This functor class invokes the computation of Eigen Analysis for
// every pixel. The input pixel type must provide the API for the [][]
// operator, while the output pixel type must provide the API for the
// [] operator. Input pixel matrices should be symmetric.
// 
// The default operation is to order eigen values in ascending order.
// You may also use OrderEigenValuesBy( ) to order eigen values by
// magnitude as is common with use of tensors in vessel extraction.
namespace Functor {  
 
template< typename TInput, typename TOutput, typename TMatrix >
class SymmetricEigenVectorAnalysisFunction
{
public:
  SymmetricEigenVectorAnalysisFunction() {}
  ~SymmetricEigenVectorAnalysisFunction() {}
  typedef SymmetricEigenAnalysis< TInput, TOutput, TMatrix > CalculatorType;
  bool operator!=( const SymmetricEigenVectorAnalysisFunction & ) const
  {
    return false;
  }
  bool operator==( const SymmetricEigenVectorAnalysisFunction & other ) const
  {
    return !(*this != other);
  }

  inline TMatrix operator()( const TInput & x )
    {
    TOutput      eigenValues;
    TMatrix eigenVectorMatrix;
    m_Calculator.ComputeEigenValuesAndVectors( x, eigenValues, eigenVectorMatrix );
    return eigenVectorMatrix;
    }

  /** Method to explicitly set the dimension of the matrix */
  void SetDimension( unsigned int n )
    {
    m_Calculator.SetDimension(n);
    }

  /** Typdedefs to order eigen values. 
   * OrderByValue:      lambda_1 < lambda_2 < ....
   * OrderByMagnitude:  |lambda_1| < |lambda_2| < .....
   * DoNotOrder:        Default order of eigen values obtained after QL method
   */
  typedef enum {
    OrderByValue=1,
    OrderByMagnitude,
    DoNotOrder
  }EigenValueOrderType;
 
  /** Order eigen values. Default is to OrderByValue:  lambda_1 < lambda_2 < ....*/
  void OrderEigenValuesBy( EigenValueOrderType order )
    {
    if( order == OrderByMagnitude )
      {
      m_Calculator.SetOrderEigenMagnitudes( true );
      }
    else if( order == DoNotOrder )
      {
      m_Calculator.SetOrderEigenValues( false );
      }
    }

private:
  CalculatorType m_Calculator;
}; 

}  // end namespace functor


/** \class SymmetricEigenVectorAnalysisImageFilter
 * 
 * \ingroup IntensityImageFilters  Multithreaded  TensorObjects
 *
 */
template <typename  TInputImage, typename  TOutputImage, typename TOutputMatrix>
class ITK_EXPORT SymmetricEigenVectorAnalysisImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputMatrix, 
                        Functor::SymmetricEigenVectorAnalysisFunction< 
                                        typename TInputImage::PixelType,
                                        typename TOutputImage::PixelType,
                                        typename TOutputMatrix::PixelType > >
{
public:
  /** Standard class typedefs. */
  typedef SymmetricEigenVectorAnalysisImageFilter  Self;
  typedef UnaryFunctorImageFilter<TInputImage,TOutputMatrix, 
                                  Functor::SymmetricEigenVectorAnalysisFunction< 
                                        typename TInputImage::PixelType,
                                        typename TOutputImage::PixelType,
                                        typename TOutputMatrix::PixelType > >
                                                                      Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  typedef typename Superclass::OutputImageType    OutputImageType;
  typedef typename TOutputImage::PixelType        OutputPixelType;
  typedef typename TInputImage::PixelType         InputPixelType;
  typedef typename Superclass::FunctorType        FunctorType; 

  /** Typdedefs to order eigen values. 
   * OrderByValue:      lambda_1 < lambda_2 < ....
   * OrderByMagnitude:  |lambda_1| < |lambda_2| < .....
   * DoNotOrder:        Default order of eigen values obtained after QL method
   */
  typedef typename FunctorType::EigenValueOrderType         EigenValueOrderType;
 
  /** Order eigen values. Default is to OrderByValue:  lambda_1 < lambda_2 < ....*/
  void OrderEigenValuesBy( EigenValueOrderType order )
    {
    this->GetFunctor().OrderEigenValuesBy( order );
    }

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Print internal ivars */
  void PrintSelf(std::ostream& os, Indent indent) const
    { this->Superclass::PrintSelf( os, indent ); }
  
  /** Set the dimension of the tensor. (For example the SymmetricSecondRankTensor
   * is a pxp matrix) */
  void SetDimension( unsigned int p )
    {
    this->GetFunctor().SetDimension(p);
    }

protected:
  SymmetricEigenVectorAnalysisImageFilter() {};
  virtual ~SymmetricEigenVectorAnalysisImageFilter() {};

private:
  SymmetricEigenVectorAnalysisImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};


  
} // end namespace itk
  
#endif

/** @}*/
