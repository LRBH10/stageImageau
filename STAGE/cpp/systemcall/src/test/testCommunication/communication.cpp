//============================================================================
// Name        : Communication.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>
#include "QueueMessage.h"
#include "SignalMain.h"
#include "CallBackFunction.h"

using namespace std;
using namespace systemcall;

int main ()
{
   QueueMessage tst (1);

   SmdIPCProgress testProgress;
   memset (&testProgress, 0, sizeof(SmdIPCProgress));

   strcpy (testProgress.title, "a simple test  ");
   testProgress.id = 6;
   testProgress.progress_value = rand () % 100;

   tst.sendSharedMessageSMD (SmdIPCStruct_progress, (void *) &testProgress);
   //   SmdIPCGlobalResponse ffq;
   //   tst.receiveSharedMessage (ffq);
   //   ffq.debug (); //*/

   //   tst.killMessageQueue ();
}

