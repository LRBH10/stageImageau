/*
 * OperationSMDFunctions.h
 *
 *  Created on: 24 mai 2012
 *      Author: rli
 */

#ifndef OPERATIONSMDFUNCTIONS_H_
#define OPERATIONSMDFUNCTIONS_H_

#include "LogFactory.h"
#include "json_spirit.h"
#include "OperationAbstract.h"

using namespace std;

namespace OperationsSMD
{
   /**
    * Interface of operation used in this namespace
    */
   class OperationSMDFunctions
   {
      public:
         static LoggerPtr logger;

         /**
          * generator d'operation from JSON
          */
         static OperationAbstract* OperationFactory (json_spirit::Object json_input);

         /**
          *  test the type of operation
          */
         static bool isOperationTypeOf (OperationAbstract* operation, OperationType);
         static json_spirit::Value searchAttribute (json_spirit::Object &obj, const string & val);

         /**
          * Conversion from enum to string
          */
         static OperationType OperationTypeFromString (const string & type_operation);
         static string OperationTypeToString (OperationType type_operation);

         /**
          * Conversion from Bool TO String
          */
         static string bool_to_string (bool variable);
   };

} /* namespace OperationsSMD */

#include "OperationsIncludes.h"

#endif /* OPERATIONSMDFUNCTIONS_H_ */
