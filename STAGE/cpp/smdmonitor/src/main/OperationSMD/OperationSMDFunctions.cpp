/*
 * OperationSMDFunctions.cpp
 *
 *  Created on: 24 mai 2012
 *      Author: rli
 */

#include "OperationSMDFunctions.h"
#include "OperationSvnUpdate.h"
#include "OperationSvnSwitch.h"

using namespace OperationsSMD;

class OperationSvnUpdate;
class OperationSvnSwitch;

LoggerPtr OperationSMDFunctions::logger (LogFactory::getLogger ("OperationSMDFunctions"));

OperationAbstract* OperationSMDFunctions::OperationFactory (json_spirit::Object json_input)
{
   OperationType type = OperationTypeFromString (searchAttribute (json_input, "type").get_str ());
   OperationType svn_type;

   OperationAbstract *res;

   try {
      switch (type) {
         case OperationTypeScript:
            res = new OperationScript ();
            res->deserialize (json_input);
            break;

         case OperationTypeMulti:
            res = new OperationMulti ();
            res->deserialize (json_input);
            break;

         case OperationTypeShellFile:
            res = new OperationShellFile ();
            res->deserialize (json_input);
            break;

         case OperationTypeSvn:
            svn_type = OperationTypeFromString (searchAttribute (json_input, "svn_type").get_str ());
            switch (svn_type) {
               case OperationTypeSvnUpdate:
                  res = new OperationSvnUpdate ();
                  res->deserialize (json_input);
                  break;

               case OperationTypeSvnSwitch:
                  res = new OperationSvnSwitch ();
                  res->deserialize (json_input);
                  break;

               default:
                  res = new OperationSvn ();
                  res->deserialize (json_input);
                  break;
            }
            break;

         default:
            res = new OperationAbstract ();
            res->deserialize (json_input);
            break;
      }
   }
   catch (SmdException &exc) {
      exc.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }

   return res;
}

OperationType OperationSMDFunctions::OperationTypeFromString (const string & type_operation)
{
   if (type_operation == "OperationAbstract")
      return OperationTypeAbstract;

   else if (type_operation == "OperationMulti")
      return OperationTypeMulti;

   else if (type_operation == "OperationScript")
      return OperationTypeScript;

   else if (type_operation == "OperationShellFile")
      return OperationTypeShellFile;

   else if (type_operation == "OperationSvn")
      return OperationTypeSvn;

   else if (type_operation == "OperationSvnUpdate")
      return OperationTypeSvnUpdate;

   else if (type_operation == "OperationSvnSwitch")
      return OperationTypeSvnSwitch;

   return OperationTypeInconnu;
}

string OperationSMDFunctions::OperationTypeToString (OperationType type_operation)
{
   string res = "";

   switch (type_operation) {
      case OperationTypeAbstract:
         res = "OperationAbstract";
         break;

      case OperationTypeScript:
         res = "OperationScript";
         break;

      case OperationTypeMulti:
         res = "OperationMulti";
         break;

      case OperationTypeShellFile:
         res = "OperationShellFile";
         break;

      case OperationTypeSvnUpdate:
         res = "OperationSvnUpdate";
         break;

      case OperationTypeSvnSwitch:
         res = "OperationSvnSwitch";
         break;

      case OperationTypeSvn:
         res = "OperationSVN";
         break;

      default:
         res = "OperationInconnu";
         break;
   }
   return res;
}

json_spirit::Value OperationSMDFunctions::searchAttribute (json_spirit::Object &obj, const string & val)
{
   json_spirit::Value returnedValue;

   for (json_spirit::Object::size_type i = 0; i < obj.size (); i++)
      if (obj.at (i).name_.compare (val) == 0) {
         returnedValue = obj.at (i).value_;
         i = obj.size ();
      }

   if (returnedValue.is_null ()) {
      LOG_WARN(logger, fString::format("The Attribute with name '%s' is NULL",val.c_str()));
      LOG_WARN(logger, "FOR DEFAULT THAT WILL BE CASTED INTO STRING EMPTY");
      returnedValue = json_spirit::Value ("");
   }
   return returnedValue;
}

bool OperationSMDFunctions::isOperationTypeOf (OperationAbstract *op, OperationType type_operation)
{
   bool res = false;
   switch (type_operation) {
      case OperationTypeAbstract:
         res = true;
         break;

      case OperationTypeScript:
         if (dynamic_cast<OperationScript *> (op) != NULL)
            res = true;
         break;

      case OperationTypeMulti:
         if (dynamic_cast<OperationMulti *> (op) != NULL)
            res = true;
         break;

      case OperationTypeShellFile:
         if (dynamic_cast<OperationShellFile *> (op) != NULL)
            res = true;
         break;

      case OperationTypeSvnUpdate:
         if (dynamic_cast<OperationSvnUpdate *> (op) != NULL)
            res = true;
         break;

      case OperationTypeSvnSwitch:
         if (dynamic_cast<OperationSvnSwitch *> (op) != NULL)
            res = true;
         break;

      case OperationTypeSvn:
         if (dynamic_cast<OperationSvn *> (op) != NULL)
            res = true;
         break;

      default:
         break;
   }

   return res;
}

string OperationSMDFunctions::bool_to_string (bool var)
{
   if (var)
      return "true";
   else
      return "false";
}
