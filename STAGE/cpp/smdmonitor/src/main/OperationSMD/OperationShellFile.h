/*
 * OperationShellFile.h
 *
 *  Created on: 24 mai 2012
 *      Author: rli
 */

#ifndef OperationShellFile_H_
#define OperationShellFile_H_

#include "OperationAbstract.h"
#include "unistd.h"

namespace OperationsSMD
{

   /**
    * Operation create a file shell and execute it
    */
   class OperationShellFile: public OperationsSMD::OperationAbstract
   {
      public:
         OperationShellFile ();
         OperationShellFile (uint32 _id, OperationType& _type, const string& _user, const string& _path, double _state, bool _isAchived,
            bool _isNested, bool report_on_success, ShellFileAction action, const string & filename, const string & contents, bool execute,
            bool delete_after);
         virtual ~OperationShellFile ();

         virtual void deserialize (json_spirit::Object & json_input);
         virtual void debug ();

         /**
          * SHELLFILE String To Enum
          */
         static ShellFileAction shell_file_action_from_string (const string & type);
         static string string_from_shell_file_action (ShellFileAction type);

      protected:
         virtual ReportData* runOperation ();
         virtual ReportData* postCheck ();
         virtual ReportData* preCheck ();

      private:
         ShellFileAction action;
         string filename;
         string contents;
         bool execute_after;
         bool delete_after;
   };

} /* namespace OperationsSMD */
#endif /* OperationShellFile_H_ */
