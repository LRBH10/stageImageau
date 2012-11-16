#include "SMDLauncher.h"
#include "Properties.h"
#include "CallBackFunction.h"
#include "SignalMain.h"

using namespace OperationsSMD;
using namespace SMDExecute;

void killed ()
{
   LOG_INFO(SMDLauncher::logger, "FUNCTION KILL LAUNCH \n\n\n ");

   ISmdMonitor::getInstance ()->setRunning (false);

   SmdIPCBool requestBool;
   requestBool.request = false;

   QueueMessage kill (1);
   kill.sendSharedMessageSMD (SmdIPCStruct_bool, (void *) &requestBool);
}

int main ()
{
   try {
      OperationAbstract str;

      LogFactory::setLogFileName ("smdLauncher.log");
      SMDLauncher site;



      SignalMain sig;
      sig.addFuntionForSignal (SIGINT, killed);
      sig.addFuntionForSignal (SIGKILL, killed);
      sig.addFuntionForSignal (SIGTERM, killed);

      sig.waitForSignal ();

      site.connect ();
      if (site.isConnected ()) {
         site.sendVersionUpdated ();
         site.whatHaveIToDo ();
         site.executeAllOperation ();
         site.debug ();
         site.startListing ();
         site.disconnect ();
      }
   }
   catch (SmdException &exp) {
      exp.calledBy (__FUNC__, __FILE__, __LINE__);
      LOG_FATAL(SMDLauncher::logger, fString::format("EXCEPTION\n%s\n\n", exp.what()));
   }
   catch (runtime_error &rf) {
      LOG_FATAL(SMDLauncher::logger, fString::format("UNMANAGED JSON SPIRIT EXCEPTION\n%s\n\n", rf.what()));
   }
   catch (...) {
      LOG_FATAL(SMDLauncher::logger, "UNKNOWN EXCEPTION (bon courage)\n\n");
   } //*/
}
