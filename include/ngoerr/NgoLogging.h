#ifndef _NgoLogging_h
#define _NgoLogging_h
/*******************************************************************************
   FILE DESCRIPTION
*******************************************************************************/
/*!
@file NgoLogging.h
@author Cedric ROMAN - roman@numengo.com
@date December 2012
@brief File containing definition of generic classes for exceptions which are related to different
error types that may happen during a calculation. This exceptions and error types are defined globally
 in order to have a uniform error management.
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


#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>

#include "ngoerr/NgoError.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
   DOXYGEN GROUP DEFINION
*******************************************************************************/
/*! @defgroup grp_log Logs
@ingroup grp_err
@brief This section describes the log system
*/



/*! @enum TLogLevel : enumeration of all possible log levels */
/*! @ingroup grp_log */
enum TLogLevel {logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

/*! @brief function to create a log with a formatted string */
/*! @ingroup grp_log */
NGO_ERR_EXPORT int NgoLogf(TLogLevel level, const char * fmt, ... );

#ifdef __cplusplus
} // end extern "C"
#endif // end of ifdef __cplusplus

/*******************************************************************************
   CLASS NgoLog DECLARATION
*******************************************************************************/

/*!
@class NgoLog
@brief class to create a log of certain level
User can access the stream which is automatically redirected to the logger manager to dispatch it to all loggers
It is usually used through the macro NGOLOG : NGOLOG(LogError) << "a log defining an error";
@ingroup grp_log
*/
class NGO_ERR_EXPORT NgoLog
{
public:
    /*! @brief constructor
    @param level log level
    @param unique boolean to indicate if a same identical log should be output or not */
    NgoLog(TLogLevel level = logINFO,bool unique=false);
    /*! @brief destructor */
    virtual ~NgoLog();
    /*! @brief method to return the stream */
    std::ostringstream& get() {return os;};
protected:
    /*! @brief stream */
    std::ostringstream os;
private:
    /*! @brief constructor */
    NgoLog(const NgoLog&);
    /*! @brief operator = */
    NgoLog& operator =(const NgoLog&);
    /*! @brief level of the log */
    TLogLevel level_;
    /*! @brief unicity of the log content */
    bool unique_;
};

/*! this define can be modified to disable all logs of a certain levels on a given build */
#ifndef NGOLOG_MAX_LEVEL
#define NGOLOG_MAX_LEVEL logDEBUG4
#endif

/*******************************************************************************
   CLASS NgoLogger DECLARATION
*******************************************************************************/
/*!
@defgroup grp_loggers Loggers
@ingroup grp_log
@brief This section describes the logger system

@defgroup grp_loggers_avl available loggers
@ingroup grp_loggers
*/

/*
@class NgoLogger
@brief abstract class for all loggers.
Loggers are used to stream the logs to a specific output (console, file, string,...)
@ingroup grp_loggers
*/
class NgoLogger
{
    friend class NgoLoggerManager;
public :
    /*! @constructor */
    /*! @brief : on construction, the logger registers itself to the logger manager */
    /*! @param reportingLevel log level of the logger */
    NgoLogger(TLogLevel reportingLevel=logDEBUG4);
    /*! @brief destructor */
    /*! @brief on destruction, the logger unscribes itself to the logger manager */
    ~NgoLogger();
    /*! @brief method to output a log
    level level of the log to output
    log string containing the log
    */
    virtual void output(const TLogLevel level, std::string & log)=0;
    /*! @brief method to flush the log */
    virtual void flush()=0;
    /*! @brief method to return and access the reporting level */
    TLogLevel& reportingLevel() {return reportingLevel_;};
protected:
    /*! @brief reporting level */
    TLogLevel reportingLevel_;
};

/*! @class NgoLoggerFile
@brief class to log the output to a file or a stream. For a file, it is better to use @ref NgoLoggerFilename
@ingroup grp_loggers_avl
*/
class NGO_ERR_EXPORT NgoLoggerFile: public NgoLogger
{
public:
    /*! @brief constructor */
    /*! the FILE object can be a FILE, or stream like stderr or stdout */
    /*! @param pFile pointer to FILE object to redirect the log */
    /*! @param reportingLevel reporting level */
    NgoLoggerFile(FILE* pFile,TLogLevel reportingLevel=logDEBUG4);
    ~NgoLoggerFile();
    virtual void output(const TLogLevel level, std::string & log);
    virtual void flush();
protected:
    /*! @brief pointer FILE object to redirect the log */
    FILE* pFile_;
};

