#include "StationException.h"

void StationException::init ()
{
   this->msg.assign ("\timaGeau Exception !\n");
}

StationException::StationException () :
   exception (), msg ("")
{
   init ();
}

StationException::StationException (std::exception & e, const char * func, int line, const char * file, const char * Msg) :
   exception (e), msg (string ())
{
   init ();
   this->msg = this->msg + " caused by exception: " + typeid(e).name () + ": " + e.what ();
   add (func, line, file, Msg);
}

StationException::StationException (int line, const char * func, const char * file, const char * Msg, ...) :
   exception (), msg (string ())
{
   init ();
   va_list varg;
   va_start(varg, Msg);

   add (line, func, file, Msg, varg);

   va_end(varg);
}

StationException & StationException::operator = (std::exception & e)
{
   std::exception::operator = (e);
   init ();
   this->msg = this->msg + " caused by exception: " + typeid(e).name () + ": " + e.what ();
   return *this;
}

StationException & StationException::operator = (StationException & e)
{
   std::exception::operator = (e);
   this->msg = e.msg;
   return *this;
}

StationException::StationException (const char * func, int line, const char * file, const char * Msg) :
   exception (), msg (string ())
{
   init ();
   add (func, line, file, Msg);
}

StationException::StationException (const char * func, int line, const char * file, const string & Msg) :
   exception (), msg (string ())
{
   init ();
   add (func, line, file, Msg);
}

StationException::~StationException () throw ()
{
}

const char * StationException::what () const throw ()
{
   return this->msg.c_str ();
}

void StationException::add (const char * func, int line, const char * file, const char * Msg)
{
   char tmp [MAX_NAME_SIZE];
   (void) func;
   snprintf (tmp, MAX_NAME_SIZE, "%s%s:%d: error: %s\n", (msg.size () ? "\tfrom " : ""), file, line, (Msg == NULL ? "**dispatch**" : Msg));
   msg.insert (0, tmp);
}

void StationException::add (const char * func, int line, const char * file, const string & Msg)
{
   add (func, line, file, Msg.c_str ());
}

void StationException::add (int line, const char * func, const char * file, const char * Msg, va_list varg)
{
   char tmp [MAX_NAME_SIZE];
   char vaStr [MAX_NAME_SIZE >> 1];
   (void) func;
   vsnprintf (vaStr, MAX_NAME_SIZE >> 1, Msg, varg);

   snprintf (tmp, MAX_NAME_SIZE, "%s%s:%d: error: %s\n", (msg.size () ? "\tfrom " : ""), file, line, vaStr);
   msg.insert (0, tmp);
}

// =======================================================
// ============= StationExceptionWarning =================
// =======================================================

StationExceptionWarning::StationExceptionWarning (int line, const char * func, const char * file, const char * Msg) :
   StationException ()
{
   add (line, func, file, Msg);
}

StationExceptionWarning::~StationExceptionWarning () throw ()
{

}

void StationExceptionWarning::add (int line, const char * func, const char * file, const char * Msg)
{
   char tmp [MAX_NAME_SIZE];
   snprintf (tmp, MAX_NAME_SIZE, "Warning %s:%s@%d : \n\t\t%s", file, func, line, (Msg == NULL ? "**dispatch**" : Msg));
   msg.insert (0, tmp);
}
