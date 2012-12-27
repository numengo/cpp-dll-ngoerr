/*******************************************************************************
   FILE DESCRIPTION
*******************************************************************************/
/*!
@file NgoLogging.cpp
@author Cedric ROMAN - roman@numengo.com
@date December 2012
@brief File containing logging system
 */
/*******************************************************************************
   LICENSE
*******************************************************************************
 Copyright (C) 2012 Numengo (admin@numengo.com)

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

#include "ngoerr/NgoLogging.h"
/*******************************************************************************
   DEFINES / TYPDEFS / ENUMS
*******************************************************************************/

/*******************************************************************************
   GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
   CLASS NgoLog DEFINITION
*******************************************************************************/
NgoLog::NgoLog(TLogLevel level,bool unique)
:level_(level),unique_(unique)
{
    os.setf(std::ios::scientific,std::ios::floatfield);
    os.precision(5);
    os << NgoLoggerManager::get()->toString(level) << "\t: "; 
};


NgoLog::~NgoLog()
{
    os << std::endl;
    if (!unique_)
        NgoLoggerManager::get()->addLog(level_,os.str());
    else
        NgoLoggerManager::get()->addUniqueLog(level_,os.str());
}
/*******************************************************************************
   CLASS NgoLogger DEFINITION
*******************************************************************************/
NgoLogger::NgoLogger(TLogLevel reportingLevel)
:reportingLevel_(reportingLevel)
{
    NgoLoggerManager::get()->loggers_.push_back(this);
};

#include <algorithm>

NgoLogger::~NgoLogger()
{
    std::vector<NgoLogger *> & loggers = NgoLoggerManager::get()->loggers_;
    std::vector<NgoLogger *>::iterator it = std::find(loggers.begin(),loggers.end(),this);
    loggers.erase(it);
};
/*******************************************************************************
   CLASS NgoLoggerFile DEFINITION
*******************************************************************************/
NgoLoggerFile::NgoLoggerFile(FILE* pFile,TLogLevel reportingLevel)
:NgoLogger(reportingLevel),pFile_(pFile)
{}

NgoLoggerFile::~NgoLoggerFile()
{
    if (pFile_)
        fclose(pFile_);
}

void NgoLoggerFile::output(const TLogLevel level, std::string & log)
{
    if (level>reportingLevel_)
        return;
    if (pFile_)
        fputs(log.c_str(),pFile_);
}

void NgoLoggerFile::flush()
{
    if (pFile_)
        fflush(pFile_);
}

/*******************************************************************************
   CLASS NgoLoggerFilename DEFINITION
*******************************************************************************/
NgoLoggerFilename::NgoLoggerFilename(std::string filename,std::string openingMode, TLogLevel reportingLevel)
:NgoLoggerFile(0L,reportingLevel),filename_(filename)
{
   pFile_ = fopen(filename.c_str(),openingMode.c_str());
   if (!pFile_)
      throw NgoError("Impossible to create logger file");
   fclose(pFile_);
   pFile_ = 0L;
}

NgoLoggerFilename::~NgoLoggerFilename()
{
    if (pFile_)
        fclose(pFile_);
}

void NgoLoggerFilename::output(const TLogLevel level, std::string & log)
{
   if (level>reportingLevel_)
       return;
   if (!pFile_)
       pFile_ = fopen(filename_.c_str(),"a");
   if (!pFile_)
       throw NgoError("Impossible to open logger file");

   fputs(log.c_str(),pFile_);
}

void NgoLoggerFilename::flush()
{
    if (pFile_)
        fflush(pFile_);
    fclose(pFile_);
    pFile_ = 0L;
}

/*******************************************************************************
   CLASS NgoLoggerBufferedString DEFINITION
*******************************************************************************/
NgoLoggerBufferedString::NgoLoggerBufferedString(TLogLevel reportingLevel)
:NgoLogger(reportingLevel)
{
}

NgoLoggerBufferedString::~NgoLoggerBufferedString()
{
}

void NgoLoggerBufferedString::output(const TLogLevel level, std::string & log)
{
   if (level>reportingLevel_)
       return;
   buffer_ += log;
}

void NgoLoggerBufferedString::flush()
{
    // we should do nothing, as in fact, we want to flush when buffered message is required
}

std::string NgoLoggerBufferedString::getBufferedMessage()
{
    std::string ret = buffer_;
    buffer_.clear();
    return ret;
}

/*******************************************************************************
   CLASS NgoLoggerManager DEFINITION
*******************************************************************************/
NgoLoggerManager * NgoLoggerManager::instance_ = 0L;

NgoLoggerManager::NgoLoggerManager() 
{};

NgoLoggerManager::~NgoLoggerManager()
{
    for (int i=loggers_.size()-1;i>=0;i--)
        delete loggers_[i];
};

std::string NgoLoggerManager::toString(TLogLevel level)
{
	static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4"};
    return buffer[level];
}

TLogLevel NgoLoggerManager::fromString(const std::string& level)
{
    if (level == "DEBUG4")
        return logDEBUG4;
    if (level == "DEBUG3")
        return logDEBUG3;
    if (level == "DEBUG2")
        return logDEBUG2;
    if (level == "DEBUG1")
        return logDEBUG1;
    if (level == "DEBUG")
        return logDEBUG;
    if (level == "INFO")
        return logINFO;
    if (level == "WARNING")
        return logWARNING;
    if (level == "ERROR")
        return logERROR;
    NgoLog(logWARNING).get() << "Unknown logging level '" << level << "'. Using INFO level as default.";
    return logINFO;
}

