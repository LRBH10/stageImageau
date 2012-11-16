/**
 * UnitTestCase.h
 *
 * $Id: UnitTestCase.h 617 2012-07-12 08:26:26Z bdm $
 */

#ifndef UnitTestCase_H_
#define UnitTestCase_H_

#include <cppunit/extensions/HelperMacros.h>
#include "CppUnitTestHelper.h"

/**
 * @ingroup unittest
 * Empty wrapper to cppunit
 * @since 28 juil. 2011
 * @author bdm
 */
class UnitTestCase: public CPPUNIT_NS::TestFixture
{
   public:
      UnitTestCase ();
      virtual ~UnitTestCase ();
};

#endif /* UnitTestCase_H_ */
