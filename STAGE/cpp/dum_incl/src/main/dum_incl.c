#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
//#include <io.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <assert.h>

#ifdef _LINUX_
#  define SEPARATOR '/'
#else
#  ifdef _CYGWIN_
#    define SEPARATOR '/'
#  else
#    define SEPARATOR '\\'
#  endif
#endif

#ifndef __FUNC__
#  ifdef WIN32
#    define __FUNC__ __FUNCTION__
#  endif
#  ifdef _LINUX_
#    define __FUNC__ __ASSERT_FUNCTION
#  endif
#  ifdef _CYGWIN_
#    define __FUNC__ __PRETTY_FUNCTION__
#  endif
#endif

#define MAX_BUF_SIZE 512

typedef struct _dirStruct
{
   char path [MAX_BUF_SIZE];
   char ext [64];
   unsigned char rec;
   struct _dirStruct * next;
} dirStruct;

char tmpStr [MAX_BUF_SIZE];
char tmpStr2 [MAX_BUF_SIZE];
char includeDir [MAX_BUF_SIZE];
char *libDest;
char *libSrc;
unsigned char lib, include, configFile, cleanBefore, verbose;
dirStruct * dirHead;
char * selectExt;

void usage (const char * t);

void errSys (const char *t, unsigned char ex, const char * func, int line);
#define ERR_SYS(t,ex) errSys(t,ex,__FUNC__,__LINE__)

void ok_log (const char * format, ...);
void error_log (const char * format, ...);

int changeDir (const char *t, unsigned char ex, const char * func, int line);
#define CHANGE_DIR(t,ex) changeDir(t,ex,__FUNC__,__LINE__)

void recurDir (int lev, char *curDir, char * destDir, unsigned char recursive, char * ext);
void getParams (int argc, char ** argv);
char * getDir (char * tmp, char * out);
char * getExt (char * tmp, char * out);
char * getFile (char * tmp, char * out);
void trim (char * tmp);
int checkIfExist (char * curDir, char * h_file_name, char * destDir);
char * strlwr (char * str);
int selectDir (const struct dirent *);
void freeDirHead (dirStruct * dir);

// *************************** //
// ********** MAIN *********** //
// *************************** //
int main (int argc, char ** argv)
{
   char baseDir [MAX_BUF_SIZE];
   char curDir [MAX_BUF_SIZE];
   char destDir [MAX_BUF_SIZE];
   char tmpSys [MAX_BUF_SIZE];
   dirStruct * tmpDir = NULL;

   dirHead = NULL;
   if (argc < 3) {
      usage (NULL);
   }

   getParams (argc, argv);

   if (include == 0 && lib == 0 && cleanBefore == 0) {
      usage ("You should ask me something ...");
   }

   getcwd (baseDir, MAX_BUF_SIZE);

   // *************************************************************************************** //
   if (cleanBefore) {
      CHANGE_DIR(includeDir, 1);

      // include.lnk
      getcwd (destDir, MAX_BUF_SIZE);
      sprintf (tmpSys, "del /q *.*");
      if (system (tmpSys) != 0) {
         ERR_SYS("clean before", 0);
      }
   }

   if (include) {
      if (!cleanBefore) {
         CHANGE_DIR(includeDir, 1);

         // include.lnk
         getcwd (destDir, MAX_BUF_SIZE);
      }

      ok_log ("----------> Making dummy header files to `%s' ...\n", destDir);
      tmpDir = dirHead;
      while (tmpDir != NULL) {
         chdir (baseDir); // .
         if (CHANGE_DIR (tmpDir->path, 0) == 0) {
            getcwd (curDir, MAX_BUF_SIZE);
            recurDir (0, curDir, destDir, tmpDir->rec, tmpDir->ext);
         }
         tmpDir = tmpDir->next;
      }
      ok_log ("<---------- Done !\n");
   }

   // *************************************************************************************** //
   if (lib) {
      chdir (baseDir); // .

      CHANGE_DIR(libDest, 1);
      // dest
      getcwd (destDir, MAX_BUF_SIZE);

      chdir (baseDir); // .

      CHANGE_DIR(getDir (libSrc, tmpStr), 1);

      // src ; tmpStr = getDir (libSrc)
      getcwd (curDir, MAX_BUF_SIZE);

      ok_log ("\n");
      ok_log ("-------> Copying library files from `%s' to `%s' ...\n", curDir, destDir);
      getFile (libSrc, tmpStr);
      /* > \\del_me.tmp 2> \\del_me2.tmp */
      sprintf (tmpSys, "copy \"%s%c%s.lib\" \"%s\"", curDir, SEPARATOR, tmpStr, destDir);
      if (system (tmpSys) != 0) {
         ERR_SYS("copy lib", 1);
      }
      sprintf (tmpSys, "copy \"%s%c%s.dll\" \"%s\"", curDir, SEPARATOR, tmpStr, destDir);
      if (system (tmpSys) != 0) {
         ERR_SYS("copy dll", 1);
      }
      /*      sprintf (tmp, "@del \\del_me.tmp \\del_me2.tmp");
       system (tmpSys);*/

      ok_log ("<------- Done `%s.lib' and `%s.dll' ! \n", tmpStr, tmpStr);
   }

   freeDirHead (dirHead);

   return 0;
}

