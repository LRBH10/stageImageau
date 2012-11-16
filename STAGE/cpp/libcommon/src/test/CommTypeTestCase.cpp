#include <cppunit/config/SourcePrefix.h>
#include "CommTypeTestCase.h"
CPPUNIT_TEST_SUITE_REGISTRATION(CommTypeTestCase);

CommTypeTestCase::CommTypeTestCase () :
   UnitTestCase ()
{
}

CommTypeTestCase::~CommTypeTestCase ()
{
}

void CommTypeTestCase::testTypeSize ()
{
   CPPUNIT_ASSERT_EQUAL_MESSAGE("bad byte size", 1, (int) sizeof(byte));
   CPPUNIT_ASSERT_EQUAL_MESSAGE("bad uint8 size", 1, (int) sizeof(uint8));
   CPPUNIT_ASSERT_EQUAL_MESSAGE("bad uint16 size", 2, (int) sizeof(uint16));
   CPPUNIT_ASSERT_EQUAL_MESSAGE("bad uint32 size", 4, (int) sizeof(uint32));
   CPPUNIT_ASSERT_EQUAL_MESSAGE("bad uint64 size", 8, (int) sizeof(uint64));
}
