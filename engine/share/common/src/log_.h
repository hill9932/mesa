#ifndef __HL_LOG_INCLUDE_H__
#define __HL_LOG_INCLUDE_H__

#include "common.h"
#include "string_util.h"
#include "time_.h"
#include "buffer_.h"
#include "locker_.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/ndc.h>

#include <list>
#include <memory>
#include <atomic>

using namespace LabSpace::Common;

extern log4cplus::Logger* g_logger;

namespace LabSpace
{
    namespace Common
    {
        bool InitLog(const tchar* _configure, const tchar* _category);
    }
}

#define RM_LOG_DEFINE(CategoryName)  log4cplus::Logger* g_logger;// = log4cplus::Logger::getInstance(CategoryName);

#define PRINT_MSG(msg)  std::cerr   << TimeUtil::GetNowTimeStr() << "\t" \
                                    << StrUtil::GetFileName(__FILE__).c_str() << ":" << __LINE__ << " - " \
                                    << msg << endl;

#ifdef LOG_FILE_LINE_INFORMATION

#define L4C_LOG_TRACE(str)       {   \
    if (g_logger) { LOG4CPLUS_TRACE(*g_logger, " " << \
                    StrUtil::GetFileName(__FILE__) << ":" << __FUNCTION__ << "():" << __LINE__ << " " << str); }\
}

#define L4C_LOG_DEBUG(str)       {   \
    if (g_logger) { LOG4CPLUS_DEBUG(*g_logger, " " << \
                    StrUtil::GetFileName(__FILE__) << ":" << __FUNCTION__ << "():" << __LINE__ << " " << str); }\
}

#define L4C_LOG_INFO(str)        {   \
    if (g_logger) { LOG4CPLUS_INFO(*g_logger,  " " << \
                    StrUtil::GetFileName(__FILE__) << ":" << __FUNCTION__ << "():" << __LINE__ << " " << str);  }\
}

#define L4C_LOG_WARNING(str)     {   \
    if (g_logger) { LOG4CPLUS_WARN(*g_logger,  " " << \
                    StrUtil::GetFileName(__FILE__) << ":" << __FUNCTION__ << "():" << __LINE__ << " " << str); } \
}

#define L4C_LOG_ERROR(str)     {   \
    if (g_logger) { LOG4CPLUS_ERROR(*g_logger,  " " << \
                    StrUtil::GetFileName(__FILE__) << ":" << __FUNCTION__ << "():" << __LINE__ << " " << str); } \
}

#else

#define L4C_LOG_TRACE(str)       {   \
    if (g_logger) { LOG4CPLUS_TRACE(*g_logger, " " << str); }   \
}

#define L4C_LOG_DEBUG(str)       {   \
    if (g_logger) { LOG4CPLUS_DEBUG(*g_logger, " " << str); }   \
}

#define L4C_LOG_INFO(str)        {   \
    if (g_logger) { LOG4CPLUS_INFO(*g_logger,  " " << str);  }  \
}

#define L4C_LOG_WARNING(str)     {   \
    if (g_logger) { LOG4CPLUS_WARN(*g_logger,  " " << str); }   \
}

#define L4C_LOG_ERROR(str)     {   \
    if (g_logger) { LOG4CPLUS_ERROR(*g_logger,  " " << str); }   \
}

#endif


//
// Check the value and log the error messages
//
#define LOG_LAST_ERRORMSG()                     \
{   u_int32 err = Util::GetLastSysError();      \
    L4C_LOG_ERROR(Util::GetLastSysErrorMessage(err).c_str());  \
}

#define LOG_LAST_ERRORMSG_S(msg)                \
{   u_int32 err = Util::GetLastSysError();      \
    L4C_LOG_ERROR(msg << ": " << Util::GetLastSysErrorMessage(err).c_str());\
}

#define LOG_ERRORMSG(errCode)           L4C_LOG_ERROR(Util::GetLastSysErrorMessage(errCode).c_str());
#define LOG_ERRORMSG_S(errCode, msg)    L4C_LOG_ERROR(msg << ": " << Util::GetLastSysErrorMessage(errCode).c_str());

#define LOG_LASTERROR_AND_RETURN_ERRCODE()      \
{                                               \
    int err = Util::GetLastSysError();          \
    if (0 != err)   LOG_ERRORMSG(err);          \
    return err;                                 \
}

#define LOG_LASTERROR_AND_DO(action)            \
{                                               \
    int err = Util::GetLastSysError();          \
    if (0 != err)  { LOG_ERRORMSG(err);         \
        action;                                 \
    }                                           \
}

#define LOG_LASTERROR_AND_RETURN()              \
{                                               \
    int err = Util::GetLastSysError();          \
    if (0 != err)   LOG_ERRORMSG(err);          \
    return;                                     \
}

#define ON_ERROR_RETURN_ERRCODE(expr, comp, error)  if (expr comp error)    return expr;
#define ON_ERROR_PRINT_LAST_ERROR(expr, comp, error)    \
    if (expr comp error)	{                   \
        u_int32 err = Util::GetLastSysError();  \
        LOG_ERRORMSG(err);                      \
    }

#define ON_ERROR_PRINT_LASTMSG_AND_DO(expr, comp, error, action)    \
    if (expr comp error)	{                   \
        u_int32 err = Util::GetLastSysError();  \
        LOG_ERRORMSG(err);                      \
        action;	                                \
    }

#define ON_ERROR_PRINT_MSG(expr, comp, error, msg)    \
    if (expr comp error)	{                   \
        L4C_LOG_ERROR(msg)                       \
    }

#define ON_ERROR_PRINT_MSG_AND_DO(expr, comp, error, msg, action)    \
    if (expr comp error)	{                   \
        L4C_LOG_ERROR(msg)                       \
        action;	                                \
    }

#define ON_ERROR_PRINT_LASTMSG_S_AND_DO(expr, comp, error, msg, action)    \
    if (expr comp error)	{                   \
        u_int32 err = Util::GetLastSysError();  \
        LOG_ERRORMSG_S(err, msg)                \
        action;	                                \
    }

#endif
