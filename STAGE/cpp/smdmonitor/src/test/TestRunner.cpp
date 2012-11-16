#include "CppUnitTestHelper.h"
#include "LogFactory.h"

#ifdef _LINUX_
extern "C" void __libc_freeres (void);
#endif

using namespace std;

int main (int argc, char** argv)
{
#ifdef _LINUX_
   /// @todo above line for test purpose
   atexit (__libc_freeres);
#endif

   LogFactory::setLogFileName ("smdlauncherUnitTest.log");

   return CppUnitTestHelper::runTest (argc, argv, "cppunit_result_smdlauncher.xml");
}
