/*
 * OperationSvn.h
 *
 *  Created on: 11 juin 2012
 *      Author: rli
 */

#ifndef OPERATIONSVN_H_
#define OPERATIONSVN_H_

#include "OperationAbstract.h"

namespace OperationsSMD
{
   /**
    * for the SVN Operation
    */
   class OperationSvn: public OperationsSMD::OperationAbstract
   {
      public:
         OperationSvn ();
         OperationSvn (uint32 _id, OperationType& _type, const string& _user, const string& _path, double _state, bool _isAchived,
            bool _isNested, bool report_on_success, OperationType &svn_type, const string & conflict, const string & head, bool forced);
         virtual ~OperationSvn ();

         OperationType getSvnType ();

         virtual void deserialize (json_spirit::Object &json_input);
         virtual void debug ();

         /**
          * ConflictAction String To Enum
          */
         static string getConflictFromString (const string & type);

         /**
          *
          * @param line
          * @param version output param
          * @return
          */
         static bool version_line_getter (const string &line, string &version);
         static bool svn_line_validator (const string &line);

      protected:
         virtual ReportData* runOperation ();
         virtual ReportData* postCheck ();
         virtual ReportData* preCheck ();

         OperationType svn_type;
         string head;
         string conflict;
         bool forced;

   };

} /* namespace OperationsSMD */
#endif /* OPERATIONSVN_H_ */
