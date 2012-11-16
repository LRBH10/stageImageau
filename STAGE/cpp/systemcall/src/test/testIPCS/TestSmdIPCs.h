/*
 * TestSmdIPCs.h
 *
 *  Created on: 20 juin 2012
 *      Author: rli
 */

#ifndef TESTSMDIPCS_H_
#define TESTSMDIPCS_H_
#include "SmdIPCStruct.h"
#include "QueueMessage.h"

using namespace systemcall;

#include "UnitTestCase.h"

class TestSmdIPCs: public UnitTestCase
{
   CPPUNIT_TEST_SUITE (TestSmdIPCs);
         CPPUNIT_TEST(testMessageString);
         CPPUNIT_TEST(testMessageProgress);
         CPPUNIT_TEST(testMessageBool);
      CPPUNIT_TEST_SUITE_END ();

   public:
      TestSmdIPCs ();
      virtual ~TestSmdIPCs ();

      void testMessageString ();
      void testMessageProgress ();
      void testMessageBool ();
};

#endif /* TESTSMDIPCS_H_ */
