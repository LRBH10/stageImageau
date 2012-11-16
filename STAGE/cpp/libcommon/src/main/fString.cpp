#include "fString.h"

string fString::format (const char *fmt, ...)
{
   va_list vl;
   va_start(vl, fmt);
   string out = fString::format (fmt, vl);
   va_end(vl);
   return out;
}

string fString::format (const string & fmt, ...)
{
   va_list vl;
   va_start(vl, fmt);
   string out = fString::format (fmt.c_str (), vl);
   va_end(vl);
   return out;
}

string fString::format (const char *fmt, va_list ap)
{
   char *buf = NULL;
   int needed = -1;

   needed = vasprintf (&buf, fmt, ap); // vasprint auto allocate the buffer

   string out (buf, (size_t) needed);

   free (buf);
   return out;
}

void fString::tokenize (const string& str, vector<string>& tokens, const string& delimiters)
{
   // Skip delimiters at beginning.
   string::size_type charPos = str.find_first_not_of (delimiters, 0);
   // Find first "non-delimiter".
   string::size_type delimPos = str.find_first_of (delimiters, charPos);
   string::size_type tokenSize;
   string token;

   while (string::npos != delimPos && string::npos != charPos) {
      // Found a token, add it to the vector.
      tokenSize = delimPos - charPos;
      token = str.substr (charPos, tokenSize);
      tokens.push_back (token);

      // Skip delimiters.  Note the "not_of"
      charPos = str.find_first_not_of (delimiters, delimPos);
      // Find next "non-delimiter"
      delimPos = str.find_first_of (delimiters, charPos);
      if (delimPos >= str.size ()) {
         delimPos = string::npos;
      }
   }

   if (charPos != string::npos) {
      token = str.substr (charPos);
      tokens.push_back (token);
   }
}

void fString::basename (string & dest, const string & src)
{
   size_t s = src.length ();
   size_t i;

   for (i = 0; i < s && src [i] != '.'; ++i) {
      dest [i] = src [i];
   }
   dest [i] = '\0';
}

string fString::trim (const string & str)
{
   string tmp;

   int start = 0;
   int length = str.length () - 1;
   string badChar = " \t\r\n";

   while (length >= 0 && start < length && badChar.find (str [start]) != string::npos) {
      ++start;
   }

   while (length >= 0 && badChar.find (str [length]) != string::npos) {
      --length;
   }

   tmp = str.substr (start, length - start + 1);

   return tmp;
}
