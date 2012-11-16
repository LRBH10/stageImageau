/**
 * StationException.h
 *
 * $Id: StationException.h 373 2012-03-02 14:10:15Z bdm $
 */

#ifndef STATIONEXCEPTION_H_
#define STATIONEXCEPTION_H_

#include <exception>
#include <cassert>
//#include <streambuf>
#include "commtype.h"
#include <cstdarg>
#include <typeinfo>
#include <cstdio>

using namespace std;

#ifdef _WIN32
#  pragma warning(disable:4996)
#endif

/**
 * @ingroup commontools
 * @defgroup exception Exception management
 *
 * @brief Classes and macros used in this program.
 *
 * Replace statusgrp.
 *
 * @version $Id: StationException.h 373 2012-03-02 14:10:15Z bdm $
 * @author bdm
 * @since 09/2005
 */
//@{
#ifndef __FUNC__
#  ifdef _WIN32
#    define __FUNC__ __FUNCTION__
#  endif
#  ifdef _LINUX_
#    define __FUNC__ __ASSERT_FUNCTION
#  endif
#  ifdef _CYGWIN_
#    define __FUNC__ __PRETTY_FUNCTION__
#  endif
#endif

/**
 * Class for fatal error exception using a stack trace.
 * @sa THROW_SE, ADD_SE
 */
class StationException: public std::exception
{
   protected:
      string msg;

      void init ();

   public:
      /** Default constructor */
      StationException ();

      /**
       * Copy constructor
       * @param e root exception to copy
       * @param func function name where exception was thrown
       * @param line line number where exception was thrown
       * @param file file name where exception was thrown
       * @param Msg to add, can be NULL
       */
      StationException (std::exception & e, const char * func, int line, const char * file, const char * Msg = NULL);

      /**
       * Full constructor with variable parameters
       * @param line line number where exception was thrown
       * @param func function name where exception was thrown
       * @param file file name where exception was thrown
       * @param Msg message to add in the stack trace
       * @param ... argument to Msg
       */
      StationException (int line, const char * func, const char * file, const char * Msg, ...);

      StationException & operator = (std::exception & e);

      StationException & operator = (StationException & e);

      /**
       * Full constructor with variable parameters
       * @param line line number where exception was thrown
       * @param func function name where exception was thrown
       * @param file file name where exception was thrown
       * @param Msg message to add in the stack trace, can be NULL
       */
      StationException (const char * func, int line, const char * file, const char * Msg = NULL);

      /**
       * Full constructor with variable parameters
       * @param line line number where exception was thrown
       * @param func function name where exception was thrown
       * @param file file name where exception was thrown
       * @param Msg message to add in the stack trace, can be NULL
       */
      StationException (const char * func, int line, const char * file, const string & Msg);

      /** Default destructor */
      virtual ~StationException () throw ();

      /** @return the stack trace */
      virtual const char * what () const throw ();

      /**
       * Add a message to the stack trace
       * @param func function name where exception was thrown
       * @param line line number where exception was thrown
       * @param file file name where exception was thrown
       * @param Msg message to add in the stack trace, can be NULL
       */
      virtual void add (const char * func, int line, const char * file, const char * Msg = NULL);

      /**
       * Add a message to the stack trace
       * @param func function name where exception was thrown
       * @param line line number where exception was thrown
       * @param file file name where exception was thrown
       * @param Msg message to add in the stack trace, can be NULL
       */
      virtual void add (const char * func, int line, const char * file, const string & Msg);

      /**
       * Add a message to the stack trace with variable parameters
       * @param line line number where exception was thrown
       * @param func function name where exception was thrown
       * @param file file name where exception was thrown
       * @param Msg message to add in the stack trace
       * @param varg argument to Msg
       */
      virtual void add (int line, const char * func, const char * file, const char * Msg, va_list varg);
};

/**
 * Class for warning exception using a stack trace.
 * @sa THROW_SE
 * @todo not finished
 */
