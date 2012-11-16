/*
 * OperationAbstract.h
 *
 *  Created on: 23 mai 2012
 *      Author: rli
 */

#ifndef OPERATIONABSTRACT_H_
#define OPERATIONABSTRACT_H_
#include <iostream>
#include <fstream>
#include <string>
#include "json_spirit.h"
#include "CurlCall.h"
#include "ReportData.h"
#include "ISmdMonitor.h"
#include "LogFactory.h"

using namespace std;
using namespace SMDCommunication;

/**
 * Name Space that contains the implementation of operation that the SMD can do
 */
namespace OperationsSMD
{
   /**
    * Different operation supported by SMD
    */
   enum OperationType
   {
      OperationTypeInconnu = 0,  //!< When the type can't be read or not initialized
      OperationTypeAbstract = 1, //!< For the Local creation (Test)
      OperationTypeScript = 2,   //!< Represent the @ref OperationScript
      OperationTypeMulti = 3,    //!< Represent the @ref OperationMulti
      OperationTypeShellFile = 4,    //!< Represent the @ref OperationShellFile
      OperationTypeSvn = 5,      //!< For the Local creation (Test)
      OperationTypeSvnUpdate = 6,      //!< Represent the @ref OperationSvnUpdate
      OperationTypeSvnSwitch = 7 //!< Represent the @ref OperationSvnSwitch
   };

   /**
    *  the Action to do for @ref OperationShellFile
    */
   enum ShellFileAction
   {
      /**
       * To create the file if doesn't  exist
       *  @warning  else an error returned
       */

      ShellFileCopy = 0,
      /**
       * To create the file however if it exist or not
       * @warning if exist the file will be override
       */
      ShellFileReplace = 1,
      /**
       * a default action
       */
      ShellFileNothing = 2
   };

   /**
    * Operation parent of all operations \n
    * contains the different  field that all operation must have \n
    * and the operation for the execution.
    */
   class OperationAbstract
   {
      public:
         /**
          * Default constructor
          */
         OperationAbstract ();
         /**
          *  For the test Only
          */
         OperationAbstract (uint32 _id, OperationType _type, const string& _user, const string& _path, double _state, bool _isAchived,
            bool _isNested, bool report_on_success);

         /**
          * Default destructor
          */
         virtual ~OperationAbstract ();

         static LoggerPtr logger;

         /**
          * @param json_input
          */
         virtual void deserialize (json_spirit::Object &json_input);

         /**
          * Show all operation
          */
         virtual void debug ();

         /**
          * Get id for current operation
          */
         long getIdOperation ();

         /**
          * Get Type of operation
          * @return @ref OperationType
          */
         OperationType getTypeOperation ();

         /**
          * get a value of the attribute @ref report_on_success
          * @return true || false
          */
         bool isReportOnSuccess ();

         /**
          * get a value of the attribute @ref achieved
          * @return true || false
          */
         bool isAchieved ();

         /**
          * enable or disable the send report \n
          * in test, you can set this variable to true
          * @param test
          */
         void setInTest (bool test);

         /**
          * Execution of operation with calling in this order\n
          * @code
          *  preCheck();
          *  runOperation();
          *  preCheck();
          * @endcode
          * @warning returned object has to be deleted
          * @return  @ref ReportData of operation
          */
         virtual ReportData* execute ();

      protected:
         /**
          * To run the operation (called by @ref execute() )
          * @warning returned object has to be deleted
          * @return @ref ReportData of operation
          */
         virtual ReportData* runOperation ();

         /**
          * verification after running operation (called by @ref execute() )
          * @warning returned object has to be deleted
          * @return @ref ReportData of operation
          */
         virtual ReportData* postCheck ();

         /**
          * verification before running operation (called by @ref execute() )
          * @warning returned object has to be deleted
          * @return @ref ReportData of operation
          */
         virtual ReportData* preCheck ();

         uint32 id;
         OperationType type;
         string user;
         string path;
         double state;
         bool achieved;
         bool nested;
         bool report_on_success;

         bool inTest;
   };

} /* namespace OperationsSMD */

#include "OperationSMDFunctions.h"

#endif /* OPERATIONABSTRACT_H_ */