void freeDirHead (dirStruct * dir)
{
   if (dir != NULL) {
      freeDirHead (dir->next);
      free (dir);
   }
}

// ************************************************************************ //
// ************************** RECURDIR ************************************ //
// ************************************************************************ //
void recurDir (int lev, char *curDir, char * destDir, unsigned char recursive, char * ext)
{
   char subDir [MAX_BUF_SIZE];
   char tmpSys [MAX_BUF_SIZE];
   char space [20];
   char tmpExt [64];
   int nb;
   struct dirent **namelist;
   struct stat buf;
   int n;

   for (nb = 0; nb < lev && nb < 20; nb++) {
      space [nb] = ' ';
   }
   space [nb] = '\0';
   nb = 0;

   ok_log ("%s[%d> Entering `%s' [%s] ...\n", space, lev, curDir, ext);
   //   ok_log ("%s[%d> From `%s' ...\n", space, lev, curDir);

   //   selectExt = ext;
   n = scandir (".", &namelist, 0, 0);

   if (n < 0) {
      ok_log ("No %s files in `%s' directory!\n", ext, curDir);
   }
   else {
      while (n--) {
         if (strcmp (namelist [n]->d_name, "..") && strcmp (namelist [n]->d_name, ".")) {
            stat (namelist [n]->d_name, &buf);
            if (S_ISDIR (buf.st_mode)) {
               if (recursive) {
                  if (CHANGE_DIR (namelist [n]->d_name, 0) == 0) {
                     getcwd (subDir, MAX_BUF_SIZE);
                     recurDir (lev + 1, subDir, destDir, recursive, ext);
                     CHANGE_DIR("..", 0);
                     // back
                  }
               }
            }
            else if (S_ISREG (buf.st_mode) && !strcmp (getExt (namelist [n]->d_name, tmpExt), ext)) {
               ok_log ("%s * Doing %s ... ", space, namelist [n]->d_name);
               if (!checkIfExist (curDir, namelist [n]->d_name, destDir)) {
                  sprintf (tmpSys, "%s%c%s", destDir, SEPARATOR, namelist [n]->d_name);
                  FILE * tmpF = fopen (tmpSys, "w");
                  fprintf (tmpF, "#include \"%s%c%s\"\n", curDir, SEPARATOR, namelist [n]->d_name);
                  //                     sprintf (tmpSys, "echo #include \"%s%c%s\" > %s%c%s", curDir, SEPARATOR, namelist [n]->d_name, destDir, SEPARATOR, namelist [n]->d_name);
                  //		     ok_log ("%s ", tmpSys);
                  //                     if (system (tmpSys) != 0)
                  //                        ERR_SYS ("echo", 1);
                  fclose (tmpF);
                  ++nb;
                  if (verbose == 1) {
                     printf ("done !\n");
                  }
               }
               else {
                  if (verbose == 1) {
                     printf ("nothing to do !\n");
                  }
               }
            }
         }
         free (namelist [n]);
      }
      free (namelist);

      if (nb != 0) {
         ok_log ("%s{%d} Done %d file(s) !\n", space, lev, nb);
      }
      else {
         ok_log ("%s{%d} Nothing to be done !\n", space, lev);
      }
      ok_log ("%s<%d] Leaving `%s' [%s].\n\n", space, lev, curDir, ext);
   }
}

