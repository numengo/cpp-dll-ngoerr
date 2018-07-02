#ifndef _NgoError_h
#define _NgoError_h
/*******************************************************************************
   FILE DESCRIPTION
*******************************************************************************/
/*!
@file NgoError.h
@author Cedric ROMAN - roman@numengo.com
@date December 2008
@brief File containing definition of generic classes for exceptions which are related to different
error types that may happen during a calculation. This exceptions and error types are defined globally
 in order to have a uniform error management.
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
   REVISIONS
*******************************************************************************/

/*******************************************************************************
   DOXYGEN GROUP DEFINION
*******************************************************************************/
/*! @defgroup grp_err Exceptions
@brief This section describes the class, its @ref grp_eos_ecs_comp "components",
and the current @ref grp_eos_avl "available descriptions".

@defgroup grp_err_avl   Available Descriptions
@ingroup grp_err
@brief This section escribes the derivated classes
*/

/*******************************************************************************
   INCLUDES
*******************************************************************************/
#include <iostream>
#include <sstream>
#include <string>
#include <limits>

/*******************************************************************************
   DEFINES / TYPDEFS / ENUMS
*******************************************************************************/
#ifdef NGOERR_USE_DYN
	#ifdef  _MSC_VER
		#ifndef NGOERR_EXPORT
			#ifdef NGOERR_MAKE_DLL
				#define NGOERR_EXPORT __declspec( dllexport )
			#else
				#define NGOERR_EXPORT __declspec( dllimport )
			#endif
		#endif
		#pragma warning( disable: 4251 )
		#pragma warning( disable: 4275 )
		#pragma warning( disable: 4996 )
		#pragma warning( disable: 4661 )
		#pragma warning( disable: 4267 )
		#pragma warning( disable: 4018 )
	#else
		#ifndef NGOERR_EXPORT
			#ifdef NGOERR_MAKE_DLL
				#define NGOERR_EXPORT
			#else
				#define NGOERR_EXPORT
			#endif
		#endif
	#endif
#else
	#define NGOERR_EXPORT
#endif

const double UNDEFERR = std::numeric_limits<double>::quiet_NaN();

/*! @enum e_NgoErrorCode : an enumeration type of all possible errors */
/*! @ingroup grp_err */
typedef enum e_NgoErrorCode
{
E_OK=0,                 /*!< No error- this exception can never be thrown */
E_UNKNOWN,              /*!< Error to be raised when other error(s), specified by the operation, do not suit. */
E_DATA,                 /*!< Errors hierarchy related to any data */
E_LICENCEERROR,         /*!< Operation can not be completed because the licence agreement is not respected */
E_BADARGUMENT,          /*!< An argument value of the operation is not correct. */
E_INVALIDARGUMENT,      /*!< An invalid argument value was passed*/
E_OUTOFBOUNDS,          /*!< An argument value is outside of the bounds. */
E_IMPLEMENTATION,       /*!< Errors hierarchy related to the current implementation */
E_NOIMPL,               /*!< The operation exists but it is not supported by the current implementation */
E_LIMITEDIMPL,          /*!< The limit of the implementation has been violated */
E_COMPUTATION,          /*!< Errors hierarchy related to the calculation. */
E_OUTOFRESOURCES,       /*!< The physical resources necessary to the execution of the operation are out of limits */
E_NOMEMORY,             /*!< The physical memory necessary to the execution of the operation is out of limit. */
E_TIMEOUT,              /*!< The time-out criterion is reached. */
E_FAILEDINITIALISATION, /*!< The pre-requisites are not valid. The necessary initialisation has not been performed or has failed. */
E_SOLVINGERROR,         /*!< A numerical algorithm fails for any reasons. */
E_BADINVORDER,          /*!< The necessary pre-requisite operation has not been called prior to the operation request */
E_INVALIDOPERATION,     /*!< This operation is not valid in the current context. */
E_PERSISTENCE,          /*!< Errors hierarchy related to the persistence. */
E_ILLEGALACCESS,        /*!< The access to something within the persistence system is not authorised */
E_PERSISTENCENOTFOUND,  /*!< The requested object, table, or something else within the persistence system was not found */
E_PERSISTENCESYSTEMERROR,/*!< There is a severe error within the persistence system.*/
E_PERSISTENCEOVERFLOW,  /*!< There is an overflow of internal persistence system. */
E_THRMPROPERTYNOTAVAILABLE  /*!< A requested thermodynamic property is not available in stored data. */
} e_NgoErrorCode;

