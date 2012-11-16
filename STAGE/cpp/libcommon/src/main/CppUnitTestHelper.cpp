#include "CppUnitTestHelper.h"

int CppUnitTestHelper::argc = 0;
char ** CppUnitTestHelper::argv = NULL;

CppUnitTestHelper::CppUnitTestHelper ()
{
}

CppUnitTestHelper::~CppUnitTestHelper ()
{
}

int CppUnitTestHelper::getArgc ()
{
   return CppUnitTestHelper::argc;
}

char * CppUnitTestHelper::getArgv (int i)
{
   char * out = NULL;
   if (i < CppUnitTestHelper::argc) {
      out = CppUnitTestHelper::argv [i];
   }
   return out;
}

void CppUnitTestHelper::listTest (CPPUNIT_NS::Test * rootTest)
{
   CppUnitTestHelper::argc = 1;
   CPPUNIT_NS::Test * subTest;
   fprintf (stderr, "Test list:\n");
   for (int i = 0, m = 0; m < rootTest->countTestCases (); ++i) {
      subTest = rootTest->getChildTestAt (i);
      fprintf (stderr, "Find test '%s'\n", subTest->getName ().c_str ());
      for (int ii = 0; ii < subTest->countTestCases (); ++ii, ++m) {
         fprintf (stderr, "Find sub test '%s'\n", subTest->getChildTestAt (ii)->getName ().c_str ());
      }
   }
}

int CppUnitTestHelper::runTest (int argc, char** argv, const string & reportName)
{
   int out = 0;

   if (argc > 0 && argv != NULL) {
      CppUnitTestHelper::argc = argc;
      CppUnitTestHelper::argv = argv;
   }
   CPPUNIT_NS::Test * rootTest = CPPUNIT_NS::TestFactoryRegistry::getRegistry ().makeTest ();
   CPPUNIT_NS::Test * test = NULL;
   if (argc > 1) {
      if (strcmp (argv [1], "help") == 0 || strcmp (argv [1], "--help") == 0 || strcmp (argv [1], "-h") == 0) {
         CppUnitTestHelper::listTest (rootTest);
      }
      else {
         try {
            test = rootTest->findTest (argv [1]);
         }
         catch (std::exception &e) {
            CppUnitTestHelper::listTest (rootTest);
            throw;
         }
      }
   }
   else {
      test = rootTest;
   }

   if (test != NULL) {
      // Create the event manager and test controller
      CPPUNIT_NS::TestResult controller;
      // Add a listener that colllects test result
      CPPUNIT_NS::TestResultCollector result;
      controller.addListener (&result);
      // Add a listener that print dots as test run.
      CPPUNIT_NS::BriefTestProgressListener progress;
      controller.addListener (&progress);
      // Add the top suite to the test runner
      CPPUNIT_NS::TestRunner runner;

      runner.addTest (test);
      runner.run (controller);
      // Print test in a compiler compatible format.
      CPPUNIT_NS::CompilerOutputter outputter (&result, CPPUNIT_NS::stdCOut ());
      outputter.write ();

      ofstream outFile (reportName.c_str ());
      CPPUNIT_NS::XmlOutputter xml_outputter (&result, outFile);
      xml_outputter.write ();

      out = result.wasSuccessful () ? 0 : -1;

      // runner destructor should do the cleaning work
   }
   else {
      CPPUNIT_NS::TestRunner deleter;
      deleter.addTest (rootTest);
   }

   return out;
}
