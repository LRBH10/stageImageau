#include "SignalHandler.h"

SignalHandler::SignalState SignalHandler::state;

SignalHandler::SignalHandler ()
{
}

SignalHandler::~SignalHandler ()
{
}

void SignalHandler::setRunningState (SignalState s)
{
   SignalHandler::state = s;
}

bool SignalHandler::stillRunning ()
{
   return SignalHandler::state == SS_RUN;
}
