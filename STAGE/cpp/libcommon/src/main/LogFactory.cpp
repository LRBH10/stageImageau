#include "LogFactory.h"

LogFactory::LogFactory (const string & filename)
{
   try {
      DOMConfigurator::configure (filename);
   }
   catch (exception &e) {
      THROW_SE(fString::format ("Cannot initialize logging ! Caused by: \n%s", e.what ()));
   }
}

LogFactory::~LogFactory ()
{
   // nop
}

log4cxx::LoggerPtr LogFactory::getRootLogger ()
{
   return log4cxx::Logger::getRootLogger ();
}

log4cxx::LoggerPtr LogFactory::getLogger (const string & name)
{
   static LogFactory lf; // to init the logger with the conf file
   log4cxx::LoggerPtr logger (log4cxx::Logger::getLogger (name));
   return logger;
}

void LogFactory::setLogFileName (const string & logFileName)
{
   LogFactory::setLogFileName ("stationFileAppender", logFileName);
}

void LogFactory::setLogFileName (const string & appenderName, const string & logFileName)
{
   log4cxx::Appender * app = LogFactory::getRootLogger ()->getAppender (appenderName.c_str ());
   log4cxx::FileAppender * fileApp = dynamic_cast<log4cxx::FileAppender *> (app);
   if (fileApp != NULL) {
      log4cxx::helpers::Pool p;
      if (!logFileName.empty ()) {
         string oldFileName = fileApp->getFile ();
         fileApp->setOption ("file", logFileName);
         fileApp->activateOptions (p);
         if (oldFileName.compare (logFileName) != 0) {
            remove (oldFileName.c_str ());
         }
      }

      log4cxx::rolling::RollingFileAppender * rollApp = dynamic_cast<log4cxx::rolling::RollingFileAppender *> (fileApp);
      if (rollApp != NULL) {
         if (!logFileName.empty ()) {
            rollApp->rollingPolicy->setOption ("FileNamePattern", logFileName + ".%i.gz");
            rollApp->rollingPolicy->setOption ("activeFileName", logFileName);
            rollApp->rollingPolicy->activateOptions (p);
         }

         // Attempt to get the file attributes
         struct stat stFileInfo;
         if (stat (logFileName.c_str (), &stFileInfo) == 0 && stFileInfo.st_size != 0) {
            rollApp->rollover (p);
         }
      }
      else {
         printf ("appender cast failed!\n");
      }
   }
}

void LogFactory::rollOver (const string & appenderName)
{
   log4cxx::Appender * app = LogFactory::getRootLogger ()->getAppender (appenderName.c_str ());
   log4cxx::FileAppender * fileApp = dynamic_cast<log4cxx::FileAppender *> (app);
   if (fileApp != NULL) {
      log4cxx::rolling::RollingFileAppender * rollApp = dynamic_cast<log4cxx::rolling::RollingFileAppender *> (fileApp);
      if (rollApp != NULL) {
         log4cxx::helpers::Pool p;
         rollApp->rollover (p);
      }
      else {
         printf ("appender cast failed!\n");
      }
   }
}
