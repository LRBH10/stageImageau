/**
 * CommTypeTestCase.h
 *
 * $Id: CommTypeTestCase.h 373 2012-03-02 14:10:15Z bdm $
 */

#ifndef CommTypeTestCase_H_
#define CommTypeTestCase_H_

#include <iostream>
#include "UnitTestCase.h"
#include "commtype.h"

/**
 * CommTypeTestCase
 * @since 8 mars 2010
 * @author bdm
 */
class CommTypeTestCase: public UnitTestCase
{
   CPPUNIT_TEST_SUITE (CommTypeTestCase);
         CPPUNIT_TEST(testTypeSize);
      CPPUNIT_TEST_SUITE_END ();

   public:
      CommTypeTestCase ();
      virtual ~CommTypeTestCase ();

      void testTypeSize ();
};

#endif /* CommTypeTestCase_H_ */
