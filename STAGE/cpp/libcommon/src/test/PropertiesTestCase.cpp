#include <cppunit/config/SourcePrefix.h>
#include "PropertiesTestCase.h"
CPPUNIT_TEST_SUITE_REGISTRATION(PropertiesTestCase);

LoggerPtr PropertiesTestCase::logger (LogFactory::getLogger ("PropertiesTestCase"));

PropertiesTestCase::PropertiesTestCase () :
   UnitTestCase ()
{
}

PropertiesTestCase::~PropertiesTestCase ()
{
}

void PropertiesTestCase::testGetString ()
{
   Properties prop;
   string test = "mljfglksjh geùsGj<ùhj mlqih mlrgIHRSg";

   prop.add ("test.key", test);
   try {
      string value = prop.get ("test.key");
      CPPUNIT_ASSERT_EQUAL_MESSAGE("get string failed (bad return)", 0, value.compare (test));
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("get string failed (not found)", true);
   }
}

void PropertiesTestCase::testGetInt ()
{
   Properties prop;

   prop.add ("test.key", "5");
   try {
      int value = prop.getInt ("test.key");
      CPPUNIT_ASSERT_EQUAL_MESSAGE("get int failed (bad return)", 5, value);
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("get int failed (not found)", true);
   }

   try {
      int value = prop.getInt ("test.noexistent", true, 222);
      CPPUNIT_ASSERT_EQUAL_MESSAGE("get int failed (bad return)", 222, value);
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("get int  failed (not found)", true);
   }
}

void PropertiesTestCase::testGetDouble ()
{
   Properties prop;

   prop.add ("test.key", "5.326");
   try {
      double value = prop.getDouble ("test.key");
      CPPUNIT_ASSERT_EQUAL_MESSAGE("get double failed (bad return)", 5.326, value);
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("get double  failed (not found)", true);
   }

   prop.add ("test.key2", "5");
   try {
      double value = prop.getDouble ("test.key2");
      CPPUNIT_ASSERT_EQUAL_MESSAGE("get double failed (bad return)", 5.0, value);
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("get double  failed (not found)", true);
   }

   prop.add ("test.key3", "-6.5");
   try {
      double value = prop.getDouble ("test.key3");
      CPPUNIT_ASSERT_EQUAL_MESSAGE("get double failed (bad return)", -6.5, value);
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("get double  failed (not found)", true);
   }

   try {
      double value = prop.getDouble ("test.noexistent", true, 222.22);
      CPPUNIT_ASSERT_EQUAL_MESSAGE("get double failed (bad return)", 222.22, value);
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("get double  failed (not found)", true);
   }

}

void PropertiesTestCase::testGetBool ()
{
   Properties prop;
   prop.add ("test.key", "true");
   try {
      bool value = prop.getBoolean ("test.key");
      CPPUNIT_ASSERT_EQUAL_MESSAGE("get boolean failed (bad return)", true, value);
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("get boolean failed (not found)", true);
   }

   prop.add ("test.key2", "1");
   try {
      bool value = prop.getBoolean ("test.key2");
      CPPUNIT_ASSERT_EQUAL_MESSAGE("get boolean  failed (bad return)", true, value);
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("get boolean failed (not found)", true);
   }

   prop.add ("test.key3", "zzzzz");
   try {
      bool value = prop.getBoolean ("test.key3");
      CPPUNIT_ASSERT_EQUAL_MESSAGE("get boolean  failed (bad return)", false, value);
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("get boolean failed (not found)", true);
   }

   try {
      bool value = prop.getInt ("test.noexistent", true, false);
      CPPUNIT_ASSERT_EQUAL_MESSAGE("get boolean failed (bad return)", false, value);
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("get int  failed (not found)", true);
   }
}

void PropertiesTestCase::testLoadFile ()
{
   Properties prop;
   try {
      prop.loadFromFile ("conf/test1/devices.cfg");
   }
   catch (StationException &e) {
      CPPUNIT_ASSERT_MESSAGE("Load failed !", false);
   }

   try {
      double value = prop.getDouble ("bus.adc.a");
      CPPUNIT_ASSERT_MESSAGE("Load failed (bad return)", value > 0.0);
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("Load failed (not found)", true);
   }

   try {
      string value = prop.get ("bus.name");
      CPPUNIT_ASSERT_EQUAL_MESSAGE("Load failed (bad return)", 0, value.compare ("v0_fedel"));
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("Load failed (not found)", true);
   }

   try {
      string value = prop.get ("power_supply.name");
      CPPUNIT_ASSERT_EQUAL_MESSAGE("Load failed (bad return)", 0, value.compare ("TDK_Genesys"));
   }
   catch (...) {
      CPPUNIT_ASSERT_MESSAGE("Load failed (not found)", true);
   }
}
