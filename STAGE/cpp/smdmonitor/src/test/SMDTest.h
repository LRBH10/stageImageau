/*
 * SMDTest.h
 *
 *  Created on: 8 juin 2012
 *      Author: rli
 */

#ifndef SMDTEST_H_
#define SMDTEST_H_
#include "UnitTestCase.h"
#include "SMDLauncher.h"

namespace OperationsSMD
{
   /**
    * SMD Test \n
    * @warning The Web service must be running
    */
   class SMDTest: public UnitTestCase
   {
      CPPUNIT_TEST_SUITE (SMDTest);
            CPPUNIT_TEST(testConnect);
            CPPUNIT_TEST(testWhat_i_will_do);
            CPPUNIT_TEST(testDo_it);
            CPPUNIT_TEST(testDisConnect);
         CPPUNIT_TEST_SUITE_END ();

      public:
         SMDTest ();
         virtual ~SMDTest ();

         void testConnect ();
         void testWhat_i_will_do ();
         void testDo_it ();
         void testDisConnect ();

   };

} /* namespace OperationsSMD */
#endif /* SMDTEST_H_ */
