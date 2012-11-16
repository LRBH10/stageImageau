/*
 * SMDLauncher.cpp
 *
 *  Created on: 25 mai 2012
 *      Author: rli
 */

#include "SMDLauncher.h"

using namespace SMDExecute;

LoggerPtr SMDLauncher::logger (LogFactory::getLogger ("SMDLauncher"));

SMDLauncher::SMDLauncher () :
   OperationStub (), ISmdMonitor (), name (), version (), connected (false), operation_to_do (), workspacepath (), threadipcs (0), running (
      true)
{
   init ();
   ISmdMonitor::setInstance (this);
}

SMDLauncher::SMDLauncher (const string & name, const string &url_server, const string &workspacepath) :
   OperationStub (url_server), ISmdMonitor (), name (name), version (), connected (false), operation_to_do (), workspacepath (
      workspacepath), threadipcs (0), running (true)
{
   ISmdMonitor::setInstance (this);
}

/*
 *
 */
SMDLauncher::~SMDLauncher ()
{
   running = false;

   disconnect ();

   for (size_t i = 0; i < operation_to_do.size (); i++) {
      delete (operation_to_do.at (i));
   }
   ISmdMonitor::setInstance (NULL);

   QueueMessage::killMessageQueue (751988);
}

//  To Connect to SERVER
void SMDLauncher::connect ()
{
   LOG_TRACE(logger, fString::format("CONNECT TO %s", this->url_server.c_str()));
   int dt = 0;
   while (dt < VAL_MAX_WAIT && !connected && running) {
      try {
         this->OperationStub::connectToServer (name);
         connected = true;
      }
      catch (SmdException &exp) {
         exp.calledBy (__FUNC__, __FILE__, __LINE__);
         LOG_WARN(logger, fString::format("EXCEPTION (SMD sleep %d)\n %s\n\n ", dt, exp.what()));
      }
      if (!connected) {
         sleep (INTERVAL_WAIT);
         dt += INTERVAL_WAIT;
      }
   }

   if (dt >= VAL_MAX_WAIT) {
      throw SmdException (fString::format ("%s - Connection Impossible", __FUNC__).c_str (), __FILE__, __LINE__);
   }
   else {
      LOG_INFO(logger, "SMD connected success on the server");
      connected = true;
   }
}

bool SMDLauncher::checkOrConnect ()
{
   if (!connected) {
      try {
         connected = this->OperationStub::connectToServer (name);
      }
      catch (SmdException &exp) {
         LOG_DEBUG(logger, fString::format("Unable to connect. Caused by: %s", exp.what()));
      }
   }

   return (connected && running);
}

// TO disconnect from SERVER
void SMDLauncher::disconnect ()
{
   if (threadipcs != 0) {
      pthread_join (threadipcs, NULL);
      threadipcs = 0;
   }

   LOG_TRACE(logger,
      fString::format("DISCONNECT FROM %s (Value of Connected %s )", this->url_server.c_str(),OperationSMDFunctions::bool_to_string(connected).c_str()));

   int dt = 0;
   while (dt < VAL_MAX_WAIT && connected) {
      try {
         this->OperationStub::connectToServer (name, false);
         connected = false;
      }
      catch (SmdException &exp) {
         exp.calledBy (__FUNC__, __FILE__, __LINE__);
         LOG_WARN(logger, fString::format("EXCEPTION (SMD sleep %d)\n %s\n\n ", dt, exp.what()));

         if (dt == 3) {
            connected = false;
         }
      }

      if (connected) {
         sleep (dt);
         dt++;
      }
   }

   if (dt >= VAL_MAX_WAIT) {
      throw SmdException (fString::format ("%s - Disconnection Impossible", __FUNC__).c_str (), __FILE__, __LINE__);
   }
   else {
      LOG_INFO(logger, "SMD disconnected success on the server");
   }

   connected = false;
}

