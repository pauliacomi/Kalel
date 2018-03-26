#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "timeHelpers.h"
#include "classHelpers.h"



/**********************************************************************************************************************************
// Template
**********************************************************************************************************************************/

enum TLogLevel {logEVENT, logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};
enum TQuestionType { qOK, qYESNO, qYESCANCEL, qYESTRYCANCEL };

/*********************************
// Class
*********************************/
template <typename T>
class Log
{
public:
    Log();
    virtual ~Log();
	std::ostringstream& GetBase(TLogLevel level = logINFO);							// Get just the message sent
    std::ostringstream& GetNoStamp(TLogLevel level = logINFO);						// Get a stringstream without timestamp
	std::ostringstream& GetTimeStamped(TLogLevel level = logINFO);					// Get a stringstream with timestamp
	std::ostringstream& GetTimeStamp();												// Get a stringstream only the timestamp
	std::ostringstream& GetNoStampAndAppended(TLogLevel level, TQuestionType type);	// Get an extra appended digit signifying type

public:
    static TLogLevel& ReportingLevel();
    static std::string ToString(TLogLevel level);
    static TLogLevel FromString(const std::string& level);
	static TLogLevel FromString(const std::wstring& level);
	static TQuestionType QtypeFromString(const std::wstring& level);

	// String stream
protected:
    std::ostringstream os;
	TLogLevel local_level = logINFO;
	std::chrono::system_clock::time_point timestamp;

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
	timestamp = timeh::NowTime();						// Get time and store it
}

/*********************************
// Get base
*********************************/
template <typename T>
std::ostringstream& Log<T>::GetBase(TLogLevel level)
{
	local_level = level;
	return os;
}

/*********************************
// Get log unstamped
*********************************/
template <typename T>
std::ostringstream& Log<T>::GetNoStamp(TLogLevel level)
{
	local_level = level;
	os << ToString(level) << ": ";
    os << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');			// Tab it out
    return os;
}

/*********************************
// Get log and appended type
*********************************/
template <typename T>
std::ostringstream& Log<T>::GetNoStampAndAppended(TLogLevel level, TQuestionType type)
{
	local_level = level;
	os << ToString(level) << ":" << type << ": ";
	return os;
}

/*********************************
// Get log stamped
*********************************/
template <typename T>
std::ostringstream& Log<T>::GetTimeStamped(TLogLevel level)
{
	local_level = level;
	os << "- " << timeh::TimePointToString(timestamp);
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
	os << timeh::TimePointToString(timestamp);
	return os;
}

/*********************************
// Destructor
*********************************/
template <typename T>
Log<T>::~Log()
{
    os << std::endl;								// Write an endline
    T::Output(os.str(), local_level, timestamp);	// Call write function of template class
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
	static const char* const buffer[] = {"EVENT", "ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4"};
    return buffer[level];
}

