#include "OptionParser.h"

LoggerPtr OptionParser::logger (LogFactory::getLogger ("OptionParser"));

OptionParser::OptionParser (const list<OptionQuery> & optQ) :
   options ()
{
   for (list<OptionQuery>::const_iterator ite = optQ.begin (); ite != optQ.end (); ++ite) {
      this->options [*ite] = OptionResult ();
   }
}

OptionParser::~OptionParser ()
{
}

void OptionParser::readOptions (int argc, char ** argv)
{
   int i = 1;
   OptionMap::iterator optIte;
   while (i < argc) {
      if (argv [i] != NULL && argv [i] [0] == '-') {
         int pos = 1;
         string optName;
         while (argv [i] [pos] == '-') {
            ++pos;
         }
         optName = string (argv [i]).substr (pos);
         optIte = options.find (optName);
         if (optIte != options.end ()) {
            if (optIte->first.needParam) {
               if (i + 1 < argc && argv [i + 1] [0] != '-') {
                  string tmpVal = "";
                  int j = 0;
                  while (i + 1 < argc && argv [i + 1] [0] != '-') {
                     LOG_DEBUG(OptionParser::logger,
                        fString::format ("will add '%s' to parameter value '%s'", argv [i + 1], tmpVal.c_str ()));
                     if (j > 0) {
                        tmpVal += " ";
                     }
                     tmpVal += argv [i + 1];
                     ++i;
                     ++j;
                  }

                  LOG_DEBUG(OptionParser::logger, fString::format ("Save parameter value '%s'", tmpVal.c_str ()));
                  options [optName].set (tmpVal);
               }
               else {
                  THROW_SE(fString::format ("option '%s' is waiting a parameter !", optName.c_str ()));
               }
            }
            else {
               options [optName].set ();
            }
         }
         else {
            LOG_WARN(OptionParser::logger, fString::format ("Unknown param '%s'", optName.c_str ()));
         }
      }
      else if (i == 1) {
         optIte = options.find (string ("c"));
         if (optIte != options.end ()) {
            options [string ("c")].set (argv [i]); // default is conf name (for compatibility)
         }
         else {
            THROW_SE(fString::format ("Malformed option '%s' (num %d). Valid pattern is '-opt [param]'.", argv [i], i));
         }
      }
      else {
         THROW_SE(fString::format ("Malformed option num %d. Valid pattern is '-opt [param]'.", i));
      }
      ++i;
   }

   bool err = false;
   for (OptionMap::iterator ite = options.begin (); ite != options.end (); ++ite) {
      if (!ite->first.isOptional && !ite->second.isSet) {
         LOG_WARN(OptionParser::logger, fString::format ("Option '%s' is needded!", ite->first.name.c_str ()));
         err = true;
      }
   }
   if (err) {
      THROW_SE("Missing options !");
   }
}

string OptionParser::listOptions () const
{
   string out;
   for (OptionMap::const_iterator ite = options.begin (); ite != options.end (); ++ite) {
      if (ite->first.isOptional) {
         out += "[";
      }
      if (ite->first.needParam) {
         out += fString::format ("-%s value", ite->first.name.c_str ());
      }
      else {
         out += fString::format ("-%s", ite->first.name.c_str ());
      }
      if (ite->first.isOptional) {
         out += "]";
      }
      out += " ";
   }
   return out;
}

string OptionParser::listOptionDetails () const
{
   string out;
   for (OptionMap::const_iterator ite = options.begin (); ite != options.end (); ++ite) {
      out += fString::format ("\t -%s \t %s \n\t\t   needParam:%s, optional:%s \n", //
         ite->first.name.c_str (), ite->first.doc.c_str (), (ite->first.needParam ? "yes" : "no"), (ite->first.isOptional ? "yes" : "no"));
   }
   return out;
}

OptionMap::const_iterator OptionParser::get (const string & optName) const
{
   return this->options.find (optName);
}

OptionMap::iterator OptionParser::get (const string & optName)
{
   return this->options.find (optName);
}

bool OptionParser::isSet (const string & optName) const
{
   bool out;
   if (this->options.find (optName) != this->options.end ()) {
      out = this->options.find (optName)->second.isSet;
   }
   else {
      out = false;
   }
   return out;
}
