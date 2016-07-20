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

/** \addtogroup TTTPlateness
 *  @{
 */
#ifndef __itkPlatenessMeasurementFunction_h
#define __itkPlatenessMeasurementFunction_h

#include "tttObjectnessMeasurementFunction.h"

namespace ttt
{


class PlatenessMeasurementFunction:public ObjectnessMeasurementFunction
{
public:
  /** Standard class typedefs. */
  typedef PlatenessMeasurementFunction   Self;
  typedef LightObject                Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro(PlatenessMeasurementFunction, ObjectnessMeasurementFunction);

  itkNewMacro(Self);
  
  /** Set/Get macros for Alpha */
  inline void SetAlpha(double alpha){
	  m_Alpha=alpha;
  }

  itkGetMacro(Alpha, double);
  

  /** Set/Get macros for Beta */
  inline void SetBeta(double beta){
  	  m_Beta=beta;
    }

  itkGetMacro(Beta, double);

  /** Set/Get macros for Gamma */
  inline void SetGamma(double gamma){
	  m_Gamma=gamma;
  }
  itkGetMacro(Gamma, double);


  /** Set/Get macros for C */
  inline void SetC(double C){
	  m_C=C;
  }

  itkGetMacro(C, double);
  


  /** Macro to scale the vesselness measure with the
      largest eigenvalue or not */
  inline void SetScalePlatenessMeasure(bool scale){
	  m_ScalePlatenessMeasure=scale;
  }
  itkGetMacro( ScalePlatenessMeasure, bool );
  itkBooleanMacro(ScalePlatenessMeasure);


  virtual double ComputeObjectProperty(const FixedArray< double,3> & eigenValue) {
		//

		double result = 0;
		// Find the smallest eigenvalue
		double smallest = vnl_math_abs(eigenValue[0]);
		double Lambda1 = eigenValue[0];
		for (unsigned int i = 1; i <= 2; i++) {
			if (vnl_math_abs(eigenValue[i]) < smallest) {
				Lambda1 = eigenValue[i];
				smallest = vnl_math_abs(eigenValue[i]);
			}
		}

		// Find the largest eigenvalue
		double largest = vnl_math_abs(eigenValue[0]);
		double Lambda3 = eigenValue[0];

		for (unsigned int i = 1; i <= 2; i++) {
			if (vnl_math_abs(eigenValue[i] > largest)) {
				Lambda3 = eigenValue[i];
				largest = vnl_math_abs(eigenValue[i]);
			}
		}

		//  find Lambda2 so that |Lambda1| < |Lambda2| < |Lambda3|
		double Lambda2 = eigenValue[0];

		for (unsigned int i = 0; i <= 2; i++) {
			if (eigenValue[i] != Lambda1 && eigenValue[i] != Lambda3) {
				Lambda2 = eigenValue[i];
				break;
			}
		}


		if (Lambda3 >= 0.0 || vnl_math_abs(Lambda3) < EPSILON) {
			return 0;
		} else {
			return vcl_sqrt(vnl_math_sqr(Lambda1) + vnl_math_sqr(Lambda2) + vnl_math_sqr(Lambda3));
#if 0
			double Lambda1Abs = vnl_math_abs(Lambda1);
			double Lambda2Abs = vnl_math_abs(Lambda2);
			double Lambda3Abs = vnl_math_abs(Lambda3);

			double Lambda1Sqr = vnl_math_sqr(Lambda1);
			double Lambda2Sqr = vnl_math_sqr(Lambda2);
			double Lambda3Sqr = vnl_math_sqr(Lambda3);

			double AlphaSqr = vnl_math_sqr(m_Alpha);
			double BetaSqr = vnl_math_sqr(m_Beta);
			double GammaSqr = vnl_math_sqr(m_Gamma);
			double A = Lambda2Abs / Lambda3Abs;
			double B = vcl_sqrt(vnl_math_abs(Lambda1 * Lambda2)) / (Lambda3Abs);//vessel vs plate vs ball
			double S = vcl_sqrt(Lambda1Sqr + Lambda2Sqr + Lambda3Sqr);

			double vesMeasure_1 = (vcl_exp(
					-0.5 * ((vnl_math_sqr(A)) / (AlphaSqr))));

			double vesMeasure_2 = vcl_exp(
					-0.5 * ((vnl_math_sqr(B)) / (BetaSqr)));

			double vesMeasure_3 = (1
					- vcl_exp(-1.0 * ((vnl_math_sqr(S)) / (2.0 * (GammaSqr)))));

			double vesMeasure_4 = vcl_exp(
					-1.0 * (2.0 * vnl_math_sqr(m_C)) / (Lambda3Sqr));

			double vesselnessMeasure = vesMeasure_1 * vesMeasure_2
					* vesMeasure_3 * vesMeasure_4;

			if (m_ScalePlatenessMeasure) {
				result = Lambda3Abs * vesselnessMeasure;
			}else{
				result=vesselnessMeasure;
			}
			return result;
#endif
		}

	}

protected:
  PlatenessMeasurementFunction(){
  	  m_Alpha = 0.5;

  	  m_Beta  = 0.5;

  	  m_Gamma = 8;
  	  m_C = 10e-6;
  	  m_ScalePlatenessMeasure=false;
    }
  virtual ~PlatenessMeasurementFunction() {

  }

  void PrintSelf(std::ostream & os, Indent indent) const{

	  os << indent << "Alpha: " << m_Alpha << std::endl;
	  os << indent << "Beta:  " << m_Beta  << std::endl;
	  os << indent << "Gamma: " << m_Gamma << std::endl;

	  os << indent << "C: " << m_C << std::endl;;
  }

private:
  PlatenessMeasurementFunction(const Self &); //purposely not implemented
  void operator=(const Self &);           //purposely not implemented

  double                                        m_Alpha;
  double                                        m_Beta;
  double                                        m_Gamma;

  double                                        m_C;

  bool                                          m_ScalePlatenessMeasure;

};
} // end namespace itk

#endif
/** @}*/
