/*
 * SmdExecption.cpp
 *
 *  Created on: 11 juil. 2012
 *      Author: rli
 */

#include "SmdException.h"

SmdException::SmdException (const char * Msg, const char * file, int line) :
   exception (), message (string ())
{
   this->message.append (fString::format ("%s:%d - ", file, line));
   this->message.append (Msg);
}

SmdException::SmdException (const StationException &e) :
   exception (e), message (e.what ())
{

}

SmdException::~SmdException () throw ()
{

}

const char * SmdException::what () const throw ()
{
   return this->message.c_str ();
}

void SmdException::calledBy (const char * Msg, const char * file, int line)
{
   this->message.append (fString::format ("\n%s:%d - %s", file, line, Msg));
}
