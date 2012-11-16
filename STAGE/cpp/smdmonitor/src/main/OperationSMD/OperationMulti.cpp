/*
 * OperationMulti.cpp
 *
 *  Created on: 24 mai 2012
 *      Author: rli
 */

#include "OperationMulti.h"

using namespace OperationsSMD;

OperationMulti::OperationMulti () :
   OperationAbstract (), fail_on_error (false), operations ()
{
   // nop
}

OperationMulti::OperationMulti (uint32 _id, OperationType _type, const string& _user, const string& _path, double _state, bool _isAchived,
   bool _isNested, bool report_on_success, bool _fail_on_error) :
   OperationAbstract (_id, _type, _user, _path, _state, _isAchived, _isNested, report_on_success), fail_on_error (_fail_on_error), operations ()
{
   // nop
}

OperationMulti::~OperationMulti ()
{
   // nop
   for (size_t i = 0; i < operations.size (); i++) {
      delete operations.at (i);
   }
}

void OperationMulti::deserialize (json_spirit::Object &json_input)
{

   json_spirit::Array operations;
   try {
      this->OperationAbstract::deserialize (json_input);
      this->fail_on_error = OperationSMDFunctions::searchAttribute (json_input, "fail_on_error").get_bool ();
      operations = OperationSMDFunctions::searchAttribute (json_input, "operations").get_array ();
   }
   catch (SmdException &exc) {
      exc.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }
   catch (runtime_error &rf) {
      throw SmdException (fString::format ("JSON SPIRIT (%s) ", rf.what ()).c_str (), __FILE__, __LINE__);
   }

   for (size_t i = 0; i < operations.size (); i++) {
      //recuperer l'operation at i
      json_spirit::Object obj = operations [i].get_obj ();

      OperationAbstract *op = new OperationAbstract ();
      try {
         op->deserialize (obj);
      }
      catch (SmdException &exc) {
         exc.calledBy (__FUNC__, __FILE__, __LINE__);
         throw;
      }

      this->operations.push_back (op); //*/
   }

}

void OperationMulti::debug ()
{
   OperationAbstract::debug ();
   cout << "fail_on_error \t: " << OperationSMDFunctions::bool_to_string (fail_on_error) << endl;
   for (size_t i = 0; i < operations.size (); i++) {
      cout << "------------- Nested -------------\n";
      operations.at (i)->debug ();
   }
}

OperationAbstract *OperationMulti::getNestedOperationById (long id)
{
   for (size_t i = 0; i < operations.size (); i++)
      if (operations.at (i)->getIdOperation () == id)
         return operations.at (i);
   return 0;
}

OperationAbstract *OperationMulti::getNestedOperationByIndex (int index)
{
   return operations.at ((size_t) index);
}

vector<OperationAbstract *> * OperationMulti::getNestedOperations ()
{
   return &operations;
}

/*
 * Execute
 * @todo to correct
 */
ReportData* OperationMulti::runOperation ()
{
   ReportData *result = this->OperationAbstract::runOperation ();
   if (result->status) {
      bool loop = true;
      for (size_t i = 0; loop && i < operations.size (); i++) {

         LOG_TRACE(logger, fString::format("Nested Execution for operation with id  %d", operations.at(i)->getIdOperation()));

         result->contents.append ("\n other Operation(Nested)\n");
         ReportData * tmpReport = operations.at (i)->execute ();
         result->append (tmpReport);
         delete tmpReport;

         if (!result->status && this->fail_on_error) {
            loop = false;
         }
         if (!inTest) {
            int pro = (int) i * 50 / (int) operations.size ();
            ISmdMonitor::getInstance ()->sendOperationProgress (this->id, pro);
         }
      }
   }
   return result;
}

/*
 * after execution
 */
ReportData* OperationMulti::postCheck ()
{
   ReportData *result = this->OperationAbstract::postCheck ();
   /**
    * @todo en cas d'ajoute des instruction
    * if (!result->status)
    * return result;
    */
   return result;
}

/*
 * before execution
 */
ReportData* OperationMulti::preCheck ()
{
   ReportData *result = this->OperationAbstract::preCheck ();
   /**
    * @todo en cas d'ajoute des instruction
    * if (!result->status)
    * return result;
    */
   return result;
}
