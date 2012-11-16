#include <cppunit/config/SourcePrefix.h>
#include "FStringTestCase.h"
CPPUNIT_TEST_SUITE_REGISTRATION(FStringTestCase);

FStringTestCase::FStringTestCase () :
   UnitTestCase ()
{
}

FStringTestCase::~FStringTestCase ()
{
}

void FStringTestCase::testFormat ()
{
   string str ("abc");
   string fmt ("%d %s %s");
   string out;

   out = fString::format (fmt.c_str (), 0, str.c_str (), str.c_str ());
   CPPUNIT_ASSERT_EQUAL_MESSAGE("format failed !", 9, (int) out.size ());

   int i = 0;
   while (out.size () < 2048) {
      try {
         char * errStr = strerror (errno);
         CPPUNIT_ASSERT_MESSAGE("strerror func returns NULL !", errStr != NULL);
         THROW_SE(fString::format ("fake exception ! fake exception! fake! Errno:'%s'", (errStr == NULL ? "NULL" : errStr)));
         //fString::format (fmt.c_str (), i, out.c_str (), strerror (errno));
         ++i;
      }
      catch (StationException &e) {
         out += e.what ();
      }
   }

   CPPUNIT_ASSERT_MESSAGE("format failed !", (out.size () > 2048));
}

void FStringTestCase::testTokenizeEmptyString ()
{
   string toTest = "";
   vector<string> tokens;
   fString::tokenize (toTest, tokens, " ");
   CPPUNIT_ASSERT_EQUAL_MESSAGE("empty string must have 0 token", 0, (int) tokens.size ());
}

void FStringTestCase::testTokenizeSimple ()
{
   string toTest = "aze dfglkjhdfg kjshd lkjhdfgh ";
   vector<string> tokens;
   fString::tokenize (toTest, tokens, " ");
   CPPUNIT_ASSERT_EQUAL_MESSAGE("Simple string must have tokens", 4, (int) tokens.size ());
}

void FStringTestCase::testTokenizeEmptyToken ()
{
   string toTest = "aze dfglkjhdfg  kjshd lkjhdfgh  dsgsd";
   vector<string> tokens;
   fString::tokenize (toTest, tokens, " ");
   CPPUNIT_ASSERT_EQUAL_MESSAGE("Simple string must have tokens", 5, (int) tokens.size ());
}

void FStringTestCase::testTrim ()
{
   string toTest = "   aze dfglkjhdfg  kjshd lkjhdfgh  dsgsd      ";
   string expected = "aze dfglkjhdfg  kjshd lkjhdfgh  dsgsd";
   string res = fString::trim (toTest);
   //   printf ("\ntoTest: '%s'\n", toTest.c_str ());
   //   printf ("res: '%s' l:%lu\n", res.c_str (), res.length ());
   //   printf ("exp: '%s' l:%lu\n", expected.c_str (), expected.length ());
   CPPUNIT_ASSERT_EQUAL_MESSAGE("Trim failed", 0, res.compare (expected));
   CPPUNIT_ASSERT_EQUAL_MESSAGE("Trim failed", expected.length (), res.length ());

   toTest = "   \t \naze dfglkjhdfg  \t kjshd \rlkjhdfgh  dsgsd   \r\n \t   ";
   expected = "aze dfglkjhdfg  \t kjshd \rlkjhdfgh  dsgsd";
   res = fString::trim (toTest);
   //   printf ("\ntoTest: '%s'\n", toTest.c_str ());
   //   printf ("res: '%s' l:%lu\n", res.c_str (), res.length ());
   //   printf ("exp: '%s' l:%lu\n", expected.c_str (), expected.length ());
   CPPUNIT_ASSERT_EQUAL_MESSAGE("Trim failed", 0, res.compare (expected));
   CPPUNIT_ASSERT_EQUAL_MESSAGE("Trim failed", expected.length (), res.length ());
}