// TO SEND REPORT Of Operation
void SMDLauncher::sendReport (ReportData *report)
{
   if (checkOrConnect ()) {
      int dt = 0;
      bool isOK = false;
      while (dt < VAL_MAX_WAIT && !isOK && running) {
         try {
            isOK = this->OperationStub::sendReportData (report);
         }
         catch (SmdException &exp) {
            exp.calledBy (__FUNC__, __FILE__, __LINE__);

            LOG_WARN(logger, fString::format("EXCEPTION TO IN Operation  %d \n%s \n\n", report->id, exp.what()));
            isOK = false;
         }

         if (!isOK) {
            sleep (dt);
            dt += INTERVAL_WAIT;
         }
      }

      if (dt >= VAL_MAX_WAIT) {
         connected = false;
         throw SmdException (fString::format ("%s - Sending Report", __FUNC__).c_str (), __FILE__, __LINE__);
      }
      else {
         LOG_INFO(logger, "SMD send Report success on the server");
      }
   }
}

//Send Progress of Operation
void SMDLauncher::sendOperationProgress (const uint32 id, const double value)
{
   if (checkOrConnect ()) {
      /**
       * @todo No exception because it's not important
       */
      try {
         this->OperationStub::sendOperationProgress (id, value);
      }
      catch (SmdException &exp) {
         exp.calledBy (__FUNC__, __FILE__, __LINE__);
         LOG_WARN(logger, fString::format("EXCEPTION  %d - %.2lf\n%s \n\n", id, value, exp.what()));
      }
   }
}

//Send Progress of Operation IPCS
void SMDLauncher::sendProgressTaskSMD (ProgressData *progress)
{
   if (checkOrConnect ()) {
      int dt = 0;
      bool isOK = false;
      while (dt < VAL_MAX_WAIT && !isOK && running) {
         try {
            isOK = this->OperationStub::sendProgressSMD (this->name, progress);
         }
         catch (SmdException &exp) {
            exp.calledBy (__FUNC__, __FILE__, __LINE__);
            LOG_WARN(logger, fString::format("EXCEPTION in Progress  %d - %s\n%s \n\n", progress->id, progress->title.c_str(), exp.what()));
            isOK = false;
         }

         if (!isOK) {
            sleep (dt);
            dt += INTERVAL_WAIT;
         }
      }

      if (dt >= VAL_MAX_WAIT) {
         connected = false;
         throw SmdException (fString::format ("%s - Sending Progress", __FUNC__).c_str (), __FILE__, __LINE__);
      }
      else {
         LOG_INFO(logger, "SMD send Progress success on the server");
      }
   }
}

// TO Update version Of SMD
void SMDLauncher::sendVersionUpdated ()
{
   if (checkOrConnect () && getVersion ()) {
      int dt = 0;
      bool isOK = false;
      while (dt < VAL_MAX_WAIT && !isOK && running) {
         try {
            isOK = this->OperationStub::sendVersion (name, version);
         }
         catch (SmdException &exp) {
            exp.calledBy (__FUNC__, __FILE__, __LINE__);
            LOG_WARN(logger, fString::format("EXCEPTION %s\n%s \n\n", version.c_str(), exp.what()));
            isOK = false;
         }

         if (!isOK) {
            sleep (dt);
            dt += INTERVAL_WAIT;
         }
      }

      if (dt >= VAL_MAX_WAIT) {
         connected = false;
         throw SmdException (fString::format ("%s - Update Version", __FUNC__).c_str (), __FILE__, __LINE__);
      }
      else {
         LOG_INFO(logger, fString::format("SMD change Version to \n '%s' \n success on the server", version.c_str()));
      }
   }
}

// TO GET OPERATIONS FROM SERVER
void SMDLauncher::whatHaveIToDo ()
{
   if (running) {
      operation_to_do = retrieveOperationList (name);
      getNestedMutliOperations ();
   }
}

// TO GET Nested Opertions into Operation Multi
void SMDLauncher::getNestedMutliOperations ()
{
   /**
    * @todo Problem with nested nested operations
    */
   for (size_t i = 0; i < operation_to_do.size (); i++)
      if (OperationSMDFunctions::isOperationTypeOf (operation_to_do.at (i), OperationTypeMulti)) {
         OperationMulti *inter = (OperationMulti *) operation_to_do.at (i);
         for (size_t j = 0; j < inter->getNestedOperations ()->size (); j++)
            inter->getNestedOperations ()->at (j) = retrieveOperationById (inter->getNestedOperations ()->at (j));
         operation_to_do.at (i) = inter;
      }

}