// ********************* //
// ***** PARSEFILE ***** //
// ********************* //
void parseFile (char * tmp)
{
   FILE * conf;
   unsigned char recursive, localRec;
   char firstWord [64];
   char ext [64];
   char * nextWord;
   int line = 0;
   dirStruct * tmpDir;

   memset (ext, '\0', 64);
   recursive = localRec = 0;

   conf = fopen (tmp, "r");
   if (conf == NULL) {
      ERR_SYS("opening conf file", 1);
   }

   while (!feof (conf)) {
      ++line;
      memset (tmpStr, '\0', MAX_BUF_SIZE);
      memset (firstWord, '\0', 64);

      fscanf (conf, "%511[^\n]", tmpStr);
      fgetc (conf);
      if (tmpStr [0] != '\0' && tmpStr [0] != '#') {
         trim (tmpStr);
         if (tmpStr [0] == '$') {
            sscanf (tmpStr, "%63s", firstWord);
            nextWord = strstr (tmpStr, " ");
            if (nextWord == NULL) {
               error_log ("Format error line %d : flag definition without right value\n", line);
            }
            else {
               strlwr (firstWord);

               while (*nextWord == ' ') {
                  ++nextWord;
               }

               switch (firstWord [1]) {
                  case 'c': // cleanBefore
                     strlwr (nextWord);
                     if (!strcmp (nextWord, "on"))
                        cleanBefore = 1;
                     else
                        cleanBefore = 0;
                     break;

                  case 'r': // recursive
                     strlwr (nextWord);
                     if (!strcmp (nextWord, "on"))
                        recursive = 1;
                     else
                        recursive = 0;
                     break;

                  case 'l': // localRec
                     strlwr (nextWord);
                     if (!strcmp (nextWord, "on"))
                        localRec = 1;
                     else
                        localRec = 0;
                     break;

                  case 'i': // includeDir
                     strcpy (includeDir, nextWord);
                     break;

                  default:
                     error_log ("Format error line %d : unrecognize token or misplaced src dir\n", line);
                     break;
               }
            }
         }
         else if (tmpStr [0] == '[' && tmpStr [strlen (tmpStr) - 1] == ']') { // extension section
            localRec = recursive;
            memset (ext, '\0', 64);
            strncpy (ext, &(tmpStr [1]), strlen (tmpStr) - 2);
         }
         else // src dir
         {
            if (ext [0] == '\0') {
               error_log ("Format error line %d : unrecognize token or misplaced src dir\n", line);
            }
            else {
               tmpDir = dirHead;
               dirHead = (dirStruct *) malloc (sizeof(dirStruct));
               strcpy (dirHead->path, tmpStr);
               strcpy (dirHead->ext, ext);
               dirHead->rec = localRec;
               dirHead->next = tmpDir;
            }
         }
      }
   }

   if (conf) {
      fclose (conf);
   }

   if (includeDir [0] == '\0') {
      ERR_SYS("no include dir defined", 1);
   }
   else {
      include = 1;
   }
}

// ********************************* //
// ********** GETPARAM ************* //
// ********************************* //
void getParams (int argc, char ** argv)
{
   int i;
   unsigned char recursive;
   int endIncl;
   dirStruct * tmpDir;

   i = 1;

   include = 0;
   cleanBefore = 0;
   recursive = 0;
   configFile = 0;
   lib = 0;
   endIncl = 0;
   verbose = 0;

   while (configFile == 0 && argv [i] [0] == '-') {
      switch (argv [i] [1]) {
         case 'v':
         case 'V': {
            verbose = 1;
            endIncl = argc - 1;
            break;
         }

         case 'c':
         case 'C': {
            if (argc < 2) {
               usage ("needs at least 2 parameters");
            }
            cleanBefore = 1;
            endIncl = argc - 1;
            break;
         }

         case 'f':
         case 'F': {
            if (argc < 3) {
               usage ("needs at least 2 parameters");
            }
            parseFile (argv [i + 1]);
            configFile = 1;
            break;
         }

         case 'i':
         case 'I': {
            if (argc < 4) {
               usage ("needs at least 3 parameters");
            }
            include = 1;
            endIncl = argc - 1;
            break;
         }

         case 'r':
         case 'R': {
            if (argc < 5) {
               usage ("needs at least 4 parameters");
            }
            recursive = 1;
            endIncl = argc - 1;
            break;
         }

         case 'l':
         case 'L': {
            if ((recursive && argc < 8) || (include && argc < 7)) {
               usage ("needs more parameters");
            }
            lib = 1;
            libDest = argv [argc - 1];
            libSrc = argv [argc - 2];
            if (recursive || include) {
               endIncl -= 2;
            }
            break;
         }

         default:
            usage (NULL);
            break;

      }

      ++i;
   }

   if (recursive && include == 0) {
      usage ("r/R needs i/I option");
   }

   if (!configFile) {
      strcpy (includeDir, argv [endIncl]);
      for (; i < endIncl; i++) {
         tmpDir = dirHead;
         dirHead = (dirStruct *) malloc (sizeof(dirStruct));
         getDir (argv [i], dirHead->path);
         getExt (argv [i], dirHead->ext);
         dirHead->rec = recursive;
         dirHead->next = tmpDir;
      }
   }
}

// *************** //
// **** USAGE **** //
// *************** //
void usage (const char * t)
{
   if (t != NULL) {
      printf ("Syntax error : %s.\n\n", t);
   }
   printf ("\nUSAGE : dum_incl OPTIONS [inclSrc1/*.UVW [inclSrc2/*.XYZ [...]] inclDest] [libSrc libDest]\n\n");
   printf ("Makes dummy header file from `inclSrcX/XXX.UVW' to `inclDest/XXX.UVW'.\n");
   printf ("`inclDest/XXX.UVW' will contain `#include \"inclSrc/XXX.UVW\"'.\n\n");
   printf ("\t OPTIONS :\n");
   printf ("\t\t -c/-C \t Clean directory inclDest before the copy.\n");
   printf ("\t\t -i/-I \t Make dummy headers.\n");
   printf ("\t\t -r/-R \t Recursive mode for include files.\n");
   printf ("\t\t -f/-f \t Use config file instead.\n");
   printf ("\t\t -l/-L \t Copy librairies from FILES libSrc.lib libSrc.dll to libDest.\n");
   exit (-1);
}

