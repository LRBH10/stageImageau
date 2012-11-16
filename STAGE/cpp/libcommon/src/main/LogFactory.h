/**
 * LogFactory.h
 *
 * $Id: LogFactory.h 293 2011-10-13 15:03:50Z bdm $
 */

#ifndef LOGFACTORY_H_
#define LOGFACTORY_H_

// include log4cxx header files.
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/rolling/rollingfileappender.h>
#include <log4cxx/fileappender.h>
#include <sys/stat.h>
#include "StationException.h"

namespace imaGeau
{
   using namespace std;
   using namespace log4cxx;
   using namespace log4cxx::xml;

#  define LOG_TRACE(_l,_msg) LOG4CXX_TRACE (_l, _msg)
#  define LOG_INFO(_l,_msg) LOG4CXX_INFO (_l, _msg)
#  define LOG_DEBUG(_l,_msg) LOG4CXX_DEBUG (_l, _msg)
#  define LOG_WARN(_l,_msg) LOG4CXX_WARN (_l, _msg)
#  define LOG_ERROR(_l,_msg) LOG4CXX_ERROR (_l, _msg)
#  define LOG_FATAL(_l,_msg) LOG4CXX_FATAL (_l, _msg)

   /**
    * LogFactory
    * @since 21 déc. 2010
    * @author bdm
    */
   class LogFactory
   {
      public:
         /**
          * Create a log factory
          * @param filename log4cxx conf file
          */
         LogFactory (const string & filename = "conf/log4cxx.xml");

         virtual ~LogFactory ();

         /**
          * @param name logger name
          * @return the corresponding logger or nop ?
          */
         static log4cxx::LoggerPtr getLogger (const string & name);

         /**
          * @return the root logger
          */
         static log4cxx::LoggerPtr getRootLogger ();

         /**
          * Set default parameters for the appender 'stationFileAppender'
          * @param fileName appender output file name
          */
         static void setLogFileName (const string & fileName);

         /**
          * Set default parameters for the specified appender
          * @param appenderName appender name to set
          * @param fileName appender output file name
          */
         static void setLogFileName (const string & appenderName, const string & fileName);

         /**
          * Force rollover for the specified appender
          * @param appenderName appender to rollover
          */
         static void rollOver (const string & appenderName);
   };
}

#endif /* LOGFACTORY_H_ */