// TO DO the OPERATIONS
bool SMDLauncher::executeAllOperation ()
{
   bool retourned = true;
   if (running) {
      LOG_INFO(logger, "Executions for operations");
      for (size_t i = 0; i < operation_to_do.size (); i++) {
         ReportData *point = operation_to_do.at (i)->execute ();
         retourned = retourned && point->status;
         delete point;
      }
   }
   return retourned;
}

//  TO DEBUG THE SMD OPERATION LaNCHER
void SMDLauncher::debug ()
{
   cout << "**************************************" << endl;
   cout << "NAME \t\t\t: " << this->name << endl;
   cout << "VERSION \t\t: " << this->version << endl;
   cout << "WORCKSPACE \t\t: " << this->workspacepath << endl;
   cout << "SERVER \t\t\t: " << this->url_server << endl;

   for (size_t i = 0; i < operation_to_do.size (); i++) {
      cout << "*********\n";
      operation_to_do.at (i)->debug ();
   }
   cout << "**************************************" << endl;
}

//initialize for "SMDMonitor.cfg"
void SMDLauncher::init ()
{
   try {
      Properties prop ("conf/SMDMonitor.cfg");
      this->name = prop.get ("smd_name");
      this->workspacepath = prop.get ("workspace_smd");
      this->url_server = prop.get ("url_server");
   }
   catch (StationException &exp) {
      SmdException exp2 (exp);
      exp2.calledBy (fString::format ("%s Problem with Read SMDMonitor.cfg ", __FUNC__).c_str (), __FILE__, __LINE__);
      throw exp2;
   }
}

//Communication with others process
void* SMDLauncher::receiveMessageIpcCallBack (void * parameters)
{
   long type = (long) parameters;
   QueueMessage tst (type);

   SMDLauncher *monitor = (SMDLauncher *) ISmdMonitor::getInstance ();
   monitor->running = true;

   while (monitor->running) {
      SmdIPCGlobalResponse data;
      tst.receiveSharedMessageSMD (data);

      switch (data.type) {
         case SmdIPCStruct_progress: {
            ProgressData prog (data.RProgress.id, data.RProgress.title, data.RProgress.progress_value);
            try {
               monitor->sendProgressTaskSMD (&prog);
            }
            catch (SmdException &exp) {
               exp.calledBy (__FUNC__, __FILE__, __LINE__);
               LOG_FATAL(SMDLauncher::logger, fString::format("EXCEPTION\n%s\n\n\n", exp.what()));
            }
            break;
         }

         case SmdIPCStruct_bool:
            monitor->running = data.RBool.request;
            break;

         default:
            break;
      }
   }

   return NULL;
}

bool SMDLauncher::isConnected ()
{
   return connected;
}

void SMDLauncher::startListing ()
{
   pthread_create (&threadipcs, NULL, SMDLauncher::receiveMessageIpcCallBack, (void *) 0);
}

bool SMDLauncher::getVersion ()
{
   string version_svn = "";
   chdir (workspacepath.c_str ());
   string cmd = "./version_svn_getter.sh  2>&1";

   bool returned = true;

   char line [1000];
   memset (line, 0, 1000);

   FILE *fp;
   fp = popen (cmd.c_str (), "r");
   if (fp == NULL) {
      LOG_FATAL(logger, "Popen execute ERROR");
   }

   while (fgets (line, sizeof(line), fp) != NULL) {
      //! @todo transformation on exception

      if (atoi (line) != 0) {
         version_svn.append (line);
         version_svn.append ("-");
      }
      else {
         version_svn.append ("0-");
         LOG_FATAL(logger, "SVN VERSION N'EXISTE PAS ");
      }
      memset (line, 0, 1000);
   }

   int status = pclose (fp);
   if (status != 0) {
      returned = false;
      LOG_FATAL(logger,
         fString::format("THERE IS NOT '%s/version_svn_getter.sh' IN YOUR OBSIRVATORY TO GET VERSION", this->workspacepath.c_str()));
   }
   /*
    * METTRE A JOUR LA VERSION
    */
   LOG_INFO(logger, version_svn);
   version = version_svn;
   return returned;
}

void SMDLauncher::setRunning (bool what)
{
   this->running = what;
}

