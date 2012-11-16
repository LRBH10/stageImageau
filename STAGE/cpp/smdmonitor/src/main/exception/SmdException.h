/*
 * SmdExecption.h
 *
 *  Created on: 11 juil. 2012
 *      Author: rli
 */

#ifndef SMDEXECPTION_H_
#define SMDEXECPTION_H_
#include <iostream>
#include <exception>
#include "fString.h"
#include "StationException.h"

using namespace std;

class SmdException: public std::exception
{
   public:
      /**
       * Constructor with all parameters
       * @param message Description of the exception
       * @param file    File where the exception where detected
       * @param line    Line where the exception where detected
       */
      SmdException (const char * message, const char * file, int line);

      SmdException (const StationException &e);

      /**
       * Show the exception
       * @return @ref SmdException::message converted in const char *
       */
      virtual const char * what () const throw ();

      /**
       * Default destructor
       */
      virtual ~SmdException () throw ();

      /**
       * To push the exception in the stack
       * @param message  Description of the exception
       * @param file     File where the exception where detected
       * @param line     Line where the exception where detected
       */
      void calledBy (const char * message, const char * file, int line);

   private:
      string message;

};

#endif /* SMDEXECPTION_H_ */
