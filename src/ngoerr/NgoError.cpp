/*******************************************************************************
   FILE DESCRIPTION
*******************************************************************************/
/*!
@file NgoError.cpp
@author Cedric ROMAN - roman@numengo.com
@date December 2008
@brief File containing generic classes  member definitions for exceptions
 */
/*******************************************************************************
   LICENSE
*******************************************************************************
 Copyright (C) 2009 Numengo (admin@numengo.com)

 This document is released under the terms of the numenGo EULA.  You should have received a
 copy of the numenGo EULA along with this file; see  the file LICENSE.TXT. If not, write at
 admin@numengo.com or at NUMENGO, 15 boulevard Vivier Merle, 69003 LYON - FRANCE
 You are not allowed to use, copy, modify or distribute this file unless you  conform to numenGo
 EULA license.
*/



/*******************************************************************************
   DOXYGEN GROUP DEFINION
*******************************************************************************/


/*******************************************************************************
   INCLUDES
*******************************************************************************/
#include <iostream>
#include <string>

#include "ngoerr/NgoError.h"
/*******************************************************************************
   DEFINES / TYPDEFS / ENUMS
*******************************************************************************/

/*******************************************************************************
   GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
   CLASS NgoError DEFINITION
*******************************************************************************/

NgoError::NgoError(std::string desc,std::string scope,std::string ifc,std::string oper)
             :description_(desc), scope_(scope), interfaceName_(ifc), operation_(oper)
{
};

void NgoError::addScopeError(std::string scope)
{
   std::ostringstream oss;
   oss << scope << "->" << (*this).scope_;
   (*this).scope_ = oss.str();
};

void NgoError::addDescription(std::string desc)
{
   std::ostringstream oss;
   oss << (*this).description_<< std::endl << desc;
   (*this).description_ = oss.str();
};

void NgoError::print(
std::ostream& os
)
const
{
   std::string line;
   for (unsigned i=name_.length()+2;i--;)
      line += "*";
   os  << name_ << " :\n" << line;
   line.clear();
   if (!scope_.empty())
   {
      os << "\nScope : "
           << scope_;
   }
   if (!interfaceName_.empty())
   {
      os << "\nInterface : "
           << interfaceName_;
   }
   if (!operation_.empty())
   {
      os << "\nOperation : "
           << operation_;
   }
   if (!description_.empty())
   {
      os << "\nDescription :\n"
           << description_;
   }
}


/*******************************************************************************
   CLASS NgoErrorUnknown INLINE FUNCTIONS
*******************************************************************************/
NgoErrorUnknown::NgoErrorUnknown(std::string desc,std::string scope,std::string ifc,std::string oper)
                    :NgoError(desc,scope,ifc,oper)
{
   code_ = E_UNKNOWN;
   name_ = "Unknown";
};

/*******************************************************************************
   CLASS NgoErrorBoundaries DEFINITION
*******************************************************************************/
NgoErrorBoundaries::NgoErrorBoundaries(double value, double lower_bound, double upper_bound, std::string type)
                       :value_(value),lower_bound_(lower_bound),upper_bound_(upper_bound),type_(type)
{
};


void NgoErrorBoundaries::print(
std::ostream& os
)
const
{
   os << "\tA value ";
   if (type_ != "")
   {
      os << "of type " << type_;
   }
   os << "(= " << value_ << ") is out of bounds [" << lower_bound_ << "," << upper_bound_ << "]\n";
}

/*******************************************************************************
   CLASS NgoErrorData INLINE FUNCTIONS
*******************************************************************************/
NgoErrorData::NgoErrorData(std::string desc,std::string scope,std::string ifc,std::string oper)
                 :NgoError(desc,scope,ifc,oper)
{
   code_ = E_DATA;
   name_ = "Data";
};

/*******************************************************************************
   CLASS NgoErrorImplementation INLINE FUNCTIONS
*******************************************************************************/
NgoErrorImplementation::NgoErrorImplementation(std::string desc,std::string scope,std::string ifc,std::string oper)
                           :NgoError(desc,scope,ifc,oper)
{
   code_ = E_IMPLEMENTATION;
   name_ = "Implementation";
};

/*******************************************************************************
   CLASS NgoErrorComputation INLINE FUNCTIONS
*******************************************************************************/
NgoErrorComputation::NgoErrorComputation(std::string desc,std::string scope,std::string ifc,std::string oper)
                        :NgoError(desc,scope,ifc,oper)
{
   code_ = E_COMPUTATION;
   name_ = "Computation";
};


/*******************************************************************************
   CLASS NgoErrorBadArgument DEFINITION
*******************************************************************************/
NgoErrorBadArgument::NgoErrorBadArgument(int position
                                         ,std::string desc,std::string scope,std::string ifc,std::string oper)
                        :NgoErrorData(desc,scope,ifc,oper),position_(position)
{
   code_ = E_BADARGUMENT;
   name_ = "Bad Argument";
};