std::vector<NgoLogger *> NgoLoggerManager::getLoggers()
{
    return loggers_;
}

TLogLevel NgoLoggerManager::reportingLevel()
{
    TLogLevel ret = logERROR;
    for (int i=loggers_.size()-1;i;i--)
        if (loggers_[i]->reportingLevel() > ret) 
            ret = loggers_[i]->reportingLevel();
    return ret;
}

void NgoLoggerManager::addUniqueLog(TLogLevel level, std::string & log)
{
    for (int i=0;i<uniqueLogs_.size();i++)
        if (log==uniqueLogs_[i])
            return;
    addLog(level,log);
    uniqueLogs_.push_back(log);
}

void NgoLoggerManager::addLog(TLogLevel level, std::string & log)
{
    if (loggers_.empty())
        new NgoLoggerFile(stderr);
    for (int i=0;i<loggers_.size();i++)
        loggers_[i]->output(level,log);
}

void NgoLoggerManager::flush()
{
    for (int i=0;i<loggers_.size();i++)
        loggers_[i]->flush();
}
#include <stdio.h>
#include <stdarg.h>
int NgoLogf(TLogLevel level, const char * format, ... )
{
   char buffer[1024];
   va_list args;
   va_start (args, format);
   vsprintf (buffer,format, args);
   va_end (args);

   //std::string str_msg = buffer;
   NGOLOG(level) << buffer;
   return 1;
}

int NgoErrorSeverity_(NgoError & er)
{
   switch( er.getCode() )
   {
   case E_UNKNOWN:
   case E_DATA:
   case E_LICENCEERROR:
   case E_BADARGUMENT:
   case E_INVALIDARGUMENT:
   case E_IMPLEMENTATION:
   case E_NOIMPL:
   case E_LIMITEDIMPL:
   case E_OUTOFRESOURCES:
   case E_NOMEMORY:
   case E_TIMEOUT:
   case E_FAILEDINITIALISATION:
   case E_BADINVORDER:
   case E_INVALIDOPERATION:
   case E_PERSISTENCE:
   case E_ILLEGALACCESS:
   case E_PERSISTENCENOTFOUND:
   case E_PERSISTENCESYSTEMERROR:
   case E_PERSISTENCEOVERFLOW:
      return 2;
      break;
   case E_SOLVINGERROR:
//   case E_BOUNDARIES:
   case E_OUTOFBOUNDS:
   case E_THRMPROPERTYNOTAVAILABLE:
   case E_COMPUTATION:
      return 1;
      break;
   default:
      return 2;
   }
   return 0;
}

std::string NgoErrorName_(NgoError & er)
{
   std::string ret;
   switch( er.getCode() )
   {
   case E_UNKNOWN:
       ret = "Unknown"; break;
   case E_DATA:
       ret = "Data"; break;
   case E_LICENCEERROR:
       ret = "License Error"; break;
   case E_BADARGUMENT:
       ret = "Bad Argument"; break;
   case E_INVALIDARGUMENT:
       ret = "Invalid Argument"; break;
   case E_IMPLEMENTATION:
       ret = "Implementation"; break;
   case E_NOIMPL:
       ret = "Not Implemented"; break;
   case E_LIMITEDIMPL:
       ret = "Limited Implementation"; break;
   case E_OUTOFRESOURCES:
       ret = "Out of Resource"; break;
   case E_NOMEMORY:
       ret = "No Memory"; break;
   case E_TIMEOUT:
       ret = "Time Out"; break;
   case E_FAILEDINITIALISATION:
       ret = "Failed Initialisation"; break;
   case E_BADINVORDER:
       ret = "Bad Invocation Order"; break;
   case E_INVALIDOPERATION:
       ret = "Invalid Operation"; break;
   case E_PERSISTENCE:
       ret = "Persistence"; break;
   case E_ILLEGALACCESS:
       ret = "Illegal Access"; break;
   case E_PERSISTENCENOTFOUND:
       ret = "Persistence Not Found"; break;
   case E_PERSISTENCESYSTEMERROR:
       ret = "Persistence System Error"; break;
   case E_PERSISTENCEOVERFLOW:
       ret = "Persistence Overflow"; break;
   case E_SOLVINGERROR:
       ret = "Solving Error"; break;
   case E_OUTOFBOUNDS:
       ret = "Out of Bounds"; break;
   case E_THRMPROPERTYNOTAVAILABLE:
       ret = "Thermodynamic Property Not Available"; break;
   case E_COMPUTATION:
       ret = "Computation"; break;
      break;
   }
   return ret;
}

void NgoLogError(NgoError & er)
{
    TLogLevel level;
    switch (NgoErrorSeverity_(er))
    {
    case 0:
        level = logINFO;
        break;
    case 1:
        level = logWARNING;
        break;
    case 2:
        level = logERROR;
        break;
    }
    bool unique = false;
    int code = er.getCode();
    if ( (code == E_THRMPROPERTYNOTAVAILABLE)
       ||(code == E_DATA)
       ||(code == E_LICENCEERROR)
       )
       unique = true;
    
    NgoLog log(level,unique);
    log.get() << NgoErrorName_(er) << std::endl;
    log.get() << er.getDescription() << std::endl;
    if ((NgoLoggerManager::get()->reportingLevel() >= logDEBUG)&&(!er.getScope().empty()))
        log.get() << "Scope: " << er.getScope() << std::endl;
    log.get() << "---------";
}