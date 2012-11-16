/*
 * Entitie.h
 *
 *  Created on: 25 avr. 2012
 *      Author: rli
 */

#ifndef ENTITIE_H_
#define ENTITIE_H_

#include <iostream>
#include <fstream>
#include "json_spirit.h"
#include "CurlCall.h"
#include "OperationSMDFunctions.h"
#include "fString.h"

#include "LogFactory.h"
using namespace std;

namespace OperationsSMD
{
   /**
    * Proxy between the SMD and the Web Service Server\n
    * It use @ref CurlCallNS::CurlCall to communicate with Web Service
    */
   class OperationStub
   {
      public:
         /**
          * Default Constructor
          */
         OperationStub ();

         /**
          * a Constructor with URL of Server
          * @param url_server url of the web service
          */
         OperationStub (const string &url_server);

         virtual ~OperationStub ();

      protected:
         /**
          * Verify and get the operation attributed to @ref nameSMD
          * @param nameSMD the name of SMD
          * @return Vector of operations
          */
         vector<OperationAbstract*> retrieveOperationList (const string & nameSMD);

         /**
          * @warning use the one with @ref OperationsSMD::OperationAbstract to choose a dynamic type \n
          * @warning or that must have a good string type
          * Get one operation by ID
          * @param id that represent the operation (unique)
          * @param type of operation
          * @return Operation
          */
         OperationAbstract* retrieveOperationById (const long &id, const string & type);

         /**
          * this operation it is developed to get the operation nested on the multiple operation
          * @param operation abstract to get good operation (@ref OperationsSMD::OperationAbstract)
          * @return Operation
          */
         OperationAbstract* retrieveOperationById (OperationAbstract *operation);

         /**
          * To connect to (or disconnect from) the Web Service
          * @param nameSMD name of SMD
          * @param what true (connect), false (disconnect)
          * @return true OK, false else
          */
         bool connectToServer (const string & nameSMD, const bool what = true);

         /**
          * To update a version in the server
          * @param nameSMD
          * @param version
          * @return true OK, false else
          */
         bool sendVersion (const string & nameSMD, const string & version);

         /**
          * To Send a  report to the server
          * @param report @ref OperationsSMD::ReportData
          * @return true OK, false else
          */
         bool sendReportData (ReportData *report);

         /**
          * To Send a  progress operation to the server
          * @param id_operation
          * @param value   the value of progress
          * @return
          */
         bool sendOperationProgress (const long & id_operation, const double & value);

         /**
          * To Send a  progress of other task to the server
          * @param nameSMD
          * @param progress @ref OperationsSMD::OperationProgress
          * @return
          */
         bool sendProgressSMD (const string & nameSMD, ProgressData *progress);

         /**
          * Set the url Server
          * @param url_server the new url server
          */
         void setUrlServer (const string & url_server);

         static LoggerPtr logger;

         string url_server;

      private:
         /**
          * Used by get operations
          */
         string retrieveTextOperationList (const string & nameSMD);

         /**
          * Used by get operations
          */
         vector<OperationAbstract*> deserializeOperationList (const string & json_text);

   };

}
#endif /* ENTITIE_H_ */