/*******************************************************************************
   GLOBAL VARIABLES
*******************************************************************************/


/*******************************************************************************
   CLASS NgoError DECLARATION
*******************************************************************************/
/*!
@class NgoError
@brief The base class of the errors hierarchy. This is an abstract class. No real error can be raised from this class.
@ingroup grp_err
*/
class NGOERR_EXPORT NgoError
{
public :
   /*! @brief Constructor */
   /*! @param desc : input string defining error description */
   /*! @param scope : input string defining scope of the error. The list of packages where the error occurs separated by ':' */
   /*! @param ifc : input string defining the interface where the error is thrown. */
   /*! @param oper : input string defining the operation where the error is thrown */
   NgoError(
      std::string desc  ="",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

      /*! @brief Destructor */
   virtual ~NgoError(){};

   /*! @brief Add scope of error */
   /*! @param scope input function scope that will be add to the current scope of error */
   void addScopeError(
      std::string scope =""
      );
   /*! @brief Add a description */
   /*! @param scope input function scope that will be add to the current scope of error */
   void addDescription(
      std::string desc =""
      );

   /*! @brief Getter of code error */
   /*! @return NgoError code */
   int getCode() { return code_;};

   /*! @brief Function to format NgoError print output */
   virtual void print(std::ostream& os) const {
   std::string line;
   for (unsigned i=name_.length()+2;i--;)
      line += "*";
   os  << name_ << " :\n" << line;
   line.clear();
   if (!scope_.empty()) {
      os << "\nScope : "
           << scope_;
   }
   if (!interfaceName_.empty()) {
      os << "\nInterface : "
           << interfaceName_;
   }
   if (!operation_.empty()) {
      os << "\nOperation : "
           << operation_;
   }
   if (!description_.empty()) {
      os << "\nDescription :\n"
           << description_;
   }
};

   /*! @brief Function to get error description */
   std::string getDescription() const {return description_;};

   /*! @brief Function to get error description */
   std::string getScope() const {return scope_;};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};

protected :
   /*! @brief A short description of the error. */
   std::string name_;
   /*! @brief Code to designate the subcategory of the error. @sa e_NgoErrorCode */
   e_NgoErrorCode code_;

   /*! @brief The description of the error. */
   std::string description_;
   /*! @brief The scope of the error. The list of packages where the error occurs separated by ':' */
   std::string scope_;
   /*! @brief The name of the interface where the error is thrown. */
   std::string interfaceName_;
   /*! @brief The name of the operation where the error is thrown*/
   std::string operation_;
   /*! @brief A string giving more information */
   std::string moreInfo_;
};

inline std::ostream& operator << (std::ostream& os, const NgoError& E)
{
    E.print(os);
    return os;
}

