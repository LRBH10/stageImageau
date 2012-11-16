/*
 * OperationsTest.h
 *
 *  Created on: 25 juin 2012
 *      Author: bibouh
 */

#ifndef OPERATIONSTEST_H_
#define OPERATIONSTEST_H_
#include "UnitTestCase.h"
#include "SMDLauncher.h"

namespace OperationsSMD
{
   /**
    * Test of operation CPPUNIT
    */
   class OperationsTest: public UnitTestCase
   {

      CPPUNIT_TEST_SUITE (OperationsTest);
            CPPUNIT_TEST(testOperationScript);
            CPPUNIT_TEST(testOperationShellFile);
            CPPUNIT_TEST(testOperationUpdateSVN);
         CPPUNIT_TEST_SUITE_END ();

      public:
         OperationsTest ();
         virtual ~OperationsTest ();

         void testOperationScript ();
         void testOperationShellFile ();
         void testOperationUpdateSVN ();
   };
}
#endif /* OPERATIONSTEST_H_ */
