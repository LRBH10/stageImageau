/**
 * StatToolsTestCase.h
 *
 * $Id: StatToolsTestCase.h 373 2012-03-02 14:10:15Z bdm $
 */

#ifndef STATTOOLSTESTCASE_H_
#define STATTOOLSTESTCASE_H_

#include <iostream>
#include "UnitTestCase.h"
#include "StatTools.h"

/**
 * StatToolsTestCase
 * @since 22 nov. 2010
 * @author bdm
 */
class StatToolsTestCase: public UnitTestCase
{
   CPPUNIT_TEST_SUITE (StatToolsTestCase);
         CPPUNIT_TEST(testPurifiedMean);
      CPPUNIT_TEST_SUITE_END ();

   public:
      StatToolsTestCase ();
      virtual ~StatToolsTestCase ();

      void testPurifiedMean ();
};

#endif /* STATTOOLSTESTCASE_H_ */
