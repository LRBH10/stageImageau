/*
 * OperationsTest.cpp
 *
 *  Created on: 25 juin 2012
 *      Author: bibouh
 */

#include "OperationsTest.h"
#include "OperationsIncludes.h"
#include "OperationSvnUpdate.h"
#include "OperationSvnSwitch.h"
#include <cppunit/config/SourcePrefix.h>

using namespace OperationsSMD;

CPPUNIT_TEST_SUITE_REGISTRATION (OperationsTest);

OperationsTest::OperationsTest ()
{

}

OperationsTest::~OperationsTest ()
{

}

void OperationsTest::testOperationScript ()
{
   long id = -1;
   OperationType type = OperationTypeScript;
   string user = "rabah";
   string path = "~/";
   double state = 0.65;
   bool achieved = false;
   bool nested = false;
   bool report_on_succes = true;
   string cmd = "echo ";
   string pars = "RABAH LAOUADI";

   OperationScript test (id, type, user, path, state, achieved, nested, report_on_succes, cmd, pars);
   test.setInTest (true);

   ReportData * report = test.execute ();

   string attendue = "PRE CHECK ...\nRUN OPERATIONS ...\nRABAH LAOUADI\nPOST_CHECK ...\n";

   CPPUNIT_ASSERT_EQUAL_MESSAGE ("OperationScript ", attendue, report->contents);
   delete report;
}

void OperationsTest::testOperationShellFile ()
{
   long id = -1;
   OperationType type = OperationTypeShellFile;
   string user = "rabah";
   string path = "~/";
   double state = 0.65;
   bool achieved = false;
   bool nested = false;
   bool report_on_succes = true;

   ShellFileAction action = ShellFileReplace;
   string filename = "test.sh";
   string contenu = "echo RABAH LAOUADI";
   bool excute = true;
   bool delete_after = false;

   OperationShellFile test (id, type, user, path, state, achieved, nested, report_on_succes, action, filename, contenu, excute,
      delete_after);
   test.setInTest (true);

   ReportData * report = test.execute ();

   string attendue = "PRE CHECK ...\nRUN OPERATIONS ...\nRABAH LAOUADI\nPOST_CHECK ...\n";
   CPPUNIT_ASSERT_EQUAL_MESSAGE ("OperationScript ", attendue, report->contents);
   delete report;
}

void OperationsTest::testOperationUpdateSVN ()
{
   long id = -1;
   OperationType type = OperationTypeSvnUpdate;
   string user = "rabah";
   string path = "~/GIT/Projects-Rabah/STAGE/cpp/";
   double state = 0.65;
   bool achieved = false;
   bool nested = false;
   bool report_on_succes = true;

   OperationType svn_type = OperationTypeSvnUpdate;
   string head = "HEAD";
   string conflict = "theirs-full";
   bool forced = true;

   OperationSvnUpdate test (id, type, user, path, state, achieved, nested, report_on_succes, svn_type, conflict, head, forced);
   test.setInTest (true);

   ReportData * report = test.execute ();
   cout << report->contents << endl;

   string attendue = "PRE CHECK ...\nRUN OPERATIONS ...\nRABAH LAOUADI\nPOST_CHECK ...\n";
   //CPPUNIT_ASSERT_EQUAL_MESSAGE("OperationScript ", attendue, report->contents);
   delete report;
}
