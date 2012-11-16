/*
 * ServerIPCs.cpp
 *
 *  Created on: 18 juin 2012
 *      Author: rli
 */

#include "QueueMessage.h"

using namespace systemcall;

LoggerPtr QueueMessage::logger (LogFactory::getLogger ("QueueMessage"));

QueueMessage::QueueMessage () :
   key_identifier (SMD_IPC_IDENTIFIER), id_communications (0)
{

}

QueueMessage::QueueMessage (long id) :
   key_identifier (SMD_IPC_IDENTIFIER), id_communications (id)
{

}

QueueMessage::QueueMessage (int key, long id) :
   key_identifier (key), id_communications (id)
{

}

QueueMessage::~QueueMessage ()
{

}

bool QueueMessage::sendSharedMessageSMD (SmdIPCStructTypes structType, void * data)
{
   bool out = true;
   LOG_DEBUG(logger, "Sending IPC message...");
   CALL_IF0_MSG(this->id_communications, "channel id must be > 0");

   int queue = msgget (key_identifier, IPC_CREAT | 0600);

   if (queue == -1) {
      string error = strerror (errno);
      LOG_ERROR(logger, fString::format ("Failed to access the message queue, err:'%s'", error.c_str()));
      out = false;
   }
   else {
      SmdIPCGlobal message;
      memset (&message, 0, sizeof(SmdIPCGlobal));

      //set data
      message.type_Ipc_struct = structType;
      message.size = QueueMessage::sizeOfStructIPC (structType);

      defaultIPCS frameToSend = encapsulate (sizeof(SmdIPCGlobal), (void *) &message);

      //First Message
      size_t size = sizeof(SmdIPCGlobal);
      if (msgsnd (queue, (void *) &frameToSend, size, IPC_NOWAIT) < 0) {
         string error = strerror (errno);
         LOG_ERROR(logger, fString::format ("Failed to send header message, err:'%s'", error.c_str()));
         out = false;
      }
      else {

         //Second Message REmplir DAta
         LOG_TRACE(logger, fString::format ("Sending message size:%d", message.size));
         frameToSend = encapsulate (message.size, data);

         if (msgsnd (queue, (void *) &frameToSend, message.size, IPC_NOWAIT) < 0) {
            out = false;
         }

         if (!out) {
            string error = strerror (errno);
            LOG_ERROR(logger, fString::format ("Failed to sent data message, err:'%s'",error.c_str()));
         }
      }
   }

   if (out) {
      LOG_DEBUG(logger, "Message sent!");
   }

   return out;
}

SmdIPCGlobalResponse & QueueMessage::receiveSharedMessageSMD (SmdIPCGlobalResponse & result)
{
   memset (&result, 0, sizeof(SmdIPCGlobalResponse));
   LOG_DEBUG(logger, "RECEIVE MESSAGE ...");

   int queue = msgget (key_identifier, IPC_CREAT | 0600);
   if (queue == -1) {
      string error = strerror (errno);
      LOG_ERROR(logger, fString::format ("CREATING OR USING A QUEUE  MESSAGE, err:'%s'", error.c_str()));
   }
   else {
      defaultIPCS frameToReceive;
      SmdIPCGlobal * message;

      memset (&frameToReceive, 0, sizeof(defaultIPCS));

      //First Message
      ssize_t ret = msgrcv (queue, (void *) &frameToReceive, sizeof(SmdIPCGlobal), id_communications, 0);
      if (ret >= 0) {
         message = (SmdIPCGlobal *) frameToReceive.mtext;

         result.type = message->type_Ipc_struct;
         size_t size = message->size;
         SmdIPCStructTypes type = message->type_Ipc_struct;
         memset (&frameToReceive, 0, sizeof(defaultIPCS));

         //second Message
         ret = msgrcv (queue, (void *) &frameToReceive, size, id_communications, 0);
         if (ret >= 0) {
            switch (type) {
               case SmdIPCStruct_string:
                  result.RString = *((SmdIPCString *) frameToReceive.mtext);
                  break;

               case SmdIPCStruct_progress:
                  result.RProgress = *((SmdIPCProgress *) frameToReceive.mtext);
                  break;

               case SmdIPCStruct_bool:
                  result.RBool = *((SmdIPCBool *) frameToReceive.mtext);
                  break;

               default:
                  LOG_ERROR(logger, fString::format ("RECEIVING THE SECOND MESSAGE: unknown type:%d", message->type_Ipc_struct));
                  break;
            }
         }
         else {
            string error = strerror (errno);
            LOG_ERROR(logger, fString::format ("RECEIVING THE SECOND MESSAGE, err:'%s'", error.c_str()));
         }
      }
      else {
         string error = strerror (errno);
         LOG_ERROR(logger, fString::format ("RECEIVING THE FIRST MESSAGE, err:'%s'", error.c_str()));
      }

   }

   LOG_DEBUG(logger, "RECEIVE MESSAGE DONE !");

   return result;
}
bool QueueMessage::receiveSharedMessage (size_t size_struct, defaultIPCS & recievedData)
{
   memset (&recievedData, 0, sizeof(defaultIPCS));

   bool out = true;
   LOG_DEBUG(logger, "Recieve IPC message...");
   CALL_IF0_MSG(this->id_communications, "channel id must be > 0");

   int queue = msgget (key_identifier, IPC_CREAT | 0600);
   if (queue == -1) {
      string error = strerror (errno);
      LOG_ERROR(logger, fString::format ("CREATING OR USING A QUEUE  MESSAGE, err:'%s'", error.c_str()));
      out = false;
   }
   else {
      ssize_t ret = msgrcv (queue, (void *) &recievedData, size_struct, id_communications, 0);
      if (ret >= 0) {
         out = true;
      }
      else {
         string error = strerror (errno);
         LOG_ERROR(logger, fString::format ("RECEIVING THE SECOND MESSAGE, err:'%s'", error.c_str()));
         out = false;
      }
   }

   if (out) {
      LOG_DEBUG(logger, "RECEIVE MESSAGE DONE !");
   }

   return out;

}

