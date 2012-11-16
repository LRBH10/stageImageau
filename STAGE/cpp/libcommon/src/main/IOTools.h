/**
 * IOTools.h
 *
 * $Id: IOTools.h 395 2012-03-30 13:04:47Z bdm $
 */

#ifndef IOTOOLS_H_
#define IOTOOLS_H_

#include "commtype.h"
#include "StationException.h"
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <sys/io.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <fstream>
#include "sys/stat.h"

using namespace std;

namespace imaGeau
{
   /**
    * IOTools
    * @since 30 mai 2011
    * @author bdm
    */
   class IOTools
   {
         static struct termios orig_termios;

      public:
         IOTools ();
         virtual ~IOTools ();

         /**
          * read line without comment (not starting with sharp '#')
          * @param f
          * @param buffer
          */
         static void readValidLine (FILE * f, string & buffer);

         /**
          * read line
          * @param f
          * @param buffer
          */
         static void readLine (FILE * f, string & buffer);

         /**
          * Copy a file
          * @param ficS source file
          * @param ficD destination file
          */
         static void fileCopy (const string & ficS, const string & ficD);

         static void resetTerminalMode ();

         static void setConioTerminalMode ();

         static int kbhit ();

         /**
          * mkpath - ensure all directories in path exist
          * Algorithm takes the pessimistic view and works top-down to ensure
          * each directory in path exists, rather than optimistically creating
          * the last element and working backwards.
          */
         static int mkpath (const string & path, mode_t mode);

         static int doMkdir (const string & path, mode_t mode);

         static void fpurge (FILE *fp = NULL);
   };
}

using namespace imaGeau;

#endif /* IOTOOLS_H_ */
