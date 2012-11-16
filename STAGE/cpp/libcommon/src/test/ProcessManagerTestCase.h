/**
 * ProcessManagerTestCase.h
 *
 * $Id: ProcessManagerTestCase.h 373 2012-03-02 14:10:15Z bdm $
 */

#ifndef ProcessManagerTestCase_H_
#define ProcessManagerTestCase_H_

#include <iostream>
#include "UnitTestCase.h"
#include "commtype.h"
#include <cerrno>
#include "StationException.h"
#include "ProcessManager.h"

using namespace std;

/**
 * ProcessManagerTestCase
 * @since 2011/05/27
 * @author bdm
 */
class ProcessManagerTestCase: public UnitTestCase
{
   CPPUNIT_TEST_SUITE (ProcessManagerTestCase);
         CPPUNIT_TEST(testSimpleProc);
         CPPUNIT_TEST(testMultipleProc);
         CPPUNIT_TEST(testQuit);
         CPPUNIT_TEST(testSimpleProcCB);
         CPPUNIT_TEST(testMultipleProcCB);
         CPPUNIT_TEST(testQuitCB);
      CPPUNIT_TEST_SUITE_END ();

      static void resultCallBackNotEmpty (ThreadInfo *info, const string & result, const string & msgStr);
      static void resultCallBackEmpty (ThreadInfo *info, const string & result, const string & msgStr);

   public:
      ProcessManagerTestCase ();
      virtual ~ProcessManagerTestCase ();

      void testSimpleProc ();
      void testMultipleProc ();
      void testQuit ();
      void testSimpleProcCB ();
      void testMultipleProcCB ();
      void testQuitCB ();

};

#endif /* ProcessManagerTestCase_H_ */
