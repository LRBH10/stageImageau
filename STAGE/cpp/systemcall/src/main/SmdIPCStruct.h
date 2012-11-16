#ifndef SETRUCTIPCS_H_
#define SETRUCTIPCS_H_
#include <iostream>
#include <cstdio>
#include <cstring>
#include "commtype.h"
using namespace std;

#define TITLE_SIZE 7168
#define CONTENTS_SIZE 7168
#define MAX_IPCS_STRUCT_SIZE 8192

namespace systemcall
{
   /**
    * @enum type of the message in IPCS
    */
   enum SmdIPCStructTypes
   {
      /**
       * reference @ref SmdIPCGlobal
       */
      SmdIPCStruct_global = 0,

      /**
       * reference @ref SmdIPCString
       */
      SmdIPCStruct_string = 1,

      /**
       * reference @ref SmdIPCProgress
       */
      SmdIPCStruct_progress = 2,
      /**
       * reference @ref SmdIPCBool
       */
      SmdIPCStruct_bool = 3

   };

   /**
    * @struct Use to encapsulate other data struct
    */
   struct defaultIPCS
   {
         long mtype;
         char mtext [MAX_IPCS_STRUCT_SIZE];
   };

   /**
    * @struct Structure of the first Message
    */
   struct SmdIPCGlobal
   {
         /**
          * type of Message type send @ref SmdIPCStructTypes
          */
         SmdIPCStructTypes type_Ipc_struct;

         /**
          * size of Message auto-calculated by the type of Message
          */
         int size;

   };

   /**
    * @struct To send a string message
    */
   struct SmdIPCString
   {
         /**
          *  Message with max size 1024
          */
         char contents [CONTENTS_SIZE];
   };

   /**
    * @struct To send a progress operation for SMD
    */
   struct SmdIPCProgress
   {
         /**
          * id of Operation in Progress (Must be unique)
          */
         uint32 id;

         /**
          * title of progress (that will be shown in Web site)
          */
         char title [TITLE_SIZE];

         /**
          * value of Progression for the progress operation
          */
         double progress_value;
   };

   /**
    * @struct to send a boolean
    */
   struct SmdIPCBool
   {
         /**
          * true or false (it is used to stop the thread listener of SMDLauncher for example)
          */
         bool request;
   };

   /**
    * @struct Response Struct
    */
   struct SmdIPCGlobalResponse
   {
         /**
          * type of Message type send @ref SmdIPCStructTypes
          */
         SmdIPCStructTypes type;

         /**
          * @union the different Message supported
          */
         union
         {
               /**
                * for the Progress
                */
               SmdIPCProgress RProgress;
               /**
                * for the String
                */
               SmdIPCString RString;
               /**
                * for the Boolean
                */
               SmdIPCBool RBool;

         };

         /**
          * to debug (show the Struct Properties )
          */
         void debug ()
         {
            switch (this->type) {
               case SmdIPCStruct_string:
                  cout << "Contents : " << this->RString.contents << endl;
                  break;
               case SmdIPCStruct_progress:
                  cout << "Id : " << this->RProgress.id << endl;
                  cout << "Title : " << this->RProgress.title << endl;
                  cout << "Value : " << this->RProgress.progress_value << endl;
                  break;

               case SmdIPCStruct_bool:
                  cout << "REQUST (0 false;  1 True) : " << this->RBool.request << endl;
                  break;

               default:
                  break;
            }
         }
   };
}

#endif /* SETRUCTIPCS_H_ */
