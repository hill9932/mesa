#include "log_.h"
#include "file_util.h"
#include "thread_.h"
#include "system_.h"
#include "stdString.h"


namespace LabSpace
{
    namespace Common
    {
        //
        // init the log4cplus
        //
        bool InitLog(const tchar* _configure, const tchar* _category)
        {
            try
            {
                if (g_logger)   return true;
        
				if (!FileUtil::IsFileExist(_configure))
				{
					PRINT_MSG("Config file not exist: " << _configure);
					return false;
				}
		
                if (!FileUtil::CanFileAccess(_configure))
                {
                    PRINT_MSG("Log configure file '" << _configure << "' is invalid.");
                    return false;
                }

                log4cplus::initialize();
                log4cplus::PropertyConfigurator::doConfigure(_configure);
                g_logger = new log4cplus::Logger(log4cplus::Logger::getInstance(_category));

                //log4cplus::ConfigureAndWatchThread(_configure, 60 * 1000);
#ifdef WIN32
                setlocale(LC_ALL, "");
#endif
            }
            catch (...)
            {
                PRINT_MSG("ERROR: Fail to init log4plus.");// << e.what() << std::endl;
                return false;
            }

            return true;
        }
	}
}
