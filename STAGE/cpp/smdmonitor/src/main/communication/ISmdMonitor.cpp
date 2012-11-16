#include "ISmdMonitor.h"

using namespace SMDCommunication;

ISmdMonitor * ISmdMonitor::instance;

void ISmdMonitor::setInstance (ISmdMonitor * mon)
{
   ISmdMonitor::instance = mon;
}

ISmdMonitor * ISmdMonitor::getInstance ()
{
   return ISmdMonitor::instance;
}

ISmdMonitor::~ISmdMonitor ()
{

}
