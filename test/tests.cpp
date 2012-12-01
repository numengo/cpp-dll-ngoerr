
#include "UnitTest++.h"

#include "ngoerr/NgoError.h"
#include "ngoerr/NgoLogging.h"

#include <fstream>

void logSomeStuff()
{
    try
    {
	    const int count = 3;
        NGOLOG(logDEBUG) << "A loop with " << count << " iterations";
        for (int i = 0; i != count; ++i)
        {
	        NGOLOG(TLogLevel(logDEBUG+i)) << "the counter i = " << i;
        }
    }
    catch(const std::exception& e)
    {
        NGOLOG(logERROR) << e.what();
    }
}

namespace
{
TEST(LogDefault)
{
    NGOLOG(logERROR) << "Log Default";
    logSomeStuff();
    NGOLOG(logINFO) << "FINISH";
    NgoLoggerManager::kill();
}


TEST(LogInfo)
{
    new NgoLoggerFile(stdout,logINFO);
    NGOLOG(logINFO) << "Log only Info";
    logSomeStuff();
    NGOLOG(logINFO) << "FINISH";
    NgoLoggerManager::kill();
}

TEST(LogDebug)
{
    new NgoLoggerFile(stderr,logDEBUG1);
    NGOLOG(logINFO) << "Log only DEBUG1";
    logSomeStuff();
    NGOLOG(logINFO) << "FINISH";
    NgoLoggerManager::kill();
}

TEST(LogException)
{
    try
    {
        throw NgoError("standard error with no scope");
    }
    catch(NgoError & er)
    {
        NgoLogError(er);
    }
    try
    {
        throw NgoErrorThrmPropertyNotAvailable(1,"standard warning with a scope","scope::example");
    }
    catch(NgoError & er)
    {
        NgoLogError(er);
    }
    NgoLoggerManager::kill();
}

TEST(LogBufferedString)
{
    NgoLoggerBufferedString * logger = new NgoLoggerBufferedString(logDEBUG1);
    logSomeStuff();
    std::string msg = logger->getBufferedMessage();
    NgoLoggerManager::kill();

    new NgoLoggerFile(stderr,logDEBUG1);
    NGOLOG(logINFO) << msg;
    NgoLoggerManager::kill();

}

TEST(LogOnce)
{
    NgoLog(logINFO,true).get() << "should be unique";
    NgoLog(logINFO,true).get() << "should be unique";
    NGOLOG(logINFO) << "FINISH";
    NgoLoggerManager::kill();
}

TEST(LogFromCFormattedFunction)
{
    int i = 2;
    NgoLogf(logINFO,"this is %d C formatted strings printed in C style.\n",i);
    std::string s("formatted string");
    NgoLogf(logINFO,"It must be able to take %s",s.c_str());
    NGOLOG(logINFO) << "FINISH";
    NgoLoggerManager::kill();
}

TEST(LogIntoFile)
{
    new NgoLoggerFilename("C:\\test.log", "w+",logDEBUG2);
    NGOLOG(logINFO) << "Log only DEBUG2";
    NgoLoggerManager::get()->flush();
    logSomeStuff();
    NGOLOG(logINFO) << "FINISH";
    NgoLoggerManager::kill();
}

} // end of anonymous namespace
    