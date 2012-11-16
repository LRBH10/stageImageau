#include "ProcessManager.h"

LoggerPtr ProcessManager::logger (LogFactory::getLogger ("ProcessManager"));
bool ProcessManager::isRunning = true;
int ProcessManager::runningProcesses = 0;
map<pthread_t, ThreadInfo *> ProcessManager::processes;
boost::interprocess::interprocess_mutex ProcessManager::mutex;

ProcessManager * ProcessManager::getInstance ()
{
   static ProcessManager inst;
   return &inst;
}

ProcessManager::ProcessManager ()
{
   boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock (ProcessManager::mutex);
   ProcessManager::isRunning = true;
   //ProcessManager::processes.clear ();
}

ProcessManager::~ProcessManager ()
{
   boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock (ProcessManager::mutex);
   ProcessManager::isRunning = false;

   sleep (1); // wait for the threads to stop
   pthread_yield ();
   time_t maxWait = time (NULL) + 5;

   while (ProcessManager::runningProcesses > 0 && time (NULL) <= maxWait) {
      LOG_INFO(logger, fString::format ("%d thread still running !!", ProcessManager::runningProcesses));
      pthread_yield ();
      usleep (1000 * 200);
      pthread_yield ();
   }

   int s;
   for (map<pthread_t, ThreadInfo *>::iterator ite = ProcessManager::processes.begin (); ite != ProcessManager::processes.end (); ++ite) {
      ThreadInfo * info = (*ite).second;
      if (info != NULL) {
         try {
            if (info->thread_id > 0 && pthread_tryjoin_np (info->thread_id, NULL) == EBUSY) {
               pthread_t thread_id = info->thread_id;
               s = pthread_join (thread_id, NULL);
               if (s != 0) {
                  LOG_ERROR(logger, fString::format ("thread %lu is a zombi. error:'%s'", thread_id, strerror (errno)));
               }
               else {
                  LOG_DEBUG(logger, fString::format ("thread %lu came back.", thread_id));
               }
            }
         }
         catch (...) {
            LOG_ERROR(logger, fString::format ("thread %lu generate a seg fault. error:'%s'", info->thread_id, strerror (errno)));
         }
         delete (info);
      }
   }

   if (ProcessManager::runningProcesses > 0) {
      LOG_ERROR(logger, fString::format ("%d thread still running !!", ProcessManager::runningProcesses));
   }
}

pthread_t ProcessManager::addNewProcess (const string & cmd, unsigned int waitInSec, void * userData, CallBackFunc resultCallBack)
{
   ThreadInfo * info = new ThreadInfo (0, cmd, waitInSec, userData, resultCallBack);

   pthread_create (&(info->thread_id), NULL, &ProcessManager::_threadProcess, info);

   usleep (1000 * 200);
   pthread_yield ();

   if (info->thread_id != 0) { // OK
      boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock (ProcessManager::mutex);
      ++ProcessManager::runningProcesses;
      ProcessManager::processes [info->thread_id] = info;
   }
   else {
      LOG_ERROR(logger, fString::format ("Cannot start thread for cmd '%s'.", cmd.c_str ()));
   }

   return info->thread_id;
}

