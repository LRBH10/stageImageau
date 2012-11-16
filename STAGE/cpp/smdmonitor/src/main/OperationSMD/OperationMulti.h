/*
 * OperationMulti.h
 *
 *  Created on: 24 mai 2012
 *      Author: rli
 */

#ifndef OPERATIONMULTI_H_
#define OPERATIONMULTI_H_

#include "OperationAbstract.h"
#include "OperationSMDFunctions.h"

using namespace std;

namespace OperationsSMD
{

   /**
    * Operation contains a List of others operations
    */
   class OperationMulti: public OperationAbstract
   {
      public:
         /**
          * Default Constructor
          */
         OperationMulti ();
         /**
          * Constructor for the Test Only
          */
         OperationMulti (uint32 _id, OperationType _type, const string& _user, const string& _path, double _state, bool _isAchived,
            bool _isNested, bool report_on_success, bool fail_on_error);
         /**
          * Default destructor
          */
         virtual ~OperationMulti ();

         /**
          * @ref OperationAbstract::deserialize(Object &json_input)
          */
         virtual void deserialize (json_spirit::Object &json_input);
         /**
          * @ref OperationAbstract::debug()
          */
         virtual void debug ();

         OperationAbstract * getNestedOperationById (long id);
         OperationAbstract * getNestedOperationByIndex (int index);

         vector<OperationAbstract *>* getNestedOperations ();

      protected:
         virtual ReportData* runOperation ();
         virtual ReportData* postCheck ();
         virtual ReportData* preCheck ();

      private:

         bool fail_on_error;
         vector<OperationAbstract*> operations;
   };

} /* namespace OperationsSMD */
#endif /* OPERATIONMULTI_H_ */
