#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>
#include <string>
#include <vector>
#include <mutex>
#include <stdio.h>

#include "timestamp.h"


/**********************************************************************************************************************************
// Template
**********************************************************************************************************************************/

enum TLogLevel {logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

/*********************************
// Class
*********************************/
template <typename T>
class Log
{
public:
    Log();
    virtual ~Log();
    std::ostringstream& GetNoStamp(TLogLevel level = logINFO);
	std::ostringstream& GetTimeStamped(TLogLevel level = logINFO);
	std::ostringstream& GetTimeStamp();

public:
    static TLogLevel& ReportingLevel();
    static std::string ToString(TLogLevel level);
    static TLogLevel FromString(const std::string& level);

	// String stream
protected:
    std::ostringstream os;
	std::string timestamp;

private:
    Log(const Log&);
    Log& operator =(const Log&);
};

/*********************************
// Constructor
*********************************/
template <typename T>
Log<T>::Log()
{
	timestamp = NowTime();
}

/*********************************
// Get log unstamped
*********************************/
template <typename T>
std::ostringstream& Log<T>::GetNoStamp(TLogLevel level)
{
    os << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');
    return os;
}

/*********************************
// Get log stamped
*********************************/
template <typename T>
std::ostringstream& Log<T>::GetTimeStamped(TLogLevel level)
{
	os << "- " << timestamp;
	os << " " << ToString(level) << ": ";
	os << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');
	return os;
}

/*********************************
// Get log stamped
*********************************/
template <typename T>
std::ostringstream& Log<T>::GetTimeStamp()
{
	return timestamp;
}

/*********************************
// Destructor
*********************************/
template <typename T>
Log<T>::~Log()
{
    os << std::endl;
    T::Output(os.str());
}

/*********************************
// Reporting level
*********************************/
template <typename T>
TLogLevel& Log<T>::ReportingLevel()
{
    static TLogLevel reportingLevel = logDEBUG4;
    return reportingLevel;
}

/*********************************
// Convert level to string
*********************************/
template <typename T>
std::string Log<T>::ToString(TLogLevel level)
{
	static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4"};
    return buffer[level];
}

/*********************************
// Convert string to level
*********************************/
template <typename T>
TLogLevel Log<T>::FromString(const std::string& level)
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
    Log<T>().Get(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
    return logINFO;
}


/**********************************************************************************************************************************
// Output to a vector
**********************************************************************************************************************************/

class Output2vector
{
public:
	static std::vector<std::string>*& Stream();
	static std::mutex*& Mutex();
	static void Output(const std::string& msg);
};

inline std::vector<std::string>*& Output2vector::Stream()
{
	static std::vector<std::string>* pStream = nullptr;
	return pStream;
}

inline std::mutex*& Output2vector::Mutex()
{
	static std::mutex* pMutex = nullptr;
	return pMutex;
}

inline void Output2vector::Output(const std::string& msg)
{
	std::vector<std::string>* pStream = Stream();
	if (!pStream)
		return;
	
	std::mutex* pMutex = Mutex();
	if (!pMutex)
		return;

	pMutex->lock();
	pStream->push_back(msg);
	pMutex->unlock();
}

/**********************************************************************************************************************************
// Output to a map
**********************************************************************************************************************************/

class Output2map
{
public:
	static std::map<std::string, std::string>*& Stream();
	static std::mutex*& Mutex();
	static void Output(const std::string& msg);
};

inline std::map<std::string, std::string>*& Output2map::Stream()
{
	static std::map<std::string, std::string>* pStream = nullptr;
	return pStream;
}

inline std::mutex*& Output2map::Mutex()
{
	static std::mutex* pMutex = nullptr;
	return pMutex;
}

inline void Output2map::Output(const std::string& msg)
{
	std::map<std::string, std::string>* pStream = Stream();
	if (!pStream)
		return;

	std::mutex* pMutex = Mutex();
	if (!pMutex)
		return;

	pMutex->lock();
	pStream->emplace(std::make_pair(NowTime(), msg));
	pMutex->unlock();
}

/**********************************************************************************************************************************
// Output to a file
**********************************************************************************************************************************/

class Output2FILE
{
public:
    static FILE*& Stream();
    static void Output(const std::string& msg);
};

inline FILE*& Output2FILE::Stream()
{
    static FILE* pStream = stderr;
    return pStream;
}

inline void Output2FILE::Output(const std::string& msg)
{   
    FILE* pStream = Stream();
    if (!pStream)
        return;
    fprintf(pStream, "%s", msg.c_str());
    fflush(pStream);
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   if defined (BUILDING_FILELOG_DLL)
#       define FILELOG_DECLSPEC   __declspec (dllexport)
#   elif defined (USING_FILELOG_DLL)
#       define FILELOG_DECLSPEC   __declspec (dllimport)
#   else
#       define FILELOG_DECLSPEC
#   endif // BUILDING_DBSIMPLE_DLL
#else
#   define FILELOG_DECLSPEC
#endif // _WIN32

/**********************************************************************************************************************************
// Class definitions
**********************************************************************************************************************************/

class FILELOG_DECLSPEC FILELog : public Log<Output2FILE> {};
class StreamLog : public Log<Output2vector> {};
class MapLog : public Log<Output2map> {};

/**********************************************************************************************************************************
// Macro definitions
**********************************************************************************************************************************/


#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL logDEBUG4
#endif

#ifndef STREAMLOG_MAX_LEVEL
#define STREAMLOG_MAX_LEVEL logDEBUG4
#endif

#ifndef MAPLOG_MAX_LEVEL
#define MAPLOG_MAX_LEVEL logDEBUG4
#endif

#define FILE_LOG(level) \
    if (level > FILELOG_MAX_LEVEL) ;\
    else if (level > FILELog::ReportingLevel() || !Output2FILE::Stream()) ; \
    else FILELog().GetTimeStamped(level)

#define STREAM_LOG(level) \
    if (level > STREAMLOG_MAX_LEVEL) ;\
    else if (level > StreamLog::ReportingLevel() || !Output2vector::Stream() || !Output2vector::Mutex()) ; \
    else StreamLog().GetTimeStamped(level)

#define MAP_LOG(level) \
    if (level > MAPLOG_MAX_LEVEL) ;\
    else if (level > MapLog::ReportingLevel() || !Output2map::Stream() || !Output2map::Mutex()) ; \
    else MapLog().GetNoStamp(level)

#endif //__LOG_H__
