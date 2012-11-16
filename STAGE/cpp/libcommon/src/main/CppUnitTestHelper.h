/**
 * CppUnitTestHelper.h
 *
 * $Id: CppUnitTestHelper.h 617 2012-07-12 08:26:26Z bdm $
 */

#ifndef CPPUNITTESTHELPER_H_
#define CPPUNITTESTHELPER_H_

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;

/**
 * @ingroup commontools
 * @defgroup unittest cpp unit testing module
 * Allow to run cppunit test case. If a parameter is given, try to find and launch the corresponding test name. Use the other parameters as test parameters.
 * @since 27 mai 2011
 * @author bdm
 */
class CppUnitTestHelper
{
      CppUnitTestHelper ();
      virtual ~CppUnitTestHelper ();

      static int argc;
      static char ** argv;

   public:
      /**
       * @return main argc value
       */
      static int getArgc ();

      /**
       * @param i index in argv
       * @return main argv value at pos i
       */
      static char * getArgv (int i);

      /**
       * run a test or a test suite
       * @param argc size of argv
       * @param argv program argument. 0 prog name, 1 test name, 2 ... N test param
       * @param reportName
       * @return
       */
      static int runTest (int argc, char** argv, const string & reportName);

      /**
       * print the sub list of test
       * @param rootTest test root tree
       */
      static void listTest (CPPUNIT_NS::Test * rootTest);
};

#endif /* CPPUNITTESTHELPER_H_ */
