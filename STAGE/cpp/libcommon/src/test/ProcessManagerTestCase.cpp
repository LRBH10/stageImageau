#include <cppunit/config/SourcePrefix.h>
#include "ProcessManagerTestCase.h"
CPPUNIT_TEST_SUITE_REGISTRATION(ProcessManagerTestCase);

ProcessManagerTestCase::ProcessManagerTestCase () :
   UnitTestCase ()
{
}

ProcessManagerTestCase::~ProcessManagerTestCase ()
{
}

void ProcessManagerTestCase::testSimpleProc ()
{
   ProcessManager * proc = ProcessManager::getInstance ();

   proc->addNewProcess ("pwd");

   time_t maxWait = time (NULL) + 10;
   printf ("\n==== Start wait ... %lu \n", maxWait);
   while (time (NULL) < maxWait && ProcessManager::getRunningProcesses () > 0) {
      sleep (1);
   }
   printf ("\n==== End wait %lu !\n", time (NULL));

   CPPUNIT_ASSERT_EQUAL_MESSAGE("all proc must have quit", 0, ProcessManager::getRunningProcesses ());
}

void ProcessManagerTestCase::testMultipleProc ()
{
   {
      time_t delay = 60;
      ProcessManager * proc = ProcessManager::getInstance ();

      proc->addNewProcess ("ls -1", delay);
      proc->addNewProcess ("ls -1 /", delay);
      proc->addNewProcess ("ls -1 /usr", delay);
      proc->addNewProcess ("ls -1 /var", delay);
      proc->addNewProcess ("ls -1 /dev", delay);

      time_t maxWait = time (NULL) + delay;
      printf ("\n==== Start wait ... %lu \n", maxWait);
      while (time (NULL) < maxWait && ProcessManager::getRunningProcesses () > 0) {
         usleep (1000 * 500);
         pthread_yield ();
         usleep (1000 * 500);
         pthread_yield ();
      }
      printf ("\n==== End wait %lu !\n", time (NULL));
   }

   CPPUNIT_ASSERT_EQUAL_MESSAGE("all proc must have quit", 0, ProcessManager::getRunningProcesses ());
}

void ProcessManagerTestCase::testQuit ()
{
   {
      ProcessManager * proc = ProcessManager::getInstance ();

      proc->addNewProcess ("sleep 500");
      proc->addNewProcess ("sleep 500");
      proc->addNewProcess ("sleep 500");
      proc->addNewProcess ("sleep 500");

      printf ("\n==== Start wait ... \n");
      sleep (5);
      printf ("\n==== End wait !\n");

      ProcessManager::setQuit ();

      printf ("\n==== Start wait ... \n");
      time_t maxWait = time (NULL) + 5;
      while (time (NULL) < maxWait && ProcessManager::getRunningProcesses () > 0) {
         sleep (1);
      }
      printf ("\n==== End wait !\n");
   }

   CPPUNIT_ASSERT_EQUAL_MESSAGE("all proc must have quit", 0, ProcessManager::getRunningProcesses ());
}

void ProcessManagerTestCase::testSimpleProcCB ()
{
   {
      ProcessManager * proc = ProcessManager::getInstance ();

      proc->addNewProcess ("pwd", 60, NULL, &ProcessManagerTestCase::resultCallBackNotEmpty);

      time_t maxWait = time (NULL) + 10;
      printf ("\n==== Start wait ... %lu \n", maxWait);
      while (time (NULL) < maxWait && ProcessManager::getRunningProcesses () > 0) {
         sleep (1);
      }
      printf ("\n==== End wait %lu !\n", time (NULL));
   }

   CPPUNIT_ASSERT_EQUAL_MESSAGE("all proc must have quit", 0, ProcessManager::getRunningProcesses ());
}

void ProcessManagerTestCase::testMultipleProcCB ()
{
   {
      ProcessManager * proc = ProcessManager::getInstance ();

      proc->addNewProcess ("ls -1", 60, NULL, &ProcessManagerTestCase::resultCallBackNotEmpty);
      proc->addNewProcess ("ls -1 /", 60, NULL, &ProcessManagerTestCase::resultCallBackNotEmpty);
      proc->addNewProcess ("ls -1 /usr", 60, NULL, &ProcessManagerTestCase::resultCallBackNotEmpty);
      proc->addNewProcess ("ls -1 /var", 60, NULL, &ProcessManagerTestCase::resultCallBackNotEmpty);
      proc->addNewProcess ("ls -1 /dev", 60, NULL, &ProcessManagerTestCase::resultCallBackNotEmpty);

      time_t maxWait = time (NULL) + 10;
      printf ("\n==== Start wait ... %lu \n", maxWait);
      while (time (NULL) < maxWait && ProcessManager::getRunningProcesses () > 0) {
         sleep (1);
      }
      printf ("\n==== End wait %lu !\n", time (NULL));
   }

   CPPUNIT_ASSERT_EQUAL_MESSAGE("all proc must have quit", 0, ProcessManager::getRunningProcesses ());
}

void ProcessManagerTestCase::testQuitCB ()
{
   {
      ProcessManager * proc = ProcessManager::getInstance ();

      proc->addNewProcess ("sleep 500", 60, NULL, &ProcessManagerTestCase::resultCallBackEmpty);
      proc->addNewProcess ("sleep 500", 60, NULL, &ProcessManagerTestCase::resultCallBackEmpty);
      proc->addNewProcess ("sleep 500", 60, NULL, &ProcessManagerTestCase::resultCallBackEmpty);
      proc->addNewProcess ("sleep 500", 60, NULL, &ProcessManagerTestCase::resultCallBackEmpty);

      printf ("\n==== Start wait ... \n");
      sleep (5);
      printf ("\n==== End wait !\n");

      ProcessManager::setQuit ();

      printf ("\n==== Start wait ... \n");
      time_t maxWait = time (NULL) + 5;
      while (time (NULL) < maxWait && ProcessManager::getRunningProcesses () > 0) {
         sleep (1);
      }
      printf ("\n==== End wait !\n");
   }

   CPPUNIT_ASSERT_EQUAL_MESSAGE("all proc must have quit", 0, ProcessManager::getRunningProcesses ());
}

void ProcessManagerTestCase::resultCallBackNotEmpty (ThreadInfo *info, const string & result, const string & msgStr)
{
   printf ("\nResult for threadId:%lu, result:%s, msg:%s\n", info->thread_id, result.c_str (), msgStr.c_str ());
   CPPUNIT_ASSERT_MESSAGE("result must not be empty", (result.size () > 0));
}

void ProcessManagerTestCase::resultCallBackEmpty (ThreadInfo *info, const string & result, const string & msgStr)
{
   printf ("\nResult for threadId:%lu, result:%s, msg:%s\n", info->thread_id, result.c_str (), msgStr.c_str ());
   CPPUNIT_ASSERT_MESSAGE("result should be empty", result.empty ());
}
