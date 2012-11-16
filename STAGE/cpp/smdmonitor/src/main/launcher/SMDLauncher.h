/*
 * SMDLauncher.h
 *
 *  Created on: 25 mai 2012
 *      Author: rli
 */

#ifndef SMDLAUNCHER_H_
#define SMDLAUNCHER_H_

#include <pthread.h>
#include <csignal>

#include "SmdIPCStruct.h"
#include "QueueMessage.h"

#include "Properties.h"
#include "OperationsStub.h"
#include "LogFactory.h"
#include "ISmdMonitor.h"

using namespace systemcall;
using namespace OperationsSMD;

#define INTERVAL_WAIT   5
#define VAL_MAX_WAIT    60
/**
 * Name space to run SMD process
 */
namespace SMDExecute
{

   /**
    * Contains all SMD properties and\n
    * different functions to communicate with the web service
    */
   class SMDLauncher: public OperationStub, ISmdMonitor
   {

      public:

         /**
          * @return Singleton of SMDLauncher
          */
         //static SMDLauncher* getInstance ();
         /**
          * constructor without @ref init() (not use config/SMDMonitor.cfg)\n
          * @param name     name of SMD
          * @param url_server    web service url
          * @param workspacepath the path that contains the shell to get the version
          */
         SMDLauncher (const string & name, const string &url_server, const string &workspacepath);

         /**
          * Default Constructor
          */
         SMDLauncher ();

         /**
          * Default destructor
          */
         virtual ~SMDLauncher ();

         static LoggerPtr logger;

         /**
          * Connect to  the server
          */
         void connect ();
         /**
          * Disonnect to  the server
          */
         void disconnect ();

         /**
          * Get The Operation to do from the server
          */
         void whatHaveIToDo ();

         /**
          * Do the operation (execute the operation in the SMD)
          */
         bool executeAllOperation ();

         /**
          * To send the actual version of SMD
          */
         void sendVersionUpdated ();

         /**
          * To  send the report (called by @ref executeAllOperation() and @ref OperationsSMD::OperationAbstract::execute())
          * @param report that will be send to the Server
          */
         void sendReport (ReportData *report);

         /**
          * To  send the progress IPC
          * @param progress of the operation IPCs
          * @warning the id of progress must be unique \n
          *          saved in database with id
          */
         void sendProgressTaskSMD (ProgressData *progress);

         /**
          * To  send the progress for an operation
          * @param id      id of operation
          * @param value   value of progress of operation
          */
         void sendOperationProgress (const uint32 id, const double value);

         /**
          *  show all operation in the SMD
          */
         void debug ();

         /**
          *  To initialize it with file Configuration "SMDMonitor.cfg"
          */
         void init ();

         /**
          * Function of thread listener for IPCS
          * @param parameters the id of communication
          */
         static void * receiveMessageIpcCallBack (void * parameters);

         /**
          * Verify if the SMD is connected or Not
          * @return true if connected, else false
          */
         bool isConnected ();

         /**
          * to start communication with others process \n
          * run the @ref recieveMessageIpcCallBack
          */
         void startListing ();

         /**
          * To check if the SMD is connected or not.
          * @return
          */
         bool checkOrConnect ();

         /**
          * To set Runinig Variable
          */
         void setRunning (bool what);

      private:
         /**
          * To get Detail of Operation Nested in Multi operation
          */
         void getNestedMutliOperations ();

         /**
          * To set a version from file 'bin/version_svn_getter.sh'
          * @return false if this does not exist
          */
         bool getVersion ();

         string name;
         string version;
         bool connected;

         /**
          * the operation getted from Server
          */
         vector<OperationAbstract *> operation_to_do;
         /**
          * for Svn update workpath (to get svn info)
          */
         string workspacepath;

         pthread_t threadipcs; //! thread listener for the IPC V5 (@ref  communication_with_otherProcess)

         bool running;
   };

}

#endif /* SMDLAUNCHER_H_ */