/*********************************
// Convert string to level
*********************************/
template <typename T>
TLogLevel Log<T>::FromString(const std::string& level)
{
	if (level == L"EVENT")
		return logEVENT;
    if (level == L"DEBUG4")
        return logDEBUG4;
    if (level == L"DEBUG3")
        return logDEBUG3;
    if (level == L"DEBUG2")
        return logDEBUG2;
    if (level == L"DEBUG1")
        return logDEBUG1;
    if (level == L"DEBUG")
        return logDEBUG;
    if (level == L"INFO")
        return logINFO;
    if (level == L"WARNING")
        return logWARNING;
    if (level == L"ERROR")
        return logERROR;
    Log<T>().GetTimeStamped(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
    return logINFO;
}

template <typename T>
TLogLevel Log<T>::FromString(const std::wstring& level)
{
	if (level == L"EVENT")
		return logEVENT;
	if (level == L"DEBUG4")
		return logDEBUG4;
	if (level == L"DEBUG3")
		return logDEBUG3;
	if (level == L"DEBUG2")
		return logDEBUG2;
	if (level == L"DEBUG1")
		return logDEBUG1;
	if (level == L"DEBUG")
		return logDEBUG;
	if (level == L"INFO")
		return logINFO;
	if (level == L"WARNING")
		return logWARNING;
	if (level == L"ERROR")
		return logERROR;
	return logINFO;
}


/*********************************
// Convert string to question
*********************************/

template <typename T>
TQuestionType Log<T>::QtypeFromString(const std::wstring& qtype)
{
	if (qtype == L"0")
		return qOK;
	if (qtype == L"1")
		return qYESNO;
	if (qtype == L"2")
		return qYESCANCEL;
	if (qtype == L"3")
		return qYESTRYCANCEL;
	return qOK;
}


/**********************************************************************************************************************************
// Custom classs extensions
**********************************************************************************************************************************/


/******************************************************************************
// Output to a vector
*******************************************************************************/
class Output2vector
{
public:
	static std::vector<std::string>*& Stream();
	static void Output(const std::string& msg, TLogLevel level, std::chrono::system_clock::time_point timestamp);
};

inline std::vector<std::string>*& Output2vector::Stream()
{
	static std::vector<std::string>* pStream = nullptr;
	return pStream;
}

inline void Output2vector::Output(const std::string& msg, TLogLevel level, std::chrono::system_clock::time_point timestamp)
{
	std::vector<std::string>* pStream = Stream();
	if (!pStream)
		return;

	pStream->push_back(msg);
}

/******************************************************************************
// Output to a map
*******************************************************************************/

class OutputGeneral
{
public:
	static StampedSafeStorage<std::string>*& Event();
	static StampedSafeStorage<std::string>*& Info();
	static StampedSafeStorage<std::string>*& Debug();
	static void Output(const std::string& msg, TLogLevel level, std::chrono::system_clock::time_point timestamp);
};


inline StampedSafeStorage<std::string>*& OutputGeneral::Event()
{
	static StampedSafeStorage<std::string>* pStream = nullptr;
	return pStream;
}

inline StampedSafeStorage<std::string>*& OutputGeneral::Info()
{
	static StampedSafeStorage<std::string>* pStream = nullptr;
	return pStream;
}

inline StampedSafeStorage<std::string>*& OutputGeneral::Debug()
{
	static StampedSafeStorage<std::string>* pStream = nullptr;
	return pStream;
}

inline void OutputGeneral::Output(const std::string& msg, TLogLevel level, std::chrono::system_clock::time_point timestamp)
{
	StampedSafeStorage<std::string>* pStream = nullptr;
	
	switch (level)
	{
	case logEVENT:
	case logERROR:
	case logWARNING:
		pStream = Event();
		break;
	case logINFO:
		pStream = Info();
		break;
	case logDEBUG:
	case logDEBUG1:
	case logDEBUG2:
	case logDEBUG3:
	case logDEBUG4:
		pStream = Debug();
		break;
	default:
		pStream = nullptr;
		break;
	}
	
	if (!pStream)
		return;

	pStream->push(timestamp, msg);
}

/******************************************************************************
// Output to a file
*******************************************************************************/

class Output2FILE
{
public:
    static FILE*& Stream();
    static void Output(const std::string& msg, TLogLevel level, std::chrono::system_clock::time_point timestamp);
};

inline FILE*& Output2FILE::Stream()
{
    static FILE* pStream = stderr;
    return pStream;
}

inline void Output2FILE::Output(const std::string& msg, TLogLevel level, std::chrono::system_clock::time_point timestamp)
{   
    FILE* pStream = Stream();
    if (!pStream)
        return;
    fprintf(pStream, "%s", msg.c_str());
    fflush(pStream);
}


/**********************************************************************************************************************************
// Class definitions
**********************************************************************************************************************************/

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


class GeneralLog : public Log<OutputGeneral> {};
class FILELOG_DECLSPEC FILELog : public Log<Output2FILE> {};
class StreamLog : public Log<OutputGeneral> {};


/**********************************************************************************************************************************
// Macro definitions
**********************************************************************************************************************************/


#ifndef LOG_MAX_LEVEL
#define LOG_MAX_LEVEL logDEBUG4
#endif

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL logDEBUG4
#endif

#ifndef STREAMLOG_MAX_LEVEL
#define STREAMLOG_MAX_LEVEL logDEBUG4
#endif


#define LOG(level) \
    if (level > LOG_MAX_LEVEL) ;\
    else if (level > GeneralLog::ReportingLevel() || !OutputGeneral::Event() || !OutputGeneral::Info() || !OutputGeneral::Debug()) ; \
    else GeneralLog().GetNoStamp(level)

#define LOG_EVENT(type) \
    if (!OutputGeneral::Event()) ; \
    else GeneralLog().GetNoStampAndAppended(logEVENT, type)

#define FILE_LOG(level) \
    if (level > FILELOG_MAX_LEVEL) ;\
    else if (level > FILELog::ReportingLevel() || !Output2FILE::Stream()) ; \
    else FILELog().GetTimeStamped(level)

#define STREAM_LOG(level) \
    if (level > STREAMLOG_MAX_LEVEL) ;\
    else if (level > StreamLog::ReportingLevel() || !Output2vector::Stream()) ; \
    else StreamLog().GetTimeStamped(level)