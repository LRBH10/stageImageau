/*
 * OperationScript.cpp
 *
 *  Created on: 24 mai 2012
 *      Author: rli
 */

#include "OperationScript.h"
#include "OperationSMDFunctions.h"
using namespace OperationsSMD;

OperationScript::OperationScript () :
   OperationAbstract (), command (), parameters ()
{

}

OperationScript::OperationScript (uint32 _id, OperationType& _type, const string& _user, const string& _path, double _state,
   bool _isAchived, bool _isNested, bool report_on_success, const string & command, const string & parameters) :
   OperationAbstract (_id, _type, _user, _path, _state, _isAchived, _isNested, report_on_success), command (command), parameters (
      parameters)
{
}

OperationScript::~OperationScript ()
{

}
void OperationScript::deserialize (json_spirit::Object &json_input)
{
   try {
      this->OperationAbstract::deserialize (json_input);
      this->command = OperationSMDFunctions::searchAttribute (json_input, "command").get_str ();
      this->parameters = OperationSMDFunctions::searchAttribute (json_input, "parameters").get_str ();
   }
   catch (SmdException &exc) {
      exc.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }
   catch (runtime_error &rf) {
      throw SmdException ("JSON SPIRIT", __FILE__, __LINE__);
   }

}

void OperationScript::debug ()
{
   OperationAbstract::debug ();
   cout << "command \t: " << command << endl;
   cout << "Parameters \t: " << parameters << endl;

}
/*
 * Execute
 */
ReportData* OperationScript::runOperation ()
{
   ReportData *result = this->OperationAbstract::runOperation ();

   if (result->status) {
      //command
      string cmd = fString::format ("su %s -c '%s %s 2>&1'", this->user.c_str (), this->command.c_str (), this->parameters.c_str ());

      //Execute the command
      char line [1000];
      FILE *fp;
      fp = popen (cmd.c_str (), "r");
      if (fp == NULL) {
         LOG_FATAL(logger, "Popen execute ERROR");
      }

      while (fgets (line, sizeof(line), fp) != NULL) {
         result->contents.append (line);
      }

      int status = pclose (fp);
      if (status != 0) {
         result->status = false;
      }

      LOG_TRACE(logger, fString::format("Status ----------->  %d \n%s", status, result->contents.c_str()));

   }
   return result;
}

/*
 * Apres l'execution
 */
ReportData* OperationScript::postCheck ()
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
ReportData* OperationScript::preCheck ()
{
   ReportData *result = this->OperationAbstract::preCheck ();
   /**
    * @todo en cas d'ajoute des instruction
    * if (!result->status)
    * return result;
    */

   return result;
}
