/**
 * ProcessManager.h
 *
 * $Id: ProcessManager.h 757 2012-07-25 12:19:27Z rli $
 */

#ifndef PROCESSMANAGER_H_
#define PROCESSMANAGER_H_

#include <fcntl.h>
#include <map>
#include <cerrno>
#include <pthread.h>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include "commtype.h"
#include "LogFactory.h"

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

/**
 * @ingroup commontools
 * @defgroup  ProcessManager command and process module
 */
//@{
/**
 * check is the process with this pid is finished
 */
#define ISDEAD(_pid,_status) (waitpid (_pid, &_status, WNOHANG) == _pid || waitpid (_pid, &_status, WNOHANG) == -1)

typedef struct _thread_info ThreadInfo;

/**
 * @ingroup commontools
 * Callback function when thread end
 * @param corresponding thread info ptr
 * @param data returns by the cmd
 * @param msg of ProcessManager
 */
typedef void (*CallBackFunc) (ThreadInfo *, const string &, const string &);

/**
 *  Used as argument to thread_start()
 */
struct _thread_info
{
      /// ID returned by pthread_create()
      pthread_t thread_id;

      /// From command-line argument
      string cmd;

      /// time to wait before kill
      unsigned int waitInSec;

      /// user data pointer
      void * userData;

      /// callback func when thread endded
      CallBackFunc resultCallBack;

      /**
       * Default constructor
       * @param id thread id
       * @param c command to launch
       * @param w time to wait before killing
       * @param ud user data pointer
       * @param result callback function pointer
       */
      _thread_info (pthread_t id = 0, const string & c = "", unsigned int w = 10, void * ud = NULL, CallBackFunc result = NULL) :
         thread_id (id), cmd (c), waitInSec (w), userData (ud), resultCallBack (result)
      {
      }

      /**
       * Copy constructor
       * @param inf _thread_info to copy
       */
      _thread_info (const _thread_info& inf) :
         thread_id (inf.thread_id), cmd (inf.cmd), waitInSec (inf.waitInSec), userData (inf.userData), resultCallBack (inf.resultCallBack)
      {
      }

      /**
       * Copy operator
       * @param inf _thread_info to copy
       */
      _thread_info& operator= (const _thread_info& inf)
      {
         this->thread_id = inf.thread_id;
         this->cmd = inf.cmd;
         this->waitInSec = inf.waitInSec;
         this->userData = inf.userData;
         this->resultCallBack = inf.resultCallBack;
         return *this;
      }
};

/**
 * ProcessManager
 * @since 24 mai 2011
 * @author bdm
 */
class ProcessManager
{
   protected:
      static boost::interprocess::interprocess_mutex mutex;

      static LoggerPtr logger;

      /// if true ProcessManager run, if false thread receive order to stop
      static bool isRunning;

      /// number of running process
      static int runningProcesses;

      /// map of the running thread with their threadInfo struct
      static map<pthread_t, ThreadInfo *> processes;

      /// thread function
      static void * _threadProcess (void * cmd);

      /// fake copy operator
      ProcessManager& operator= (const ProcessManager&);

      /// fake copy operator
      ProcessManager (const ProcessManager&);

      /**
       * default constructor
       */
      ProcessManager ();

   public:
      static ProcessManager * getInstance ();

      /**
       * default destructor
       */
      virtual ~ProcessManager ();

      /**
       * Add a new process running the corresponding command.
       * @param cmd the command to run
       * @param waitInSec time to wait before kill
       * @param userData user data pointer
       * @param resultCallBack result callback function pointer
       * @return thread id if ok
       */
      virtual pthread_t addNewProcess (const string & cmd, unsigned int waitInSec = 60, void * userData = NULL,
         CallBackFunc resultCallBack = NULL);

      /**
       * Control the process manager running state
       */
      static void setQuit ();

      /**
       * @return the current running state
       */
      static bool isStillRunning ();

      /**
       * @return the number of running processes
       */
      static int getRunningProcesses ();

      /**
       * Execute a command and wait for its stop (autonomous / external of ProcessManager)
       * @param cmd command to launch
       * @return stdout of the command
       */
      static string execWait (const string & cmd);

      /**
       * Execute a command and wait for its stop until timeout (autonomous / external of ProcessManager)
       * @param cmd command to launch
       * @param stdinStr stdin for the cmd
       * @param waitInSec wait before kill
       * @return stdout of the command
       */
      static string execWaitKill (const string & cmd, const string & stdinStr = "", unsigned int waitInSec = 10);

      /**
       * Open pipe onto a command and return the input and output file descriptors (autonomous / external of ProcessManager)
       * @param command command to launch
       * @param infp [output] input file descriptor if not NULL
       * @param outfp [output] output file descriptor if not NULL
       * @return pid of the new process created
       */
      static pid_t pipedFork (const char *command, int *infp, int *outfp);

};

//@}

#endif /* PROCESSMANAGER_H_ */
