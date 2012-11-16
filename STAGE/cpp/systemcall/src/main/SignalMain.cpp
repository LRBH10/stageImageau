/*
 * SignalMain.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: rli
 */

#include "SignalMain.h"

using namespace systemcall;

map<int, boost::function<void (void)> > SignalMain::signalcall;

LoggerPtr SignalMain::logger (LogFactory::getLogger ("SignalMain"));

SignalMain::SignalMain ()
{

}

SignalMain::~SignalMain ()
{

}

void SignalMain::waitForSignal ()
{
   struct sigaction new_action, old_action;

   /* Set up the structure to specify the new action. */
   new_action.sa_handler = SignalMain::signalProcess;
   sigemptyset (&new_action.sa_mask);
   new_action.sa_flags = 0;

   // change all default to signalTrap_STOP
   for (int i = 1; i <= 16; ++i) {
      sigaction (i, NULL, &old_action);
      if (old_action.sa_handler != SIG_IGN) {
         sigaction (i, &new_action, NULL);
      }
   }

   sigaction (SIGTERM, &new_action, NULL);
   sigaction (SIGSEGV, &new_action, NULL);
   sigaction (SIGFPE, &new_action, NULL);
   sigaction (SIGILL, &new_action, NULL);
   sigaction (SIGSTOP, &new_action, NULL);
}

void SignalMain::signalProcess (int sig)
{
   LOG_INFO(logger, fString::format("SIGNAL DETECTED  '%s'  (%d)", strsignal(sig),sig));

   bool found = false;
   for (map<int, boost::function<void (void)> >::iterator it = signalcall.begin (); it != signalcall.end (); ++it) {
      if ((*it).first == sig) {
         LOG_INFO(logger, "FUNCTION  ATTACHED DETECTED");
         (*it).second ();
         found = true;
      }
   }

   if (!found) {
      LOG_INFO(logger, "NO FUNCTION  ATTACHED (THEN EXIT)");
      exit (0);
   }

}

void SignalMain::addFuntionForSignal (int sig, void (&fun) (void))
{
   signalcall [sig] = boost::bind (fun);
}

void SignalMain::addFuntionForSignal (int sig, boost::function<void (void)> fun)
{
   signalcall [sig] = fun;
}

