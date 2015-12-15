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

/** \addtogroup TTTObjectnessMeasurementBase
 *  @{
 */

#ifndef __itkObjectnessMeasurementFunction_h
#define __itkObjectnessMeasurementFunction_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace ttt
{
using namespace itk;
class ObjectnessMeasurementFunction:public Object
{
public:
  /** Standard class typedefs. */
  typedef ObjectnessMeasurementFunction   Self;
  typedef LightObject                Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro(ObjectnessMeasurementFunction, LightObject);

  virtual double ComputeObjectProperty(const FixedArray< double,3> & eigenvalues)=0;

protected:
  ObjectnessMeasurementFunction(){

  }
  ~ObjectnessMeasurementFunction(){

  }
  void PrintSelf(std::ostream & os, Indent indent) const{

  }

private:
  ObjectnessMeasurementFunction(const Self &); //purposely not implemented
  void operator=(const Self &);           //purposely not implemented
};

} // end namespace itk


#endif
/** @}*/
