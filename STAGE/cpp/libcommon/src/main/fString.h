/**
 * fString.h
 *
 * $Id: fString.h 617 2012-07-12 08:26:26Z bdm $
 */

#ifndef FSTRING_H_
#define FSTRING_H_

#include <string>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
//#include <sstream>
#include <vector>
#include <sys/ioctl.h>

#include "StationException.h"

#define FD_READ 0
#define FD_WRITE 1

using namespace std;

/**
 * @ingroup commontools
 * Adds format function (aka sprintf) to string. And other string tools
 * @since 21 oct. 2010
 * @author bdm
 */
class fString
{
   public:
      /**
       * Formats a string as sprintf
       * @param fmt format string
       * @return the formated string
       */
      static string format (const string & fmt, ...);

      /**
       * Formats a string as sprintf
       * @param fmt format string
       * @return the formated string
       */
      static string format (const char *fmt, ...);

      /**
       * Formats a string as sprintf
       * @param fmt format string
       * @param ap va_list generated from ... parameter
       * @return the formated string
       */
      static string format (const char *fmt, va_list ap);

      /**
       * Splits a string into tokens. Ignores empty tokens !
       * @param str the string to split
       * @param tokens will be filled with string tokens
       * @param delimiters delimiters list in a string
       */
      static void tokenize (const string& str, vector<string>& tokens, const string& delimiters = " ");

      /**
       * @param dest src filename without extension
       * @param src filename with extension
       */
      static void basename (string & dest, const string & src);

      /**
       * remove useless space characters before and after the string
       * @param str string to trim
       * @return trimed string
       */
      static string trim (const string & str);
};

#endif /* FSTRING_H_ */