/*******************************************************************************
   CLASS NgoErrorUnknown DECLARATION
*******************************************************************************/
/*!
@class NgoErrorUnknown
@ingroup grp_err_avl
@brief The error to be raised when other error(s), specified by the operation, do not suit.
*/
class NGOERR_EXPORT NgoErrorUnknown : public NgoError
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoError::NgoError */
   NgoErrorUnknown(
      std::string desc  ="Unknown error",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorUnknown(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};

/*******************************************************************************
   CLASS NgoErrorBoundaries DECLARATION
*******************************************************************************/
/*!
@class NgoErrorBoundaries
@ingroup grp_err_avl
@brief A "utility" class which factorises a state which describes the value, its type and its boundaries
*/
class NGOERR_EXPORT NgoErrorBoundaries
{
public :
   /*! @brief Constructor */
   /*! @param value : input value which has led to error */
   /*! @param lower_bound : input value of the lower bound. */
   /*! @param upper_bound : input value of the upper bound. */
   /*! @param type : input string defining the type/nature of the value.(default is empty string) */
   NgoErrorBoundaries(
      double value=UNDEFERR,
      double lower_bound=UNDEFERR,
      double upper_bound=UNDEFERR,
      std::string type = ""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorBoundaries(){};

   /*! @copydoc NgoError::print */
   virtual void print(std::ostream& os) const {
   os << "\tA value ";
   if (type_ != "") {
      os << "of type " << type_;
   }
   os << "(= " << value_ << ") is out of bounds [" << lower_bound_ << "," << upper_bound_ << "]\n";
};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};

protected :
   /*! @brief The current value which has led to an error. */
   double value_;
   /*! @brief The value of the lower bound. */
   double lower_bound_;
   /*! @brief The value of the upper bound. */
   double upper_bound_;
   /*! @brief The type/nature of the value. */
   std::string type_;
};

/*******************************************************************************
   CLASS NgoErrorData DECLARATION
*******************************************************************************/
/*!
@class NgoErrorData
@ingroup grp_err_avl
@brief The base class of the errors hierarchy related to any data. The data are the arguments of operations, the parameters.
*/
class NGOERR_EXPORT NgoErrorData : public NgoError
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoError::NgoError */
   NgoErrorData(
      std::string desc  ="",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorData(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};


/*******************************************************************************
   CLASS NgoErrorImplementation DECLARATION
*******************************************************************************/
/*!
@class NgoErrorImplementation
@ingroup grp_err_avl
@brief The base class of the errors hierarchy related to the current implementation.
*/
class NGOERR_EXPORT NgoErrorImplementation : public NgoError
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoError::NgoError */
   NgoErrorImplementation(
      std::string desc  ="",
      std::string scope ="An error occured in implementation",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorImplementation(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};

/*******************************************************************************
   CLASS NgoErrorComputation DECLARATION
*******************************************************************************/
/*!
@class NgoErrorComputation
@ingroup grp_err_avl
@brief The base class of the errors hierarchy related to the calculation.
*/
class NGOERR_EXPORT NgoErrorComputation : public NgoError
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoError::NgoError */
   NgoErrorComputation(
      std::string desc  ="",
      std::string scope ="An error occured in computation",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorComputation(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};


/*******************************************************************************
   CLASS NgoErrorBadArgument DECLARATION
*******************************************************************************/
/*!
@class NgoErrorBadArgument
@ingroup grp_err_avl
@brief An argument value of the operation is not correct
*/
class NGOERR_EXPORT NgoErrorBadArgument : public NgoErrorData
{
public :
   /*! @brief Constructor */
   /*! @param position : argument position in opertation */
   /*! @copydetails NgoErrorData::NgoErrorData */
   NgoErrorBadArgument(
      int position = 1,
      std::string desc  ="An argument value of the operation is not correct",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorBadArgument(){};

   /*! @copydoc NgoError::print */
   virtual void print(std::ostream& os) const {
   NgoError::print(os);
   if (position_)
   {
      os << "\tArgument with position " << position_ << " is not correct.\n";
   }
};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
private :
   int position_;
};

/*******************************************************************************
   CLASS NgoErrorLicenceError DECLARATION
*******************************************************************************/
/*!
@class NgoErrorLicenceError
@ingroup grp_err_avl
@brief An operation can not be completed because the licence agreement is not respected.
*/
class NGOERR_EXPORT NgoErrorLicenceError : public NgoErrorData
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoError::NgoError */
   NgoErrorLicenceError(
      std::string desc  ="An operation can not be completed because the licence agreement is not respected.",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorLicenceError(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};


/*******************************************************************************
   CLASS NgoErrorInvalidArgument DECLARATION
*******************************************************************************/
/*!
@class NgoErrorInvalidArgument
@ingroup grp_err_avl
@brief An invalid argument value was passed
*/
class NGOERR_EXPORT NgoErrorInvalidArgument : public NgoErrorBadArgument
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoErrorBadArgument::NgoErrorBadArgument */
   NgoErrorInvalidArgument(
      int position = 1,
      std::string desc  ="An invalid argument value was passed",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorInvalidArgument(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};

/*******************************************************************************
   CLASS NgoErrorThrmPropertyNotAvailable DECLARATION
*******************************************************************************/
/*!
@class NgoErrorThrmPropertyNotAvailable
@ingroup grp_err_avl
@brief A Physical Property is not available
*/
class NGOERR_EXPORT NgoErrorThrmPropertyNotAvailable : public NgoErrorBadArgument
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoErrorBadArgument::NgoErrorBadArgument */
   NgoErrorThrmPropertyNotAvailable(
      int position = 1,
      std::string desc  ="A Physical Property is not available",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorThrmPropertyNotAvailable(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};

/*******************************************************************************
   CLASS NgoErrorOutOfBounds DECLARATION
*******************************************************************************/
/*!
@class NgoErrorOutOfBounds
@ingroup grp_err_avl
@brief An argument value is outside of the bounds.
*/
class NGOERR_EXPORT NgoErrorOutOfBounds : public NgoErrorBadArgument, public NgoErrorBoundaries
{
public :

   /*! @brief Constructor */
   /*! @copydetails NgoErrorBoundaries::NgoErrorBoundaries */
   /*! @copydetails NgoErrorBadArgument::NgoErrorBadArgument */
   NgoErrorOutOfBounds(
      double value=UNDEFERR,
      double lower_bound=UNDEFERR,
      double upper_bound=UNDEFERR,
      std::string type = "",
      int position = 0,
      std::string desc  ="An argument value of the operation is out of bounds",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorOutOfBounds(){};

   /*! @copydoc NgoError::print */
   virtual void print(std::ostream& os) const {
   NgoErrorBadArgument::print(os);
   NgoErrorBoundaries::print(os);
};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};

/*******************************************************************************
   CLASS NgoErrorSolving DECLARATION
*******************************************************************************/
/*!
@class NgoErrorSolving
@ingroup grp_err_avl
@brief A numerical algorithm fails for any reasons.
*/
class NGOERR_EXPORT NgoErrorSolving : public NgoErrorComputation
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoError::NgoError */
   NgoErrorSolving(
      std::string desc  ="A numerical algorithm has failed",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorSolving(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};

/*******************************************************************************
   CLASS NgoErrorFailedInitialisation DECLARATION
*******************************************************************************/
/*!
@class NgoErrorFailedInitialisation
@ingroup grp_err_avl
@brief The pre-requisites are not valid. The necessary initialisation has not been performed or has failed.
*/
class NGOERR_EXPORT NgoErrorFailedInitialisation : public NgoErrorComputation
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoError::NgoError */
   NgoErrorFailedInitialisation(
      std::string desc  ="The pre-requisites are not valid. The necessary initialisation has not been performed or has failed.",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorFailedInitialisation(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};

/*******************************************************************************
   CLASS NgoErrorInvalidOperation DECLARATION
*******************************************************************************/
/*!
@class NgoErrorInvalidOperation
@ingroup grp_err_avl
@brief This operation is not valid in the current context
*/
class NGOERR_EXPORT NgoErrorInvalidOperation : public NgoErrorComputation
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoError::NgoError */
   NgoErrorInvalidOperation(
      std::string desc  ="This operation is not valid in the current context",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorInvalidOperation(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};

/*******************************************************************************
   CLASS NgoErrorNoImpl DECLARATION
*******************************************************************************/
/*!
@class NgoErrorNoImpl
@ingroup grp_err_avl
@brief The operation is \93not\94 implemented. The operation exists but it is not supported by the current implementation.
*/
class NGOERR_EXPORT NgoErrorNoImpl : public NgoErrorImplementation
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoError::NgoError */
   NgoErrorNoImpl(
      std::string desc  ="The operation is 'not' implemented. The operation exists but it is not supported by the current implementation.",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorNoImpl(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};

/*******************************************************************************
   CLASS NgoErrorLimitedImpl DECLARATION
*******************************************************************************/
/*!
@class NgoErrorLimitedImpl
@ingroup grp_err_avl
@brief The limit of the implementation has been violated. An operation may be partially implemented
*/
class NGOERR_EXPORT NgoErrorLimitedImpl : public NgoErrorImplementation
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoError::NgoError */
   NgoErrorLimitedImpl(
      std::string desc  ="The limit of the implementation has been violated. An operation may be partially implemented",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorLimitedImpl(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};
};

/*******************************************************************************
   CLASS NgoErrorBadInvOrder DECLARATION
*******************************************************************************/
/*!
@class NgoErrorBadInvOrder
@ingroup grp_err_avl
@brief An invalid argument value was passed
*/
class NGOERR_EXPORT NgoErrorBadInvOrder : public NgoErrorComputation
{
public :
   /*! @brief Constructor */
   /*! @copydetails NgoError::NgoError */
   /*! @param requested_operation input string detailing which operation is required */
   NgoErrorBadInvOrder(
      std::string requested_operation ="",
      std::string desc  ="An invalid argument value was passed",
      std::string scope ="",
      std::string ifc   ="",
      std::string oper  =""
      );

   /*! @brief Destructor */
   virtual ~NgoErrorBadInvOrder(){};

   /*! @brief virtual method to raise a polymorphic exception */
   virtual void raise() { throw *this;};

protected:
   std::string requestedOperatation_;
};

#endif

