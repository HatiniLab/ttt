#ifndef __itkVertexnessMeasurementFunction_h
#define __itkVertexnessMeasurementFunction_h

#include "tttObjectnessMeasurementFunction.h"

namespace ttt {

/**
 * TODO
 */
template<typename TFloat> class VertexnessMeasurementFunction: public ObjectnessMeasurementFunction<TFloat> {
public:

	/**
	 * TODO
	 */
	typedef VertexnessMeasurementFunction Self;
	/**
	 * TODO
	 */
	typedef LightObject Superclass;
	/**
	 * TODO
	 */
	typedef SmartPointer<Self> Pointer;
	/**
	 * TODO
	 */
	typedef SmartPointer<const Self> ConstPointer;

	/** Run-time type information (and related methods) */
	itkTypeMacro(VertexnessMeasurementFunction, ObjectnessMeasurementFunction)
	;

	/**
	 * TODO
	 */
	itkNewMacro(Self)
	;

	/**
	 * TODO
	 */
	inline void SetAlpha(double alpha) {
		m_Alpha = alpha;
	}

	/**
	 * TODO
	 */
	itkGetMacro(Alpha, double)
	;

	/** Set/Get macros for Beta */
	inline void SetBeta(double beta) {
		m_Beta = beta;
	}

	/**
	 * TODO
	 */
	itkGetMacro(Beta, double)
	;

	/** Set/Get macros for Gamma */
	inline void SetGamma(double gamma) {
		m_Gamma = gamma;
	}
	/**
	 * TODO
	 */
	itkGetMacro(Gamma, double)
	;

	/** Set/Get macros for C */
	inline void SetC(double C) {
		m_C = C;
	}
	/**
	 * TODO
	 */
	itkGetMacro(C, double)
	;

	/** Macro to scale the vesselness measure with the
	 largest eigenvalue or not */
	inline void SetScaleVertexnessMeasure(bool scale) {
		m_ScaleVertexnessMeasure = scale;
	}
	/**
	 * TODO
	 */
	itkGetMacro( ScaleVertexnessMeasure, bool )
	;
	/**
	 * TODO
	 */
	itkBooleanMacro(ScaleVertexnessMeasure)
	;

	/**
	 * TODO
	 * @param eigenValue
	 * @return
	 */
	virtual TFloat ComputeObjectProperty(
			const FixedArray<TFloat, 3> & eigenValue) {

				double result = 0;
				// Find the smallest eigenvalue
				double smallest = std::fabs(eigenValue[0]);
				double Lambda1 = eigenValue[0];
				for (unsigned int i = 1; i <= 2; i++) {
					if (std::fabs(eigenValue[i]) < smallest) {
						Lambda1 = eigenValue[i];
						smallest = std::fabs(eigenValue[i]);
					}
				}

				// Find the largest eigenvalue
				double largest = std::fabs(eigenValue[0]);
				double Lambda3 = eigenValue[0];

				for (unsigned int i = 1; i <= 2; i++) {
					if (std::fabs(eigenValue[i] > largest)) {
						Lambda3 = eigenValue[i];
						largest = std::fabs(eigenValue[i]);
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

				assert(std::fabs(Lambda3) >= std::fabs(Lambda2));
				assert(std::fabs(Lambda2) >= std::fabs(Lambda1));
				if (Lambda3 > 0 || Lambda2 > 0 || Lambda1 > 0) {
					return 0;
				} else {

					double S = std::sqrt(
							Lambda1 * Lambda1 + Lambda2 * Lambda2 * Lambda3 * Lambda3);
					double A =  Lambda2 / Lambda3;
					double B = Lambda1 / Lambda2;
					double C = Lambda1 / Lambda3;

					double Lambda3Sqr = std::pow(Lambda3,2);

					double GammaSqr = m_Gamma * m_Gamma;
					double AlphaSqr = m_Alpha * m_Alpha;
					double BetaSqr = m_Beta * m_Beta;
					double term1 = 1 - (std::exp(-1.0 * ((std::pow(S,2))) / (2.0 * GammaSqr)));
					double term2 = 1 - (std::exp(-1.0 * ((std::pow(A,2))) / (2.0 * AlphaSqr)));
					double term3 = 1 - (std::exp(-1.0 * ((std::pow(B,2))) / (2.0 * BetaSqr)));

					double term4 = std::exp(-1.0 * (2.0 * std::pow(m_C,2)) / (Lambda3Sqr));
					double term5 = 1 - (std::exp(-1.0 * ((std::pow(C,2))) / (2.0 * BetaSqr)));

					//			double term1 =  ( 1 - vcl_exp( -1.0 * (S)));
					//			double term2  = ( 1 - vcl_exp( -1.0 * (A)));
					//			double term3  = ( 1 - vcl_exp( -1.0 * (B)));

					double vertexnessMeasure = term1 * term2 * term3 * term4*term5;

					if (m_ScaleVertexnessMeasure) {
						double Lambda3Abs = std::fabs(Lambda3);
						result = Lambda3Abs * vertexnessMeasure;
					} else {
						result = vertexnessMeasure;
					}
					return (result);
				}
	}

protected:
	/**
	 * TODO
	 */
	VertexnessMeasurementFunction() {
		m_Alpha = 0.5;
		m_Beta = 0.5;
		m_Gamma = 8;
		m_C = 10e-6;
		m_ScaleVertexnessMeasure = false;
	}
	/**
	 * TODO
	 */
	virtual ~VertexnessMeasurementFunction() {

	}
	/**
	 * TODO
	 * @param os
	 * @param indent
	 */
	void PrintSelf(std::ostream & os, Indent indent) const {

		os << indent << "Alpha: " << m_Alpha << std::endl;
		os << indent << "Beta:  " << m_Beta << std::endl;
		os << indent << "Gamma: " << m_Gamma << std::endl;

		os << indent << "C: " << m_C << std::endl;
		;
	}

private:
	/**
	 * TODO
	 * @param
	 */
	VertexnessMeasurementFunction(const Self &); //purposely not implemented
	/**
	 * TODO
	 * @param
	 */
	void operator=(const Self &);           //purposely not implemented

	/**
	 * TODO
	 */
	double m_Alpha;
	/**
	 * TODO
	 */
	double m_Beta;
	/**
	 * TODO
	 */
	double m_Gamma;
	/**
	 * TODO
	 */
	double m_C;
	/**
	 * TODO
	 */
	bool m_ScaleVertexnessMeasure;

};
} // end namespace itk

#endif
