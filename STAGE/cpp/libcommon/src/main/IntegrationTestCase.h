/**
 * IntegrationTestCase.h
 *
 * $Id: IntegrationTestCase.h 617 2012-07-12 08:26:26Z bdm $
 */

#ifndef IntegrationTestCase_H_
#define IntegrationTestCase_H_

#include <cppunit/extensions/HelperMacros.h>
#include "CppUnitTestHelper.h"

/**
 * @ingroup unittest
 * IntegrationTestCase: CppUnit test case with parameters
 * @since 28 juil. 2011
 * @author bdm
 */
class IntegrationTestCase: public CPPUNIT_NS::TestFixture
{
   public:
      IntegrationTestCase ();
      virtual ~IntegrationTestCase ();

      /**
       * Parse default parameters
       * @param testName look for parameter for this test name
       */
      virtual void readParameters (const string & testName);

      /**
       * Should display an explicit message and quit or throw an exception
       */
      virtual void usage () = 0;

      /**
       * read parameters from command line
       */
      virtual void setParameterValues () = 0;

      /**
       * Set parameters to default value
       */
      virtual void setDefaultParameterValues () = 0;
};

#endif /* IntegrationTestCase_H_ */
