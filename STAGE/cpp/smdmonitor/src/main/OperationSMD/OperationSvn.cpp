/*
 * OperationSvn.cpp
 *
 *  Created on: 11 juin 2012
 *      Author: rli
 */

#include "OperationSvn.h"
#include "OperationSMDFunctions.h"

using namespace OperationsSMD;

OperationSvn::OperationSvn () :
   OperationAbstract (), svn_type (OperationTypeSvn), head ("HEAD"), conflict ("base"), forced (false)
{

}

OperationSvn::OperationSvn (uint32 _id, OperationType& _type, const string& _user, const string& _path, double _state, bool _isAchived,
   bool _isNested, bool report_on_success, OperationType &svn_type, const string &conflict, const string & head, bool forced) :
   OperationAbstract (_id, _type, _user, _path, _state, _isAchived, _isNested, report_on_success), svn_type (svn_type), head (head), conflict (
      conflict), forced (forced)
{
}

OperationSvn::~OperationSvn ()
{

}

void OperationSvn::debug ()
{
   OperationAbstract::debug ();
   cout << "SVN TYPE\t: " << OperationSMDFunctions::OperationTypeToString (svn_type) << endl;
   cout << "HEAD\t\t: " << head << endl;
   cout << "Conflict\t: " << OperationSvn::getConflictFromString (conflict) << endl;
   cout << "FORCED\t\t: " << OperationSMDFunctions::bool_to_string (forced) << endl;

}

void OperationSvn::deserialize (json_spirit::Object& json_input)
{
   try {
      this->OperationAbstract::deserialize (json_input);
      this->head = OperationSMDFunctions::searchAttribute (json_input, "head").get_str ();
      this->conflict = OperationSvn::getConflictFromString (OperationSMDFunctions::searchAttribute (json_input, "conflict").get_str ());
      this->forced = OperationSMDFunctions::searchAttribute (json_input, "forced").get_bool ();
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
ReportData* OperationSvn::runOperation ()
{
   ReportData *result = this->OperationAbstract::runOperation ();
   /**
    * @todo en cas d'ajoute des instruction
    * if (!result->status)
    * return result;
    */
   return result;
}

/*
 * Apres l'execution
 */
ReportData* OperationSvn::postCheck ()
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
 * Avant d'execution
 */
ReportData* OperationSvn::preCheck ()
{
   ReportData *result = this->OperationAbstract::preCheck ();
   if (result->status) {
      //
      string cmd = fString::format ("su %s -c 'svn status -q'", this->user.c_str ());

      //Execute the command
      char line [1000];
      FILE *fp;

      fp = popen (cmd.c_str (), "r");
      if (fp == NULL) {
         LOG_FATAL(logger, "Popen execute ERROR");
      }

      while (fgets (line, sizeof(line), fp) != NULL) {
         string inter = line;
         result->contents.append (inter);
         result->status = OperationSvn::svn_line_validator (inter);
      }

      int status = pclose (fp);
      if (status != 0) {
         result->status = false;
         result->contents = "ERREUR STATUS";
         LOG_INFO(logger, fString::format ("ERREUR STATUS %s%s", path.c_str (), cmd.c_str ()));
      }
   }
   return result;
}

OperationType OperationSvn::getSvnType ()
{
   return svn_type;
}

string OperationSvn::getConflictFromString (const string & msg)
{
   if (msg == "SvnAcceptActionMINE")
      return "mine-full";

   else if (msg == "SvnAcceptActionTHEIRS")
      return "theirs-full";

   else
      return "";

}

bool OperationSvn::svn_line_validator (const string &line)
{
   bool out = true;
   size_t i = 0;
   if (line.at (i) == 'C') {
      LOG_FATAL(logger, fString::format("CONFLIT DETECTED at  %s", line.c_str()));
      out = false;
   }
   if (line.at (i) == 'M') {
      LOG_FATAL(logger, fString::format("FILE MODIFIED at  %s", line.c_str()));
      out = false;
   }

   return out;
}

bool OperationSvn::version_line_getter (const string &line, string &version)
{
   bool result = false;
   istringstream iss (line);
   while (iss) {
      string sub;
      iss >> sub;
      if (sub == "Revision:") {
         LOG_INFO(logger, fString::format("VERSION DETECTED ON LINE  %s", line.c_str()));
         iss >> version;
         result = true;
      }
   }

   return result;
}

