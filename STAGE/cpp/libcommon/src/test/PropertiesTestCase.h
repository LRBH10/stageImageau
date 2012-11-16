/**
 * PropertiesTestCase.h
 *
 * $Id: PropertiesTestCase.h 373 2012-03-02 14:10:15Z bdm $
 */

#ifndef PROPERTIESTESTCASE_H_
#define PROPERTIESTESTCASE_H_

#include "UnitTestCase.h"
#include "Properties.h"
#include "LogFactory.h"

using namespace imaGeau;

/**
 * PropertiesTestCase
 * @since 28/06/2011
 * @author bdm
 */
class PropertiesTestCase: public UnitTestCase
{
   CPPUNIT_TEST_SUITE (PropertiesTestCase);
         CPPUNIT_TEST(testGetString);
         CPPUNIT_TEST(testGetInt);
         CPPUNIT_TEST(testGetDouble);
         CPPUNIT_TEST(testGetBool);
         CPPUNIT_TEST(testLoadFile);
      CPPUNIT_TEST_SUITE_END ();

      static LoggerPtr logger;

   public:
      PropertiesTestCase ();
      virtual ~PropertiesTestCase ();

      void testGetString ();
      void testGetInt ();
      void testGetDouble ();
      void testGetBool ();
      void testLoadFile ();
};

#endif /* PROPERTIESTESTCASE_H_ */
