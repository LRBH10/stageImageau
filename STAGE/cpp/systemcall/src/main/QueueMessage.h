/*
 * ServerIPCs.h
 *
 *  Created on: 18 juin 2012
 *      Author: rli
 */

#ifndef SERVERIPCS_H_
#define SERVERIPCS_H_

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "SmdIPCStruct.h"
#include <cstdlib>
#include <cerrno>

#include "LogFactory.h"

#define     SMD_IPC_IDENTIFIER  0x000b7974   //751988 en Hexa
#define     TAILLE_FILE_MSG   4096
using namespace std;

/**
 * Name space to make easy the utilization of IPC Queue Message and SIGNAL System Receive in C++
 */
namespace systemcall
{
   /**
    * Example of utilization:\n
    * Create and initialize String message @ref SmdIPCString
    * @code
    * SmdIPCString message;
    * memset (&message, 0, sizeof(SmdIPCString));
    * strcpy (message.contents, "Tableau format international");
    * @endcode
    * Create an ipcs that communicate in canal 1
    * @code
    * QueueMessage ipcs (1);
    * @endcode
    * send message
    * @code
    * ipcs.sendSharedMessageSMD (SmdIPCStruct_string, (void *) &testString);
    * @endcode
    * receive and debug the message (to show it)
    * @code
    * SmdIPCGlobalResponse response = ipcs.receiveSharedMessageSMD ();
    * reponse.debug();
    * @endcode
    *
    * @warning the canal of communication must be the same or 0 in receiver
    *
    */
   class QueueMessage
   {
      public:

         /*!
          * For Log manipulation
          */
         static LoggerPtr logger;

         /*!
          * default key is @ref SMD_IPC_IDENTIFIER \n
          * id_communications is 0
          */
         QueueMessage ();
         /*!
          * @param id  to initialize id_communications
          * default key is @ref SMD_IPC_IDENTIFIER \n
          */
         QueueMessage (long id);
         /*!
          * @param key the key of communications.
          * @param id  to initialize id_communications
          */
         QueueMessage (int key, long id);

         virtual ~QueueMessage ();

         /**
          * send a struct message @ref SmdIPCStructTypes
          * @warning  The type_struct and data must have the same TYPE \n
          * @warning  else  error in USE
          * @param type_struct type of struct @ref SmdIPCStructTypes
          * @param data the data to Send
          * @return
          */
         bool sendSharedMessageSMD (SmdIPCStructTypes type_struct, void * data);

         /**
          * Receive a struct message @ref SmdIPCStructTypes
          * @return the struct message
          */
         SmdIPCGlobalResponse & receiveSharedMessageSMD (SmdIPCGlobalResponse & out);

         /**
          * send the data that will be recived from @ref receiveSharedMessage
          * @warning Becarful when you use this function
          * @param size_struct size of the struct to send
          * @param data that can be any STRUCT <strong>without virtual Attributes</strong>.
          * @return true if OK, false else
          */
         bool sendSharedMessage (size_t size_struct, void *data);

         /**
          * recive the date sended by @ref sendSharedMessage \n
          * Exemple for use :\n
          * Get the message from queue message
          * @code
          *  defaultIPCS response;
          * queuemessage.receiveSharedMessage(sizeof(MyStruct),response);
          * @endcode
          *
          * the cast of struct
          * @code
          * MyStruct my_struct;
          * my_struct = *((MyStruct *) response.mtext);
          * @endcode
          *
          * @param size_struct the size of struct <strong> must be the same in the function sended</strong>
          * @param recievedData it will content the response
          * @return true if OK, false else
          */
         bool receiveSharedMessage (size_t size_struct, defaultIPCS & recievedData);

         /**
          * Get the size of File message in SYSTEM IPC V of key @ref SMD_IPC_IDENTIFIER
          * @return size of file message
          */
         int messageQueueSize ();

         /**
          * Get the size of struct from the @ref SmdIPCStructTypes
          * @return size of struct that match with the type givend
          */
         static size_t sizeOfStructIPC (SmdIPCStructTypes type_struct);

         /**
          * To kill the queue message
          */
         void killMessageQueue ();

         /**
          * Static function to kill the queue message
          * @param key identifier of Queue Message
          */
         static void killMessageQueue (int key);

         /**
          * Converting other structures @ref  SmdIPCStructTypes on default ipcs @ref defaultIPCS
          * @param size_struct size of Struct
          * @param data   any structure
          * @return   @ref defaultIPCS
          */
         defaultIPCS encapsulate (size_t size_struct, const void *data);

         /**
          * @todo Ajouter 2 fonction qui  permet d'envoyer et de recevoir ce n'import quel structure
          */
      private:
         /**
          * Key of communication in IPCs
          */
         key_t key_identifier;
         /**
          * Type of Communication
          */
         long id_communications;
   };
}
#endif /* SERVERIPCS_H_ */