void NgoErrorBadArgument::print(
std::ostream& os
)
const
{
   NgoError::print(os);
   if (position_)
   {
      os << "\tArgument with position " << position_ << " is not correct.\n";
   }
}

/*******************************************************************************
   CLASS NgoErrorLicenceError INLINE FUNCTIONS
*******************************************************************************/
NgoErrorLicenceError::NgoErrorLicenceError(std::string desc,std::string scope,std::string ifc,std::string oper)
                         :NgoErrorData(desc,scope,ifc,oper)
{
   code_ = E_LICENCEERROR;
   name_ = "Licence Error";
};

/*******************************************************************************
   CLASS NgoErrorInvalidArgument INLINE FUNCTIONS
*******************************************************************************/
NgoErrorInvalidArgument::NgoErrorInvalidArgument(int position
                                         ,std::string desc,std::string scope,std::string ifc,std::string oper)
                            :NgoErrorBadArgument(position,desc,scope,ifc,oper)
{
   code_ = E_INVALIDARGUMENT;
   name_ = "Invalid Argument";
};

/*******************************************************************************
   CLASS NgoErrorThrmPropertyNotAvailable INLINE FUNCTIONS
*******************************************************************************/
NgoErrorThrmPropertyNotAvailable::NgoErrorThrmPropertyNotAvailable(int position
                                         ,std::string desc,std::string scope,std::string ifc,std::string oper)
                            :NgoErrorBadArgument(position,desc,scope,ifc,oper)
{
   code_ = E_THRMPROPERTYNOTAVAILABLE;
   name_ = "Thermodynamic Property Not Available";
};

/*******************************************************************************
   CLASS NgoErrorOutOfBounds DEFINITION
*******************************************************************************/
NgoErrorOutOfBounds::NgoErrorOutOfBounds(double value, double lower_bound, double upper_bound, std::string type
                                         ,int position
                                         ,std::string desc,std::string scope,std::string ifc,std::string oper)
                        :NgoErrorBadArgument(position,desc,scope,ifc,oper), NgoErrorBoundaries(value,lower_bound,upper_bound,type)
{
   code_ = E_OUTOFBOUNDS;
   name_ = "Out Of Bounds";
};

void NgoErrorOutOfBounds::print(
std::ostream& os
)
const
{
   NgoErrorBadArgument::print(os);
   NgoErrorBoundaries::print(os);
}

/*******************************************************************************
   CLASS NgoErrorSolving INLINE FUNCTIONS
*******************************************************************************/
NgoErrorSolving::NgoErrorSolving(std::string desc,std::string scope,std::string ifc,std::string oper)
                    :NgoErrorComputation(desc,scope,ifc,oper)
{
   code_ = E_SOLVINGERROR;
   name_ = "Error Solving";
};

/*******************************************************************************
   CLASS NgoErrorFailedInitialisation INLINE FUNCTIONS
*******************************************************************************/
NgoErrorFailedInitialisation::NgoErrorFailedInitialisation(std::string desc,std::string scope,std::string ifc,std::string oper)
                                 :NgoErrorComputation(desc,scope,ifc,oper)
{
   code_ = E_FAILEDINITIALISATION;
   name_ = "Failed Initialisation";
};

/*******************************************************************************
   CLASS NgoErrorInvalidOperation INLINE FUNCTIONS
*******************************************************************************/
NgoErrorInvalidOperation::NgoErrorInvalidOperation(std::string desc,std::string scope,std::string ifc,std::string oper)
                             :NgoErrorComputation(desc,scope,ifc,oper)
{
   code_ = E_INVALIDOPERATION;
   name_ = "Invalid Operation";
};

/*******************************************************************************
   CLASS NgoErrorNoImpl INLINE FUNCTIONS
*******************************************************************************/
NgoErrorNoImpl::NgoErrorNoImpl(std::string desc,std::string scope,std::string ifc,std::string oper)
                   :NgoErrorImplementation(desc,scope,ifc,oper)
{
   code_ = E_NOIMPL;
   name_ = "NoImplementation";
};

/*******************************************************************************
   CLASS NgoErrorLimitedImpl INLINE FUNCTIONS
*******************************************************************************/
NgoErrorLimitedImpl::NgoErrorLimitedImpl(std::string desc,std::string scope,std::string ifc,std::string oper)
                        :NgoErrorImplementation(desc,scope,ifc,oper)
{
   code_ = E_LIMITEDIMPL;
   name_ = "Limited Implementation";
};

/*******************************************************************************
   CLASS NgoErrorBadInvOrder INLINE FUNCTIONS
*******************************************************************************/
NgoErrorBadInvOrder::NgoErrorBadInvOrder(std::string requestedOperatation_
                                         ,std::string desc,std::string scope,std::string ifc,std::string oper)
                        :NgoErrorComputation(desc,scope,ifc,oper)
{
   requestedOperatation_ = requestedOperatation_;
   code_ = E_BADINVORDER;
   name_ = "Bad Invocation Order";
};