/*! class NgoLoggerFilename
@param class to log the output to a file defined by a filename. 
The file is closed at each flush and updated each time.
@ingroup grp_loggers_avl
*/
class NGO_ERR_EXPORT NgoLoggerFilename : public NgoLoggerFile
{
public:
    /*! @brief constructor */
    /*! @param filename path of the file to redirect the output */
    /*! @param openingMode opening mode: 'w+' can be used to create a file or discard the content of an existing file, 'a' to append logs to an existing log */
    /*! @param reportingLevel reporting level */
    NgoLoggerFilename(std::string filename,std::string openingMode="w+",TLogLevel reportingLevel=logDEBUG4);
    ~NgoLoggerFilename();
    virtual void output(const TLogLevel level, std::string & log);
    virtual void flush();
private:
    /*! @brief filename_ string to store the filename */
    std::string filename_;
};

/*! class NgoLoggerBufferedString
@param class to log the output to a string which is used as a buffer until it is flushed by the method getBufferedMessage
is closed at each flush and updated each time.
@ingroup grp_loggers_avl
*/
class NGO_ERR_EXPORT NgoLoggerBufferedString : public NgoLogger
{
public:
    /*! @brief constructor */
    /*! @param reportingLevel reporting level */
    NgoLoggerBufferedString(TLogLevel reportingLevel);
    ~NgoLoggerBufferedString();
    virtual void output(const TLogLevel level, std::string & log);
    virtual void flush();
    /*! @brief method to retrieve the buffered message. Once retrieved the buffer is empty */
    std::string getBufferedMessage();
    /*! @brief method to know if buffer is empty or not*/
    bool isBufferEmpty() [return buffer_.empty();)
private:
    /*! @brief string to hold the buffer */
    std::string buffer_;
};

/*******************************************************************************
   CLASS NgoLoggerManager DECLARATION
*******************************************************************************/
/*!
@class NgoLoggerManager
@brief This is the logger manager. It allows to register a logger (stderr, callback function, file, etc...)
It deals with unique message (messages that should only appear once)
It contains some methods that 
It flushes the message in all registered loggers and remove current messages
@ingroup grp_logger
*/
class NGO_ERR_EXPORT NgoLoggerManager
{
    friend class NgoLog;
    friend class NgoLogger;
    /* singleton base methods */
private:
    NgoLoggerManager();
    ~NgoLoggerManager();
    static NgoLoggerManager * instance_;

public:
    static NgoLoggerManager * get() {
        if (0L == instance_)
             instance_ = new NgoLoggerManager();
        return instance_;
    };
    static void kill() {
        if (instance_ != 0L)
        {
            delete instance_;
            instance_ = 0L;
        }
    }

public:
    /*! @brief method to flush all registered loggers */
    void flush();
    /*! @brief method to retrieve a level as a string */
    static std::string toString(TLogLevel level);
    /*! @brief method to retrieve a log level index from its string identifier */
    static TLogLevel fromString(const std::string& level);
    /*! @brief method to retrieve the highest reporting level of all registered identifiers */
    TLogLevel reportingLevel();
    /*! @brief method to access the vector of registered pointers */
    std::vector<NgoLogger *> getLoggers();
private:
    std::vector<NgoLogger *> loggers_;
    std::vector<std::string> uniqueLogs_;
protected:
    /*! @brief this method allows to dispatch a log which is supposed to be unique */
    void addUniqueLog(TLogLevel level, std::string & msg);
    /*! @brief this is the method to dispatch a log to all loggers */
    void addLog(TLogLevel level, std::string & msg);
};

/*! @brief this is the macro to use to create logs easily
The macro will make no overhead for logs above the specified symbol NGOLOG_MAX_LEVEL.
This symbol is defined at compile time
Another test is then made to compare it to all registered reporting levels.
*/
#define NGOLOG(level) \
    if (level > NGOLOG_MAX_LEVEL) ;\
    else if (level > NgoLoggerManager::get()->reportingLevel()) ; \
    else NgoLog(level).get()

/*! @brief method to log the content of an error @ref NgoError to a properly formatted log
The content is adapted to the reporting level of loggers
*/
void NGO_ERR_EXPORT NgoLogError(NgoError & er);


#endif // _NgoLogging_h