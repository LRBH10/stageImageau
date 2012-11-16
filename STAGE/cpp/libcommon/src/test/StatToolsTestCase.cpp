#include <cppunit/config/SourcePrefix.h>
#include "StatToolsTestCase.h"
CPPUNIT_TEST_SUITE_REGISTRATION(StatToolsTestCase);

StatToolsTestCase::StatToolsTestCase () :
   UnitTestCase ()
{

}

StatToolsTestCase::~StatToolsTestCase ()
{

}

void StatToolsTestCase::testPurifiedMean ()
{
   double values [] = { 8, 9, 10, 11, 12, 11, 10, 8, 1, 2, 20 };
   uint32 nbVal = 11;
   Covariance cov (values, nbVal);
   Covariance out = StatTools::purifiedCov (cov, 2.0);

   CPPUNIT_ASSERT_EQUAL(nbVal - 3, out.nbVal);
   //   for (uint32 i = 0; i < cov.nbVal; ++i) {
   //      printf ("%f ", cov.values [i]);
   //   }
   //   printf ("\n");

}
