/*
 * OperationSvnUpdate.h
 *
 *  Created on: 11 juin 2012
 *      Author: rli
 */

#ifndef OPERATIONSVNUPDATE_H_
#define OPERATIONSVNUPDATE_H_

#include "OperationSvn.h"

namespace OperationsSMD
{
   /**
    *  To update a repository
    */
   class OperationSvnUpdate: public OperationSvn
   {
      public:
         OperationSvnUpdate ();

         OperationSvnUpdate (uint32 _id, OperationType& _type, const string & _user, const string& _path, double _state, bool _isAchived,
            bool _isNested, bool report_on_success, OperationType &svn_type, const string & conflict, const string & head, bool forced);
         virtual ~OperationSvnUpdate ();

         virtual void deserialize (json_spirit::Object &json_input);
         virtual void debug ();

      protected:
         virtual ReportData* runOperation ();
         virtual ReportData* postCheck ();
         virtual ReportData* preCheck ();

   };

} /* namespace OperationsSMD */
#endif /* OPERATIONSVNUPDATE_H_ */
