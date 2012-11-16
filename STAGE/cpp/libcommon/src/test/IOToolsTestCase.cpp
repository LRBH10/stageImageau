#include <cppunit/config/SourcePrefix.h>
#include "IOToolsTestCase.h"
CPPUNIT_TEST_SUITE_REGISTRATION(IOToolsTestCase);

IOToolsTestCase::IOToolsTestCase () :
   UnitTestCase ()
{
}

IOToolsTestCase::~IOToolsTestCase ()
{
}

void IOToolsTestCase::testCopy ()
{
   string src = "/etc/mime.types";
   string dest = "/tmp/toDelete";

   IOTools::fileCopy (src.c_str (), dest.c_str ());

   struct stat fileStat;
   stat (src.c_str (), &fileStat);
   int srcS = fileStat.st_size;
   stat (dest.c_str (), &fileStat);
   int destS = fileStat.st_size;

   remove (dest.c_str ());

   CPPUNIT_ASSERT_EQUAL_MESSAGE("File copy must have the same size", srcS, destS);
}