// ************************************ //
// *********** CHANGEDIR ************** //
// ************************************ //
int changeDir (const char *t, unsigned char ex, const char * func, int line)
{
   int ret = chdir (t);
   if (ret != 0) {
      error_log ("%s:%d: while opening directory `%s', %s.\n", func, line, t, (ex ? "exiting" : "continuing"));
      if (ex) {
         exit (-1);
      }
   }
   return ret;
}

// ********************************* //
// *********** ERRSYS ************** //
// ********************************* //
void errSys (const char *t, unsigned char ex, const char * func, int line)
{
   error_log ("%s:%d: while executing `%s', %s.\n", func, line, t, (ex ? "exiting" : "continuing"));
   if (ex) {
      exit (-1);
   }
}

// ********************* //
// ****** GETDIR ******* //
// ********************* //
char * getDir (char * tmp, char * out)
{
   int i = strlen (tmp) - 1;

   while (i > 0 && tmp [i] != SEPARATOR) {
      --i;
   }

   strncpy (out, tmp, i);
   out [i] = '\0';
   return out;
}

// ********************* //
// ****** GETEXT ******* //
// ********************* //
char * getExt (char * tmp, char * out)
{
   int i, j, l = strlen (tmp);

   i = l - 1;
   while (i > 0 && tmp [i] != '.' && tmp [i] != SEPARATOR) {
      --i;
   }

   if (tmp [i] == '.') {
      //      out [0] = '*';
      j = 0;
      while (tmp [i + j] != '\0' && tmp [i + j] != '"') {
         out [j] = tmp [i + j];
         ++j;
      }
      out [j] = '\0';
      out = strlwr (out);
   }
   else {
      strcpy (out, "\0");
   }
   return out;
}

// ********************** //
// ****** GETFILE ******* //
// ********************** //
char * getFile (char * tmp, char * out)
{
   int i, j, l = strlen (tmp);

   i = l - 1;
   while (i > 0 && tmp [i] != SEPARATOR) {
      --i;
   }

   ++i;
   j = 0;
   while (tmp [i + j] != '.') {
      out [j] = tmp [i + j];
      ++j;
   }

   out [j] = '\0';
   return out;
}

void trim (char * tmp)
{
   int j, i = 0;
   int l = strlen (tmp) - 1;

   while (i < l && tmp [i] == ' ') {
      ++i;
   }

   while (tmp [l] == ' ') {
      --l;
   }

   for (j = i; i != 0 && j <= l && tmp [j] != ' '; j++) {
      tmp [j - i] = tmp [j];
   }

   tmp [l + 1] = '\0';
}

int checkIfExist (char * curDir, char * h_file_name, char * destDir)
{
   FILE * exist;
   int out = 0;

   snprintf (tmpStr, MAX_BUF_SIZE, "%s%c%s", destDir, SEPARATOR, h_file_name);
   exist = fopen (tmpStr, "r");
   if (exist != NULL) {
      fscanf (exist, "%511[^\n]", tmpStr2);
      sprintf (tmpStr, "#include \"%s%c%s\"", curDir, SEPARATOR, h_file_name);
      if (!strcmp (tmpStr, tmpStr2)) {
         out = 1;
      }
      fclose (exist);
   }
   return out;
}

char * strlwr (char * str)
{
   int i;
   int lg = strlen (str);
   for (i = 0; i < lg; ++i) {
      str [i] = tolower (str [i]);
   }

   return str;
}

int selectDir (const struct dirent * t)
{
   int s = strlen (t->d_name);
   int s2 = strlen (selectExt);
   return (s > s2 && strcmp (t->d_name + s - s2, selectExt) == 0);
}

void ok_log (const char * format, ...)
{
   if (verbose == 1) {
      char tmp [256];
      va_list list_arg;
      va_start(list_arg, format);

      memset (tmp, '\0', 256);
      strcpy ((char *) tmp, " dum_incl: \0");
      strcat ((char *) tmp, format);
      vprintf ((char *) tmp, list_arg);
   }
}

void error_log (const char * format, ...)
{
   char tmp [256];
   va_list list_arg;
   va_start(list_arg, format);

   memset (tmp, '\0', 256);
   strcpy ((char *) tmp, " dum_incl: ERROR: \0");
   strcat ((char *) tmp, format);
   vfprintf (stderr, (char *) tmp, list_arg);
}
