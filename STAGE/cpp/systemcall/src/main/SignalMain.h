/*
 * SignalMain.h
 *
 *  Created on: 19 juil. 2012
 *      Author: rli
 */

#ifndef SIGNALMAIN_H_
#define SIGNALMAIN_H_
#include <iostream>
#include <csignal>
#include <cstring>
#include <stdlib.h>
#include <map>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "LogFactory.h"


using namespace std;

namespace systemcall
{

   /**
    * The class that Receive the SIGNALs of the system \n
    * Example for utilization \n
    * Creating A Signal Handler
    * @code
    * SignalMain sig;
    * @endcode
    * add the different function for SIGNAL types
    * @code
    * sig.addFuntionForSignal (SIGINT, killed);
    * sig.addFuntionForSignal (SIGKILL, killed);
    * sig.addFuntionForSignal (SIGTERM, killed);
    * @endcode
    * Wait for the Signals
    * @code
    * sig.waitForSignal ();
    * @endcode
    *
    *
    *  Signals. \n
    *  @code
    * #define  SIGHUP      1  // Hangup (POSIX).
    * #define  SIGINT      2  // Interrupt (ANSI).
    * #define  SIGQUIT     3  // Quit (POSIX).
    * #define  SIGILL      4  // Illegal instruction (ANSI).
    * #define  SIGTRAP     5  // Trace trap (POSIX).
    * #define  SIGABRT     6  // Abort (ANSI).
    * #define  SIGIOT      6  // IOT trap (4.2 BSD).
    * #define  SIGBUS      7  // BUS error (4.2 BSD).
    * #define  SIGFPE      8  // Floating-point exception (ANSI).
    * #define  SIGKILL     9  // Kill, unblockable (POSIX).
    * #define  SIGUSR1     10 // User-defined signal 1 (POSIX).
    * #define  SIGSEGV     11 // Segmentation violation (ANSI).
    * #define  SIGUSR2     12 // User-defined signal 2 (POSIX).
    * #define  SIGPIPE     13 // Broken pipe (POSIX).
    * #define  SIGALRM     14 // Alarm clock (POSIX).
    * #define  SIGTERM     15 // Termination (ANSI).
    * #define  SIGSTKFLT   16 // Stack fault.
    * #define  SIGCLD      SIGCHLD  // Same as SIGCHLD (System V).
    * #define  SIGCHLD     17 // Child status has changed (POSIX).
    * #define  SIGCONT     18 // Continue (POSIX).
    * #define  SIGSTOP     19 // Stop, unblockable (POSIX).
    * #define  SIGTSTP     20 // Keyboard stop (POSIX).
    * #define  SIGTTIN     21 // Background read from tty (POSIX).
    * #define  SIGTTOU     22 // Background write to tty (POSIX).
    * #define  SIGURG      23 // Urgent condition on socket (4.2 BSD).
    * #define  SIGXCPU     24 // CPU limit exceeded (4.2 BSD).
    * #define  SIGXFSZ     25 // File size limit exceeded (4.2 BSD).
    * #define  SIGVTALRM   26 // Virtual alarm clock (4.2 BSD).
    * #define  SIGPROF     27 // Profiling alarm clock (4.2 BSD).
    * #define  SIGWINCH 28 // Window size change (4.3 BSD, Sun).
    * #define  SIGPOLL     SIGIO // Pollable event occurred (System V).
    * #define  SIGIO    29 // I/O now possible (4.2 BSD).
    * #define  SIGPWR      30 // Power failure restart (System V).
    * #define SIGSYS    31 // Bad system call.
    * #define SIGUNUSED 31
    * #define  _NSIG    65 // Biggest signal number + 1
    * @endcode
    */
   class SignalMain
   {
      public:

         static LoggerPtr logger;

         /**
          * default Constructor
          */
         SignalMain ();

         /**
          * default destructor
          */

         virtual ~SignalMain ();

         /**
          * define the different signal that will be intercepted \n
          * by default the first 16 signal.
          */
         virtual void waitForSignal ();

         /**
          * call the function that will be attached for the signal \n
          * else Exit();
          * @param signum the type of the SIGNAL
          */
         static void signalProcess (int signum);

         /**
          * Attach A signal with a function\n
          * with prototype <strong>void (&fun) (void) </strong>
          * @param sig type of signal
          * @param fun function with  void (&fun) (void)  prototype
          */
         static void addFuntionForSignal (int sig, void (&fun) (void));

         /**
          * attach with boost::function() generated with @ref CallBackFunction
          * Example of utilization:
          * @code
          * void callBacktest(MyStruct my_struct, int y , int z);
          * CallBackFunction<void (&)(MyStruct,int,int)> callme (callBacktest, mystruct,y,z);
          *
          * addFuntionForSignal(SIGINT, callme.getfunctionBind() );
          *
          * @endcode
          * @param sig signal Type
          * @param fun to attach it
          */
         static void addFuntionForSignal (int sig, boost::function<void (void)> fun);

         static map<int, boost::function<void (void)> > signalcall;

   };

} /* namespace SignalNS */
#endif /* SIGNALMAIN_H_ */
