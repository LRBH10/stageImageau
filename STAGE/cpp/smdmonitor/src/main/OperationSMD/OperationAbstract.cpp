/*
 * OperationAbstract.cpp
 *
 *  Created on: 23 mai 2012
 *      Author: rli
 */

#include "OperationAbstract.h"

using namespace OperationsSMD;

LoggerPtr OperationAbstract::logger (LogFactory::getLogger ("OperationAbstract"));

OperationAbstract::OperationAbstract () :
   id (-1), type (OperationTypeInconnu), user (""), path (""), state (-1.0), achieved (false), nested (false), report_on_success (false), inTest (
      false)
{
}

/*
 *
 */
OperationAbstract::OperationAbstract (uint32 _id, OperationType _type, const string& _user, const string& _path, double _state,
   bool _isAchived, bool _isNested, bool report_on_success) :
   id (_id), type (_type), user (_user), path (_path), state (_state), achieved (_isAchived), nested (_isNested), report_on_success (
      report_on_success), inTest (false)
{
}

/*
 *
 */
OperationAbstract::~OperationAbstract ()
{
   // TODO Auto-generated destructor stub
}

/*
 * Debug Operations
 */

void OperationAbstract::debug ()
{
   cout << "id \t\t: " << id << endl;
   cout << "type \t\t: " << OperationSMDFunctions::OperationTypeToString (type) << endl;
   cout << "path \t\t: " << path << endl;
   cout << "user \t\t: " << user << endl;
   cout << "state \t\t: " << state << endl;
   cout << "Nested \t\t: " << OperationSMDFunctions::bool_to_string (nested) << endl;
   cout << "Achieved \t: " << OperationSMDFunctions::bool_to_string (achieved) << endl;
   cout << "Report On Success: " << OperationSMDFunctions::bool_to_string (report_on_success) << endl;
}
/*
 * Converter JSON to Operation c++
 */
void OperationAbstract::deserialize (json_spirit::Object &json_input)
{
   try {
      this->id = OperationSMDFunctions::searchAttribute (json_input, "id").get_int64 ();
      this->type = OperationSMDFunctions::OperationTypeFromString (OperationSMDFunctions::searchAttribute (json_input, "type").get_str ());
      this->path = OperationSMDFunctions::searchAttribute (json_input, "path").get_str ();
      this->user = OperationSMDFunctions::searchAttribute (json_input, "user").get_str ();
      this->state = OperationSMDFunctions::searchAttribute (json_input, "state").get_real ();
      this->achieved = OperationSMDFunctions::searchAttribute (json_input, "achieved").get_bool ();
      this->nested = OperationSMDFunctions::searchAttribute (json_input, "nested").get_bool ();
      this->report_on_success = OperationSMDFunctions::searchAttribute (json_input, "report_on_success").get_bool ();
      //try {
   }
   catch (SmdException &exc) {
      exc.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }
   catch (runtime_error &rf) {
      throw SmdException (fString::format ("JSON SPIRIT (%s) ", rf.what ()).c_str (), __FILE__, __LINE__);
   }
}

/*
 * Execute
 */
ReportData* OperationAbstract::execute ()
{
   LOG_INFO(logger,
      fString::format (" %s \t %ld \t %s\t %s", OperationSMDFunctions::OperationTypeToString (type).c_str (), id, path.c_str (), user.c_str ()));

   ReportData *result = new ReportData (this->id);

   if (!inTest) {
      ISmdMonitor::getInstance ()->sendOperationProgress (this->id, 25);
   }

   ReportData * tmpReport = preCheck ();
   result->append (tmpReport);
   delete tmpReport;

   if (result->status) {
      if (!inTest) {
         ISmdMonitor::getInstance ()->sendOperationProgress (this->id, 30);
      }
      tmpReport = runOperation ();
      result->append (tmpReport);
      delete tmpReport;
   }

   if (result->status) {
      if (!inTest) {
         ISmdMonitor::getInstance ()->sendOperationProgress (this->id, 80);
      }
      tmpReport = postCheck ();
      result->append (tmpReport);
      delete tmpReport;

   }

   if (result->status) {
      if (!this->report_on_success) {
         result->contents = "OK!!";
      }

      achieved = true;
      if (!inTest) {
         ISmdMonitor::getInstance ()->sendOperationProgress (this->id, 100);
      }
   }
   else {
      achieved = false;
      if (!inTest) {
         ISmdMonitor::getInstance ()->sendOperationProgress (this->id, 0);
      }

   }

   if (!inTest) {
      ISmdMonitor::getInstance ()->sendReport (result);
   }

   return result;
}

/*
 * Before d'execution
 */
ReportData* OperationAbstract::preCheck ()
{
   ReportData *result = new ReportData (this->id, true, "PRE CHECK ...\n");

   LOG_TRACE(logger, fString::format ("%d : PRE CHECK", this->id));
   string cmd = fString::format ("su %s -c 'cd %s'", this->user.c_str (), this->path.c_str ());

   if (system (cmd.c_str ()) != 0) {
      result->status = false;
      result->contents = "ERROR DIRECTORY OR USER DOES NOT EXIST " + cmd;
      LOG_FATAL(logger, fString::format ("ERROR DIRECTORY OR USER DOES NOT EXIST %s", cmd.c_str ()));
   }
   else {
      chdir (path.c_str ());
   }
   return result;
}

/*
 * Run The operation
 */
ReportData* OperationAbstract::runOperation ()
{
   LOG_TRACE(logger, fString::format ("%d : RUN OPERATION", this->id));
   return new ReportData (this->id, true, "RUN OPERATION ...\n");
}

/*
 * After l'execution
 */
ReportData* OperationAbstract::postCheck ()
{
   LOG_TRACE(logger, fString::format ("%d : POST CHECK", this->id));
   return new ReportData (this->id, true, "POST_CHECK ...\n");
}

long OperationAbstract::getIdOperation ()
{
   return id;
}
/*
 * Get Type
 */
OperationType OperationAbstract::getTypeOperation ()
{
   return type;
}

bool OperationAbstract::isReportOnSuccess ()
{
   return report_on_success;
}

bool OperationAbstract::isAchieved ()
{
   return achieved;
}

void OperationAbstract::setInTest (bool test)
{
   inTest = test;
}
