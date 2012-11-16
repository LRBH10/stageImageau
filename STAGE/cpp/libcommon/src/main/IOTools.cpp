#include "IOTools.h"

struct termios IOTools::orig_termios;

IOTools::IOTools ()
{
}

IOTools::~IOTools ()
{
}

void IOTools::readLine (FILE * f, std::string & buffer)
{
   int i = -1;
   char c;
   if (f != NULL) {
      buffer.clear ();
      c = fgetc (f);
      for (i = 0; c != EOF && c != '\n' && c != '\r'; ++i) {
         buffer.push_back (c);
         c = fgetc (f);
      }
   }
}

void IOTools::readValidLine (FILE * f, std::string & buffer)
{
   int i = -1;
   char c;
   if (f != NULL) {
      do {
         buffer.clear ();
         c = fgetc (f);
         for (i = 0; c != EOF && c != '\n' && c != '\r'; ++i) {
            buffer.push_back (c);
            c = fgetc (f);
         }
      }
      while ((i == 0 && c != EOF) || buffer [0] == '#');

      if (buffer [0] == '#') { // ie.: last line is a comment
         buffer.clear ();
      }
   }
   //printf ("readline ______\n");
}

void IOTools::fileCopy (const string & ficS, const string & ficD)
{
   std::ifstream src;
   std::ofstream dest;

   const int LEN = 8192;
   char buffer_out [LEN];
   char buffer_in [LEN];

   if (LEN) {
      src.rdbuf ()->pubsetbuf (buffer_in, LEN);
      dest.rdbuf ()->pubsetbuf (buffer_out, LEN);
   }
   else {
      src.rdbuf ()->pubsetbuf (NULL, 0);
      dest.rdbuf ()->pubsetbuf (NULL, 0);
   }

   src.open (ficS.c_str (), ios::binary);
   dest.open (ficD.c_str (), ios::binary);
   dest << src.rdbuf ();

   dest.flush ();
   src.close ();
   dest.close ();
}

void IOTools::resetTerminalMode ()
{
   tcsetattr (0, TCSANOW, &IOTools::orig_termios);
}

void IOTools::setConioTerminalMode ()
{
   struct termios new_termios;

   /* take two copies - one for now, one for later */
   tcgetattr (0, &IOTools::orig_termios);
   memcpy (&new_termios, &IOTools::orig_termios, sizeof(new_termios));

   /* register cleanup handler, and set the new terminal mode */
   //atexit (resetTerminalMode);
   cfmakeraw (&new_termios);
   tcsetattr (0, TCSANOW, &new_termios);
}

int IOTools::kbhit ()
{
   struct timeval tv = { 0L, 0L };
   fd_set fds;
   FD_ZERO(&fds);
   FD_SET(0, &fds);
   return select (1, &fds, NULL, NULL, &tv);
}

int IOTools::doMkdir (const string & path, mode_t mode)
{
   struct stat st;
   int status = 0;

   if (stat (path.c_str (), &st) != 0) {
      /* Directory does not exist */
      if (mkdir (path.c_str (), mode) != 0) {
         status = -1;
      }
   }
   else if (!S_ISDIR (st.st_mode)) {
      errno = ENOTDIR;
      status = -1;
   }

   return (status);
}

/**
 ** mkpath - ensure all directories in path exist
 ** Algorithm takes the pessimistic view and works top-down to ensure
 ** each directory in path exists, rather than optimistically creating
 ** the last element and working backwards.
 */
int IOTools::mkpath (const string & path, mode_t mode)
{
   int status;
   size_t pos = 0, oldPos = 0;

   status = 0;
   pos = path.find ('/', oldPos);
   while (status == 0 && pos != string::npos) {
      if (pos != oldPos) {
         /* Neither root nor double slash in path */
         status = IOTools::doMkdir (path.substr (0, pos), mode);
      }
      oldPos = pos + 1;
      pos = path.find ('/', oldPos);
   }

   if (status == 0) {
      status = IOTools::doMkdir (path, mode);
   }

   return (status);
}

void IOTools::fpurge (FILE *fp)
{
   FILE * stream;
   if (fp == NULL) {
      stream = stdin;
   }
   else {
      stream = fp;
   }
   int c;

   while ((c = fgetc (stream)) != '\n' && c != EOF) {
   }
}

