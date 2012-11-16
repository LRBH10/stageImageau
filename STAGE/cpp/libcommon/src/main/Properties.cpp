#include "Properties.h"

using namespace imaGeau;

Properties::Properties () :
   properties ()
{
   // nop
}

Properties::Properties (const Properties & p) :
   properties (p.properties)
{
   // nop
}

Properties::Properties (const string & fileName) :
   properties ()
{
   loadFromFile (fileName);
}

void Properties::loadFromFile (const string & fileName)
{
   vector<string> tokens;
   string line, key, value, tmp, tmp2;

   FILE * fdev = fopen (fileName.c_str (), "r");
   CALL_IF0_MSG(fdev, fString::format ("Cannot open file '%s'", fileName.c_str ()));

   this->properties.clear ();
   try {
      while (!feof (fdev)) {
         IOTools::readValidLine (fdev, line);
         if (!line.empty ()) {
            tokens.clear ();
            fString::tokenize (line, tokens, "=");
            tmp = tokens [0];
            key = fString::trim (tmp);
            if (!key.empty ()) {
               tmp = "";
               for (uint32 i = 1; i < tokens.size (); ++i) {
                  if (i > 1) {
                     tmp += '=';
                  }
                  tmp += tokens [i];
               }
               value = fString::trim (tmp);
               //               fprintf (stderr, "key: '%s' %u/%u \t\t\t \n", key.c_str (), (uint32) key.size (), (uint32) strlen (key.c_str ()));
               //               fprintf (stderr, "value: '%s' s:%u/strl:%u \n", value.c_str (), (uint32) value.size (), (uint32) strlen (value.c_str ()));
               //               //               CALL_IF0_MSG (key.size () == strlen (key.c_str ()), "key size failure!");
               //               //               CALL_IF0_MSG (value.size () == strlen (value.c_str ()), "value size failure!");
               //               //               CALL_IF0_MSG (line.size () == strlen (line.c_str ()), "line size failure!");
               //               //               CALL_IF0_MSG (tmp.size () == strlen (tmp.c_str ()), "tmp size failure!");
               //               fprintf (stderr, "next line ...\n");
               //               fflush (stderr);
               properties [key] = value;
            }
         }
      }
   }
   catch (...) {
      fclose (fdev);
      throw;
   }

   fclose (fdev);
}

Properties::~Properties ()
{
   // nop
}

string Properties::get (const string & key, bool optional, const string & defVal) const
{
   string out = "";
   map<string, string>::const_iterator p = this->properties.find (key);
   if (p != this->properties.end ()) {
      out.assign ((*p).second);
   }
   else if (optional) {
      out.assign (defVal);
   }
   else {
      THROW_SE(fString::format ("Key '%s' not found !", key.c_str ()));
   }
   return out;
}

int Properties::getInt (const string & key, bool optional, int defVal) const
{
   int out = 0;
   map<string, string>::const_iterator p = this->properties.find (key);
   if (p != this->properties.end ()) {
      string val = (*p).second;
      if (val [0] == 'x' || val [0] == 'X' || val [1] == 'x' || val [1] == 'X') {
         out = strtol (val.c_str (), NULL, 16);
      }
      else {
         out = atoi (val.c_str ());
      }
   }
   else if (optional) {
      out = defVal;
   }
   else {
      THROW_SE(fString::format ("Key '%s' not found !", key.c_str ()));
   }
   return out;
}

double Properties::getDouble (const string & key, bool optional, double defVal) const
{
   double out = 0.0;
   map<string, string>::const_iterator p = this->properties.find (key);
   if (p != this->properties.end ()) {
      string val = (*p).second;
      out = atof (val.c_str ());
   }
   else if (optional) {
      out = defVal;
   }
   else {
      THROW_SE(fString::format ("Key '%s' not found !", key.c_str ()));
   }
   return out;
}

bool Properties::getBoolean (const string & key, bool optional, bool defVal) const
{
   bool out = false;
   map<string, string>::const_iterator p = this->properties.find (key);
   if (p != this->properties.end ()) {
      string val = (*p).second;
      if (val.compare ("yes") == 0 || val.compare ("YES") == 0 || val.compare ("true") == 0 || val.compare ("TRUE") == 0
         || val.compare ("1") == 0) {
         out = true;
      }
   }
   else if (optional) {
      out = defVal;
   }
   else {
      THROW_SE(fString::format ("Key '%s' not found !", key.c_str ()));
   }
   return out;
}

void Properties::add (const string & key, const string & value)
{
   this->properties [key] = value;
}

void Properties::add (const Properties & prop)
{
   this->properties.insert (prop.properties.begin (), prop.properties.end ());
}
