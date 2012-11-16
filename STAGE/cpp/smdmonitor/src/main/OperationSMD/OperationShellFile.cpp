/*
 * OperationShellFile.cpp
 *
 *  Created on: 24 mai 2012
 *      Author: rli
 */

#include "OperationShellFile.h"
#include  "OperationSMDFunctions.h"

using namespace OperationsSMD;

OperationShellFile::OperationShellFile () :
   OperationAbstract (), action (), filename (), contents (), execute_after (true), delete_after (false)
{

}

OperationShellFile::OperationShellFile (uint32 _id, OperationType& _type, const string& _user, const string& _path, double _state,
   bool _isAchived, bool _isNested, bool report_on_success, ShellFileAction action, const string & filename, const string & contents,
   bool execute, bool delete_after) :
   OperationAbstract (_id, _type, _user, _path, _state, _isAchived, _isNested, report_on_success), action (action), filename (filename), contents (
      contents), execute_after (execute), delete_after (delete_after)
{
}

OperationShellFile::~OperationShellFile ()
{

}
void OperationShellFile::deserialize (json_spirit::Object& json_input)
{
   try {
      this->OperationAbstract::deserialize (json_input);
      this->action = OperationShellFile::shell_file_action_from_string (
         OperationSMDFunctions::searchAttribute (json_input, "action").get_str ());
      this->filename = OperationSMDFunctions::searchAttribute (json_input, "filename").get_str ();
      this->contents = OperationSMDFunctions::searchAttribute (json_input, "contents").get_str ();
      this->execute_after = OperationSMDFunctions::searchAttribute (json_input, "execute_after").get_bool ();
      this->delete_after = OperationSMDFunctions::searchAttribute (json_input, "delete_after").get_bool ();
   }
   catch (SmdException &exc) {
      exc.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }
   catch (runtime_error &rf) {
      throw SmdException (fString::format ("JSON SPIRIT (%s) ", rf.what ()).c_str (), __FILE__, __LINE__);
   }

}

void OperationShellFile::debug ()
{
   OperationAbstract::debug ();
   cout << "Action \t\t: " << OperationShellFile::string_from_shell_file_action (action) << endl;
   cout << "File Name \t: " << filename << endl;
   cout << "Contents \t: " << contents << endl;
   cout << "Execute After \t: " << OperationSMDFunctions::bool_to_string (execute_after) << endl;
   cout << "Delete After \t: " << OperationSMDFunctions::bool_to_string (delete_after) << endl;
}

/*
 * Execute
 */
ReportData* OperationShellFile::runOperation ()
{
   ReportData *result = this->OperationAbstract::runOperation ();
   if (!result->status)
      return result;

   int status = 0;
   //command
   if (result->status && execute_after) {

      string cmd = fString::format ("su %s -c 'bash %s 2>&1'", this->user.c_str (), this->filename.c_str ());

      //Execute the command
      char line [1000];
      FILE *fp;

      fp = popen (cmd.c_str (), "r");
      if (fp == NULL)
         LOG_FATAL(logger, "Popen execute ERROR");

      while (fgets (line, sizeof(line), fp) != NULL) {
         result->contents.append (line);
      }

      status = pclose (fp);
      if (status != 0)
         result->status = false;
   }

   LOG_TRACE(logger, fString::format("Status ----------->  %d \n%s", status, result->contents.c_str()));
   return result;
}

/*
 * Apres l'execution
 */
ReportData* OperationShellFile::postCheck ()
{
   ReportData *result = this->OperationAbstract::postCheck ();
   if (result->status) {
      if (delete_after) {
         string cmd = fString::format ("su %s -c 'rm -f %s' ", this->user.c_str (), this->filename.c_str ());

         if (system (cmd.c_str ()) != 0) {
            result->status = false;
            result->contents = "ERROR OF DELETING FILE ...";
            LOG_FATAL(logger, fString::format("ERROR DELETE FILE %s%s", path.c_str(), filename.c_str()));
         }

         LOG_INFO(logger, fString::format("DELETE FILE %s%s", path.c_str(), filename.c_str()));
      }
   }
   return result;
}

/*
 * Avant d'execution
 */
ReportData* OperationShellFile::preCheck ()
{
   ReportData *result = this->OperationAbstract::preCheck ();
   if (result->status) {

      //Creation de Fichier
      ofstream objetfichier;
      objetfichier.open (filename.c_str (), ios::out);

      if (objetfichier.bad ()) {
         LOG_FATAL(logger, fString::format("ERROR FILE CREATION  %s /%s", path.c_str(), filename.c_str()));
         result->status = false;
         result->contents = " ERREUR REPLACE OR COPY";
      }
      else {

         //Input the contents into File name
         objetfichier << contents << endl;
         objetfichier.close ();

         string cmd = fString::format ("su %s -c 'chmod +x %s'", this->user.c_str (), this->filename.c_str ());

         if (system (cmd.c_str ()) != 0) {
            result->status = false;
            result->contents = "ERREUR DE LA CREATION DE FICHIER ou CONVERTION FOR EXECUTABLE";
            LOG_FATAL(logger, fString::format("ERREUR DE LA CREATION DE FICHIER  %s/%s", path.c_str(), filename.c_str()));
         }
      }
   }
   return result;
}

ShellFileAction OperationShellFile::shell_file_action_from_string (const string & msg)
{
   if (msg == "ShellFileActionCopy")
      return ShellFileCopy;
   else if (msg == "ShellFileActionReplace" || msg == "ShellFileActionCopyOrReplace")
      return ShellFileReplace;
   else
      return ShellFileNothing;
}
string OperationShellFile::string_from_shell_file_action (ShellFileAction type)
{
   switch (type) {
      case ShellFileCopy:
         return "ShellFileActionCopy";

      case ShellFileReplace:
         return "ShellFileActionCopyOrReplace";

      default:
         return "ShellFileActionNothing";
   }
}
