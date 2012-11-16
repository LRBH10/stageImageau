/*
 * OperationSvnUpdate.cpp
 *
 *  Created on: 11 juin 2012
 *      Author: rli
 */

#include "OperationSvnUpdate.h"
#include  "OperationSMDFunctions.h"

using namespace OperationsSMD;

OperationSvnUpdate::OperationSvnUpdate () :
   OperationSvn ()
{

}

OperationSvnUpdate::OperationSvnUpdate (uint32 _id, OperationType& _type, const string& _user, const string& _path, double _state,
   bool _isAchived, bool _isNested, bool report_on_success, OperationType &svn_type, const string & conflict, const string & head,
   bool forced) :
   OperationSvn (_id, _type, _user, _path, _state, _isAchived, _isNested, report_on_success, svn_type, conflict, head, forced)
{
}

OperationSvnUpdate::~OperationSvnUpdate ()
{

}

void OperationSvnUpdate::debug ()
{
   OperationSvn::debug ();

}

void OperationSvnUpdate::deserialize (json_spirit::Object &json_input)
{
   try {
      this->OperationSvn::deserialize (json_input);
      this->svn_type = OperationSMDFunctions::OperationTypeFromString (
         OperationSMDFunctions::searchAttribute (json_input, "svn_type").get_str ());
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
ReportData* OperationSvnUpdate::runOperation ()
{
   ReportData *result = this->OperationSvn::runOperation ();
   if (result->status) {
      //command
      string cmd = fString::format ("su %s -c 'svn update -r %s", this->user.c_str (), this->head.c_str ());

      if (this->conflict != "") {
         cmd = cmd + " --accept " + this->conflict;
      }

      if (this->forced) {
         cmd = cmd + " --force ";
      }
      cmd = cmd + " 2>&1 '";

      LOG_TRACE(logger, fString::format ("SVN EXCUTION WITH %s ", cmd.c_str ()));
      //Execute the command
      char line [1000];
      FILE *fp;
      fp = popen (cmd.c_str (), "r");
      if (fp == NULL)
         LOG_FATAL(logger, "Popen execute ERROR");

      while (fgets (line, sizeof(line), fp) != NULL) {
         result->contents.append (line);
      }

      int status = pclose (fp);
      if (status != 0) {
         result->status = false;
         result->contents += strerror (status);
      }

      LOG_TRACE(logger, fString::format ("Status ----------->  %d \n%s", status, result->contents.c_str ()));
   }

   return result;
}

/*
 * Apres l'execution
 */
ReportData* OperationSvnUpdate::postCheck ()
{
   ReportData *result = this->OperationSvn::postCheck ();

   if (!result->status) {
      ISmdMonitor * mon = ISmdMonitor::getInstance ();
      mon->sendVersionUpdated ();
   }
   return result;
}

/*
 * Avant d'execution
 */
ReportData* OperationSvnUpdate::preCheck ()
{
   ReportData *result = this->OperationSvn::preCheck ();
   /**
    * @todo en cas d'ajoute des instruction
    * if (!result->status)
    * return result;
    */

   return result;
}