class StationExceptionWarning: public StationException
{
   public:
      /** Default constructor */
      StationExceptionWarning (int line, const char * func, const char * file, const char * Msg = NULL);

      /** Destructor */
      virtual ~StationExceptionWarning () throw ();

      /**
       * Add a message to the stack trace
       * @param line line number where exception was thrown
       * @param func function name where exception was thrown
       * @param file file name where exception was thrown
       * @param Msg message to add in the stack trace, can be NULL
       */
      virtual void add (int line, const char * func, const char * file, const char * Msg = NULL);
};

/** throw a fatal exception with message _msg */
#define THROW_SE(_msg) throw StationException (__FUNC__, __LINE__, __FILE__, _msg)

/** throw a fatal exception with message _msg */
#define THROW_SE_TYPE(_msg,_type) throw _type (__FUNC__, __LINE__, __FILE__, _msg)

/** throw a fatal exception with message _msg from std::exception */
#define THROW_SE_STD(_e,_msg) throw StationException (_e, __FUNC__, __LINE__, __FILE__, _msg)

/** throw an exception with message _FMT and variable parameters */
#define THROW_SE_FMT(_FMT,...) throw StationException (__LINE__, __FUNC__, __FILE__, _FMT, __VA_ARGS__)

/** throw a warning exception with message _msg */
#define THROW_SEW(_msg) throw StationExceptionWarning (__LINE__, __FUNC__, __FILE__, _msg)

/** add a message _msg to the stack trace of exception _e */
#define SE_ADD(_e,_msg) _e.add (__FUNC__, __LINE__, __FILE__, _msg)

/** Executes __exp expression and throws an exception with __msg if the __exp returns 0 or false  */
#define CALL_IF0_MSG(__exp,__msg)                   \
   if (!(__exp)) { THROW_SE (__msg); }                \
   else { (void)0; }

/** Executes __exp expression and throws an exception with __msg if the __exp returns 0 or false  */
#define CALL_IF0_MSG_TYPE(__exp,__msg,__type)                   \
   if (!(__exp)) { THROW_SE_TYPE (__msg,__type); }                \
   else { (void)0; }

/** Executes __exp expression and throws an exception if an exception is caught */
#define CALL_TRY(__exp)                                         \
   try {                                                        \
      __exp;                                                    \
   } catch (StationException & e) {                                   \
      e.add (__FUNC__, __LINE__, __FILE__);                     \
      throw;                                                    \
   } catch (std::exception & e1) {                              \
      throw StationException (e1,__FUNC__, __LINE__, __FILE__);   \
   } catch (...) {                                              \
      throw StationException (__FUNC__, __LINE__, __FILE__);          \
   }

/** Executes __exp expression and throws an exception with message __msg if an exception is caught */
#define CALL_TRY_MSG(__exp,__msg)                               \
   try {                                                        \
      __exp;                                                    \
   } catch (StationException & e) {                                   \
      e.add (__FUNC__, __LINE__, __FILE__, __msg);              \
      throw;                                                    \
   } catch (std::exception & e1) {                              \
      throw StationException (e1,__FUNC__, __LINE__, __FILE__, __msg);\
   } catch (...) {                                              \
      throw StationException (__FUNC__, __LINE__, __FILE__, __msg);   \
   }

/**
 * Executes __exp expression and throws an exception with variable message __msg if an exception is caught
 * @todo to finish the constructor  StationException (std::exception & e, const char * Msg, ...)
 */
#define CALL_TRY_FMT(__exp,__msg,...)                          \
   try {                                                        \
      __exp;                                                    \
   } catch (StationException & e) {                                   \
      e.add (__LINE__, __FUNC__, __FILE__, __msg, __VA_ARG__);  \
      throw;                                                    \
   } catch (std::exception & e1) {                              \
      throw StationException (e1,__FUNC__, __LINE__, __FILE__, __msg);\
   } catch (...) {                                              \
      throw StationException (__LINE__, __FUNC__, __FILE__, __msg, __VA_ARG__);  \
   }

//@}

#endif /* STATIONEXCEPTION_H_ */
