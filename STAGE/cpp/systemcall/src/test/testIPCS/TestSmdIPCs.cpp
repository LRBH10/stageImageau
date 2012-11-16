/*
 * TestSmdIPCs.cpp
 *
 *  Created on: 20 juin 2012
 *      Author: rli
 */

#include "TestSmdIPCs.h"

#include <cppunit/config/SourcePrefix.h>
CPPUNIT_TEST_SUITE_REGISTRATION (TestSmdIPCs);

TestSmdIPCs::TestSmdIPCs () :
   UnitTestCase ()
{
   QueueMessage::killMessageQueue (SMD_IPC_IDENTIFIER);
}

TestSmdIPCs::~TestSmdIPCs ()
{
   QueueMessage::killMessageQueue (SMD_IPC_IDENTIFIER);
}

void TestSmdIPCs::testMessageString ()
{
   SmdIPCString testString;

   memset (&testString, 0, sizeof(SmdIPCString));
   strcpy (testString.contents, "Tableau format international");

   QueueMessage tst (1);
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SIZE FILE MESSAGE ", 0, tst.messageQueueSize ());

   tst.sendSharedMessageSMD (SmdIPCStruct_string, (void *) &testString);
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SIZE FILE MESSAGE ", 2, tst.messageQueueSize ());

   SmdIPCGlobalResponse ffq;
   tst.receiveSharedMessageSMD (ffq);
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SAME STRING ", 0, strcmp (ffq.RString.contents, testString.contents));

   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SIZE FILE MESSAGE ", 0, tst.messageQueueSize ());
}

void TestSmdIPCs::testMessageProgress ()
{
   SmdIPCProgress testProgress;
   memset (&testProgress, 0, sizeof(SmdIPCProgress));

   testProgress.id = 127;
   strcpy (testProgress.title, "Progress Acquisition ");
   testProgress.progress_value = rand () % 100;

   QueueMessage tst (1);
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SIZE FILE MESSAGE ", 0, tst.messageQueueSize ());

   tst.sendSharedMessageSMD (SmdIPCStruct_progress, (void *) &testProgress);
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SIZE FILE MESSAGE ", 2, tst.messageQueueSize ());

   SmdIPCGlobalResponse ffq;
   tst.receiveSharedMessageSMD (ffq);
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SIZE FILE MESSAGE ", 0, tst.messageQueueSize ());

   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SAME ID ", testProgress.id, ffq.RProgress.id);
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SAME TITLE ", 0, strcmp (ffq.RProgress.title, testProgress.title));
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SAME VALUE ", testProgress.progress_value, ffq.RProgress.progress_value);
}

void TestSmdIPCs::testMessageBool ()
{
   SmdIPCBool testBool;
   memset (&testBool, 0, sizeof(SmdIPCBool));

   testBool.request = true;

   QueueMessage tst (1);
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SIZE FILE MESSAGE ", 0, tst.messageQueueSize ());

   tst.sendSharedMessageSMD (SmdIPCStruct_bool, (void *) &testBool);
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SIZE FILE MESSAGE ", 2, tst.messageQueueSize ());

   SmdIPCGlobalResponse ffq;
   tst.receiveSharedMessageSMD (ffq);
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SIZE FILE MESSAGE ", 0, tst.messageQueueSize ());

   CPPUNIT_ASSERT_EQUAL_MESSAGE ("SAME REquest ", testBool.request, ffq.RBool.request);
}
