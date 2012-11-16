/*!
 * @file CurlCall.h
 * @brief a functions to communicate with an HTTP server
 *        TWO Mode :   GET   or   POST
 * @author rabah laouadi
 * @date 20 avril 2012
 */

#ifndef CURLCALL_H_
#define CURLCALL_H_

#include <iostream>
#include <string>
#include <curl/curl.h>

#include "LogFactory.h"
#include "SmdException.h"

using namespace std;

/**
 * Namespace permits communication between a C + + code with any
 * http server with two methods  GET or POST
 */
namespace SMDCommunication
{
   /**
    * A class to communicate with HTTP server with two methods  GET or POST
    */
   class CurlCall
   {
      public:
         /**
          * Default constructor
          */
         CurlCall ();
         /**
          * Default destructor
          */
         virtual ~CurlCall ();

         /*!
          * function used by CURL to get the data in Buffer\n
          * not used by users
          * @param data returned by Curl Request
          * @param buffer the result to will be used
          * @return 1 in success 0 else
          */
         static int writerCallBack (char *data, size_t size, size_t nmemb, std::string *buffer);

         /*!
          * send Request in GET and receive Response
          * @param request that will send in GET options
          * @param server_url The Server URL
          * @return response of the request
          */
         static string sendGETRequest (const string &request, const string & server_url = "");

         /*!
          * send Request in POST and receive Response
          * @param request that will send in POST options
          * @param server_url The Server URL
          * @return response of the request
          */
         static string sendPOSTRequest (const string &request, const string & server_url = "");

         static string escape (const string& data);

         static LoggerPtr logger;

   };
}
#endif /* CURLCALL_H_ */
