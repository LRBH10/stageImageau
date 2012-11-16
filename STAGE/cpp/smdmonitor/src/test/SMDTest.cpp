/*
 * SMDTest.cpp
 *
 *  Created on: 8 juin 2012
 *      Author: rli
 */

#include "SMDTest.h"
#include <cppunit/config/SourcePrefix.h>

using namespace OperationsSMD;
using namespace SMDExecute;

CPPUNIT_TEST_SUITE_REGISTRATION (SMDTest);

SMDTest::SMDTest ()
{

}

SMDTest::~SMDTest ()
{

}

void SMDTest::testConnect ()
{
   SMDLauncher smd;
   smd.connect ();

   CPPUNIT_ASSERT_EQUAL_MESSAGE ("Connected", true, smd.isConnected ());

}

void SMDTest::testWhat_i_will_do ()
{

}

void SMDTest::testDo_it ()
{

}

void SMDTest::testDisConnect ()
{
   SMDLauncher smd;
   smd.disconnect ();

   CPPUNIT_ASSERT_EQUAL_MESSAGE ("Connected", false, smd.isConnected ());

}