void * ProcessManager::_threadProcess (void * inf)
{
   int infp, outfp;
   string result;
   string msgStr;

   ThreadInfo * info = (ThreadInfo *) inf;

   pid_t pid = ProcessManager::pipedFork (info->cmd.c_str (), &infp, &outfp);
   if (pid <= 0) { // error (0 is not returned)
      usleep (1000 * 500); // wait for the main thread to add threadId in the map
      pthread_yield ();
      msgStr = fString::format ("Cannot start cmd '%s'.", info->cmd.c_str ());
      LOG_ERROR(logger, msgStr);
   }
   else {
      size_t bufSize = 128;
      size_t s;
      char buffer [128];
      time_t maxWait = time (NULL) + info->waitInSec;
      int status = 0;
      bool isDead;

      LOG_TRACE(logger, fString::format ("pid:%d '%s' running", pid, info->cmd.c_str ()));
      usleep (1000 * 200);
      pthread_yield ();

      // ============= wait for data to read
      int bytesQueued = 0;
      do {
         usleep (1000 * 100);
         pthread_yield ();
         isDead = ISDEAD (pid, status); // is it running ?
         ioctl (outfp, FIONREAD, &bytesQueued);
      }
      while (!isDead // sub proc not exit
      && time (NULL) <= maxWait // still time to wait
      && ProcessManager::isRunning && bytesQueued == 0); // main process still running

      // ============= read data if any
      if (bytesQueued > 0) {
         LOG_TRACE(logger, fString::format ("pid:%d %d bytes to be read.", pid, bytesQueued));
         do {
            memset (buffer, 0, bufSize);
            if (!result.empty ()) { // some data already read, wait a while to see of more data will come ...
               usleep (1000 * 100);
               pthread_yield ();
               usleep (1000 * 100);
               pthread_yield ();
            }
            s = read (outfp, buffer, bufSize);
            if (s > 0) {
               result.append (buffer, s);
            }
            isDead = ISDEAD (pid, status); // is it running ?
         }
         while (s == bufSize // still more to read
         /*&& !isDead // sub proc not exit*/
         && time (NULL) <= maxWait // still time to wait
         && ProcessManager::isRunning); // main process still running

         LOG_TRACE( logger, fString::format ("pid:%d Read output ... isDead:%d, isRunning:%s, wait:%d, size:%d, output:\n%s", //
            pid, isDead, (ProcessManager::isRunning?"ok":"no"), (maxWait - time (NULL)), s, result.c_str()));
      }
      else {
         LOG_TRACE( logger, fString::format ("pid:%d No output ... isDead:%d, isRunning:%s, wait:%d", //
            pid, isDead, (ProcessManager::isRunning?"ok":"no"), (maxWait - time (NULL))));
      }

      // ============ wait for timeout or child termination
      do {
         usleep (1000 * 100);
         pthread_yield ();
         isDead = ISDEAD (pid, status); // is it running ?
      }
      while (!isDead // sub proc not exit
      && time (NULL) <= maxWait // still time to wait
      && ProcessManager::isRunning); // main process still running

      // ============= does it need to be killed ?
      if (!isDead) { // still running
         LOG_TRACE(logger, fString::format ("pid:%d still running. Will soft kill ...", pid));
         kill (pid, 15);
         usleep (1000 * 100);
         pthread_yield ();
         usleep (1000 * 100);
         pthread_yield ();

         isDead = ISDEAD (pid, status); // is it running ?
         if (!isDead) { // still running
            LOG_TRACE(logger, fString::format ("pid:%d still running. Will hard kill ...", pid));
            kill (pid, 9);
            msgStr = fString::format ("pid:%d Cmd '%s' hard killed!", pid, info->cmd.c_str ());
            LOG_INFO(logger, msgStr);
         }
         else {
            msgStr = fString::format ("pid:%d Cmd '%s' soft killed!", pid, info->cmd.c_str ());
            LOG_INFO(logger, msgStr);
         }
      }
      else {
         msgStr = fString::format ("pid:%d Cmd '%s' done!", pid, info->cmd.c_str ());
         LOG_INFO(logger, msgStr);
      }

      close (infp); // stdin unused
      close (outfp);
   }

   if (info->resultCallBack != NULL) {
      info->resultCallBack (info, result, msgStr);
   }

   {
      boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock (ProcessManager::mutex);
      --ProcessManager::runningProcesses;
   }

   return (void *) result.c_str ();
}

string ProcessManager::execWait (const string & cmd)
{
   LOG_TRACE(logger, fString::format ("execWait will run cmd:'%s'...", cmd.c_str ()));
   string result = "";
   FILE* pipe = popen (cmd.c_str (), "re");
   if (pipe) {
      char buffer [128];
      while (!feof (pipe)) {
         memset (buffer, 0, 128);
         if (fgets (buffer, 128, pipe) != NULL)
            result += buffer;
      }
      pclose (pipe);
   }
   else {
      result = "ERROR";
   }
   LOG_TRACE(logger, fString::format ("execWait quit! result:'%s'", result.c_str()));
   return result;
}

