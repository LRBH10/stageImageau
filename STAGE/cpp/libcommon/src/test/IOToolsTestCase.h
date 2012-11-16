/**
 * IOToolsTestCase.h
 *
 * $Id: IOToolsTestCase.h 373 2012-03-02 14:10:15Z bdm $
 */

#ifndef IOToolsTestCase_H_
#define IOToolsTestCase_H_

#include <iostream>
#include "UnitTestCase.h"
#include "commtype.h"
#include <cerrno>
#include <sys/stat.h>
#include "IOTools.h"

using namespace std;

/**
 * IOToolsTestCase
 * @since 2011/07/21
 * @author bdm
 */
class IOToolsTestCase: public UnitTestCase
{
   CPPUNIT_TEST_SUITE (IOToolsTestCase);
         CPPUNIT_TEST(testCopy);
      CPPUNIT_TEST_SUITE_END ();

   public:
      IOToolsTestCase ();
      virtual ~IOToolsTestCase ();

      void testCopy ();

};

#endif /* IOToolsTestCase_H_ */
