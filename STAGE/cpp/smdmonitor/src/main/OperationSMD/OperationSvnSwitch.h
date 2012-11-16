/*
 * OperationSwitch.h
 *
 *  Created on: 11 juin 2012
 *      Author: rli
 */

#ifndef OPERATIONSWITCH_H_
#define OPERATIONSWITCH_H_

#include "OperationSvn.h"

namespace OperationsSMD
{
   /**
    * To switch  the repository to another url
    */
   class OperationSvnSwitch: public OperationsSMD::OperationSvn
   {
      public:
         OperationSvnSwitch ();
         virtual ~OperationSvnSwitch ();

         virtual void deserialize (json_spirit::Object &json_input);
         virtual void debug ();

      protected:
         virtual ReportData* runOperation ();
         virtual ReportData* postCheck ();
         virtual ReportData* preCheck ();

      private:
         string url_switch;

   };

} /* namespace OperationsSMD */
#endif /* OPERATIONSWITCH_H_ */
