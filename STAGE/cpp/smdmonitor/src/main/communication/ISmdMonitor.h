/**
 * ISmdMonitor.h
 *
 * $Id: ISmdMonitor.h 765 2012-07-26 10:00:49Z rli $
 */

#ifndef ISMDMONITOR_H_
#define ISMDMONITOR_H_

#include "ProgressData.h"
#include "ReportData.h"

using namespace OperationsSMD;

namespace SMDCommunication
{

   /**
    * imaGeau::ISmdMonitor
    * @since 12 juil. 2012
    * @author bdm
    */
   class ISmdMonitor
   {
      public:
         virtual ~ISmdMonitor ();

         /**
          * Connect to  the server
          */
         virtual void connect () = 0;

         /**
          * Disonnect to  the server
          */
         virtual void disconnect () = 0;

         /**
          * Get The Operation to do from the server
          */
         virtual void whatHaveIToDo () = 0;

         /**
          * Do the operation (execute the operation in the SMD)
          */
         virtual bool executeAllOperation () = 0;

         /**
          * To send the actual version of SMD
          */
         virtual void sendVersionUpdated () = 0;

         /**
          * To  send the report (called by @ref executeAllOperation() and @ref OperationsSMD::OperationAbstract::execute())
          * @param report that will be send to the Server
          */
         virtual void sendReport (ReportData *report) = 0;

         /**
          * To  send the progress IPC
          * @param progress of the operation IPCs
          * @warning the id of progress must be unique \n
          *          saved in database with id
          */
         virtual void sendProgressTaskSMD (ProgressData *progress) = 0;

         /**
          * To  send the progress for an operation
          * @param id      id of operation
          * @param value   value of progress of operation
          */
         virtual void sendOperationProgress (const uint32 id, const double value) = 0;

         /**
          * Verify if the SMD is connected or Not
          * @return true if connected, else false
          */
         virtual bool isConnected () = 0;

         /**
          * Set Value of Running
          * @param what
          */
         virtual void setRunning (bool what) = 0;

         static void setInstance (ISmdMonitor * mon);
         static ISmdMonitor * getInstance ();

      private:
         static ISmdMonitor * instance;

   };

} /* namespace SMDCommunication */
#endif /* ISMDMONITOR_H_ */