string ProcessManager::execWaitKill (const string & cmd, const string & stdinStr, unsigned int waitInSec)
{
   int infp, outfp;
   string result;

   LOG_TRACE(logger, fString::format ("execWaitKill will run fork cmd:'%s'...", cmd.c_str ()));
   pid_t pid = ProcessManager::pipedFork (cmd.c_str (), &infp, &outfp);
   if (pid <= 0) {
      result = "ERROR";
   }
   else {
      int status = 0;
      bool isDead = true;
      string ps;
      size_t bufSize = 128;
      size_t s;
      char buffer [128];
      time_t maxWait = time (NULL) + waitInSec;

      if (!stdinStr.empty ()) {
         write (infp, stdinStr.c_str (), stdinStr.size ());
      }
      close (infp);

      LOG_TRACE(logger, "execWaitKill wait for child end ...");
      isDead = ISDEAD(pid,status);
      int bytesQueued = 0;
      ioctl (outfp, FIONREAD, &bytesQueued);
      if (bytesQueued > 0) {
         do {
            memset (buffer, 0, bufSize);
            s = read (outfp, buffer, bufSize);
            if (s > 0) {
               result += buffer;
            }
            isDead = ISDEAD(pid,status);
         }
         while (s == bufSize && !isDead && time (NULL) < maxWait);
      }
      else {
         isDead = ISDEAD(pid,status);
         while (!isDead && time (NULL) < maxWait) {
            usleep (1000 * 100);
            pthread_yield ();
            isDead = ISDEAD(pid,status);
         }
      }
      close (outfp);

      LOG_TRACE(logger, fString::format ("execWaitKill child should have endded, isDead:%s", (isDead?"dead":"alive")));
      if (!isDead) { // still running
         kill (pid, SIGTERM);
         isDead = ISDEAD(pid,status);
         if (!isDead) { // still running
            kill (pid, SIGKILL);
         }
      }
   }
   LOG_TRACE(logger, fString::format ("execWaitKill quit! result:'%s'", result.c_str()));

   return result;
}

pid_t ProcessManager::pipedFork (const char *command, int *infp, int *outfp)
{
   int stdFd [2];
   pid_t pid;

   int retI = pipe2 (stdFd, O_CLOEXEC);
   if (retI != 0) {
      pid = -1;
      close (stdFd [FD_WRITE]);
      close (stdFd [FD_READ]);
   }
   else {
      pid = vfork ();

      if (pid > 0) { // parent process
         LOG_TRACE(logger, fString::format ("pid:%d Fork running (from parent)", pid));
         if (infp == NULL) {
            close (stdFd [FD_WRITE]);
         }
         else {
            *infp = stdFd [FD_WRITE];
         }

         if (outfp == NULL) {
            close (stdFd [FD_READ]);
         }
         else {
            *outfp = stdFd [FD_READ];
         }
      }
      else if (pid == 0) { // forked process
         LOG_TRACE(logger, fString::format("pid:%d Fork running (from child)", getpid ()));
         dup2 (stdFd [FD_READ], FD_READ);
         dup2 (stdFd [FD_WRITE], FD_WRITE);

         const char* argv [] = { "/bin/sh", "-c", command, NULL };
         int ret = execve ("/bin/sh", (char**) argv, NULL);
         LOG_ERROR(logger, fString::format ("Fork: execl failed! error: '%s'", strerror(errno)));

//         close (stdFd [FD_READ]);
//         close (stdFd [FD_WRITE]);
//
         _exit (ret);
      }
      else { // failure
         close (stdFd [FD_WRITE]);
         close (stdFd [FD_READ]);
         LOG_ERROR(logger, "Fork failed!");
      }
   }

   return pid;
}

void ProcessManager::setQuit ()
{
   ProcessManager::isRunning = false;
}

bool ProcessManager::isStillRunning ()
{
   return ProcessManager::isRunning;
}

int ProcessManager::getRunningProcesses ()
{
   return ProcessManager::runningProcesses;
}
