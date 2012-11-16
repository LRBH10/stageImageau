#include "IntegrationTestCase.h"

IntegrationTestCase::IntegrationTestCase () :
   CPPUNIT_NS::TestFixture ()
{
}

IntegrationTestCase::~IntegrationTestCase ()
{
}

void IntegrationTestCase::readParameters (const string & testName)
{
   setDefaultParameterValues ();

   if (CppUnitTestHelper::getArgc () > 1) {
      char * argv1 = CppUnitTestHelper::getArgv (1);
      if (strcmp (argv1, "help") == 0 || strcmp (argv1, "--help") == 0 || strcmp (argv1, "-h") == 0) {
         // nop general help asked ... just for listing
      }
      else {
         if (strstr (argv1, testName.c_str ()) == argv1) { // parameter for me
            if (CppUnitTestHelper::getArgc () > 2) {
               char * argv2 = CppUnitTestHelper::getArgv (2);
               if (argv2 != NULL && (strcmp (argv2, "help") == 0 || strcmp (argv2, "--help") == 0 || strcmp (argv2, "-h") == 0)) {
                  usage ();
               }
               else {
                  setParameterValues ();
               }
            }
         }
         else {
            fprintf (stderr, "Do not know what to do with this test '%s'", testName.c_str ());
         }
      }
   }
}
