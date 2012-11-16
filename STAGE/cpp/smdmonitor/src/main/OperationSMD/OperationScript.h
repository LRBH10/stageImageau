/*
 * OperationScript.h
 *
 *  Created on: 24 mai 2012
 *      Author: rli
 */

#ifndef OPERATIONSCRIPT_H_
#define OPERATIONSCRIPT_H_

#include "OperationAbstract.h"
#include "unistd.h"

namespace OperationsSMD
{

   /**
    * Operation that execute un simple script shell
    */
   class OperationScript: public OperationsSMD::OperationAbstract
   {
      public:
         OperationScript ();
         OperationScript (uint32 _id, OperationType& _type, const string& _user, const string& _path, double _state, bool _isAchived,
            bool _isNested, bool report_on_success, const string & command, const string & parameters);
         virtual ~OperationScript ();

         virtual void deserialize (json_spirit::Object &json_input);
         virtual void debug ();

      protected:
         virtual ReportData* runOperation ();
         virtual ReportData* postCheck ();
         virtual ReportData* preCheck ();

      private:
         string command;
         string parameters;
   };

} /* namespace OperationsSMD */
#endif /* OPERATIONSCRIPT_H_ */