bool QueueMessage::sendSharedMessage (size_t size_struct, void * data)
{
   bool out = true;
   LOG_DEBUG(logger, "Sending IPC message...");
   CALL_IF0_MSG(this->id_communications, "channel id must be > 0");

   int queue = msgget (key_identifier, IPC_CREAT | 0600);

   if (queue == -1) {
      string error = strerror (errno);
      LOG_ERROR(logger, fString::format ("Failed to access the message queue, err:'%s'", error.c_str()));
      out = false;
   }
   else {
      defaultIPCS frameToSend = encapsulate (size_struct, data);

      if (msgsnd (queue, (void *) &frameToSend, size_struct, IPC_NOWAIT) < 0) {
         string error = strerror (errno);
         LOG_ERROR(logger, fString::format ("Failed to send message, err:'%s'", error.c_str()));
         out = false;
      }
   }

   if (out) {
      LOG_DEBUG(logger, "Message sent !");
   }

   return out;
}

size_t QueueMessage::sizeOfStructIPC (SmdIPCStructTypes type)
{
   switch (type) {
      case SmdIPCStruct_global:
         return sizeof(SmdIPCGlobal);

      case SmdIPCStruct_string:
         return sizeof(SmdIPCString);

      case SmdIPCStruct_progress:
         return sizeof(SmdIPCProgress);

      case SmdIPCStruct_bool:
         return sizeof(SmdIPCBool);

      default:
         return 0;
   }
}

int QueueMessage::messageQueueSize ()
{
   int file = 0;
   struct msqid_ds donnees;

   file = msgget (key_identifier, IPC_CREAT | 0600);
   if (file == -1) {
      string error = strerror (errno);
      LOG_ERROR(logger, fString::format ("CREATING OR USING A QUEUE  MESSAGE, err:'%s'", error.c_str()));
   }

   if (file >= 0 && msgctl (file, IPC_STAT, &donnees) == -1) {
      string error = strerror (errno);
      LOG_ERROR(logger, fString::format ("GETTING A QUEUE MESSAGE PROPRIETIES, err:'%s'", error.c_str()));
   }

   return donnees.msg_qnum;
}

defaultIPCS QueueMessage::encapsulate (size_t size_struct, const void *data)
{
   defaultIPCS enc;
   memset (&enc, 0, sizeof(defaultIPCS));
   enc.mtype = id_communications;

   char *ss = (char *) data;
   memcpy (enc.mtext, ss, size_struct);

   return enc;
}

void QueueMessage::killMessageQueue ()
{
   QueueMessage::killMessageQueue (key_identifier);
}

void QueueMessage::killMessageQueue (int key_identifier)
{
   int queue = 0;

   if ((queue = msgget (key_identifier, IPC_CREAT | 0600)) == -1) {
      string error = strerror (errno);
      LOG_ERROR(logger, fString::format ("CREATING OR USING A QUEUE  MESSAGE, err:'%s'", error.c_str()));
   }

   if (queue >= 0 && msgctl (queue, IPC_RMID, NULL) == -1) {
      string error = strerror (errno);
      LOG_ERROR(logger, fString::format ("DELETING A QUEUE MESSAGE, err:'%s'", error.c_str()));
   }
}
