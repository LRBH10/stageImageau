/**
 * FStringTestCase.h
 *
 * $Id: FStringTestCase.h 373 2012-03-02 14:10:15Z bdm $
 */

#ifndef FStringTestCase_H_
#define FStringTestCase_H_

#include <iostream>
#include "UnitTestCase.h"
#include "commtype.h"
#include <cerrno>
#include "StationException.h"
#include <vector>

using namespace std;

/**
 * FStringTestCase
 * @since 2011/05/19
 * @author bdm
 */
class FStringTestCase: public UnitTestCase
{
   CPPUNIT_TEST_SUITE (FStringTestCase);
         CPPUNIT_TEST(testFormat);
         CPPUNIT_TEST(testTokenizeEmptyString);
         CPPUNIT_TEST(testTokenizeSimple);
         CPPUNIT_TEST(testTokenizeEmptyToken);
         CPPUNIT_TEST(testTrim);
      CPPUNIT_TEST_SUITE_END ();

   public:
      FStringTestCase ();
      virtual ~FStringTestCase ();

      void testFormat ();
      void testTokenizeEmptyString ();
      void testTokenizeSimple ();
      void testTokenizeEmptyToken ();

      void testTrim ();
};

#endif /* FStringTestCase_H_ */
