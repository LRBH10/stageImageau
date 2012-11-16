/*
 * OperationSwitch.cpp
 *
 *  Created on: 11 juin 2012
 *      Author: rli
 */

#include "OperationSvnSwitch.h"
#include  "OperationSMDFunctions.h"

using namespace OperationsSMD;

OperationSvnSwitch::OperationSvnSwitch () :
   OperationSvn (), url_switch ()
{

}

OperationSvnSwitch::~OperationSvnSwitch ()
{

}
void OperationSvnSwitch::debug ()
{
   OperationSvn::debug ();
   cout << "URL\t\t: " << url_switch << endl;

}
void OperationSvnSwitch::deserialize (json_spirit::Object &json_input)
{
   try {
      this->OperationSvn::deserialize (json_input);
      this->svn_type = OperationSMDFunctions::OperationTypeFromString (
         OperationSMDFunctions::searchAttribute (json_input, "svn_type").get_str ());
      this->url_switch = OperationSMDFunctions::OperationTypeFromString (
         OperationSMDFunctions::searchAttribute (json_input, "url_switch").get_str ());
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
ReportData* OperationSvnSwitch::runOperation ()
{
   ReportData *result = this->OperationSvn::runOperation ();
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
ReportData* OperationSvnSwitch::postCheck ()
{
   ReportData *result = this->OperationSvn::postCheck ();
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
ReportData* OperationSvnSwitch::preCheck ()
{
   ReportData *result = this->OperationSvn::preCheck ();
   /**
    * @todo en cas d'ajoute des instruction
    * if (!result->status)
    * return result;
    */

   return result;
}
