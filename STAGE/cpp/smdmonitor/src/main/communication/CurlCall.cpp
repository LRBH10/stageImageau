#include "CurlCall.h"

using namespace SMDCommunication;
using namespace std;

LoggerPtr CurlCall::logger (LogFactory::getLogger ("CurlCall"));

CurlCall::CurlCall ()
{

}

CurlCall::~CurlCall ()
{

}

int CurlCall::writerCallBack (char *data, size_t size, size_t nmemb, string *buffer)
{
   // What we will return
   int result = 0;
   // Is there anything in the buffer?
   if (buffer != NULL) {
      // Append the data to the buffer
      buffer->append (data, size * nmemb);
      // How much did we write?
      result = size * nmemb;
   }
   return result;
}

string CurlCall::escape (const string& data)
{
   string out = data;
   CURL *curl;
   curl = curl_easy_init ();
   if (curl) {
      char * tmpStr = curl_easy_escape (curl, data.data (), data.length ());
      out = string (tmpStr);
      delete tmpStr;
      curl_easy_cleanup (curl);
   }
   return out;
}

string CurlCall::sendGETRequest (const string& request, const string & server_url)
{
   CURL *curl;
   CURLcode result;
   string buffer;

   string url = server_url + "/" + request;

   curl = curl_easy_init ();
   if (curl) {
      // Now set up all of the curl options
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str ());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writerCallBack);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

      // Attempt to retrieve the remote page
      result = curl_easy_perform (curl);
      // Always cleanup
      curl_easy_cleanup (curl);

      if (result == CURLE_OK) {
         LOG_TRACE(logger, fString::format ("Curl is Executed with status : OK, result:\n%s", buffer.c_str ()));
      }
      else {
         throw SmdException (fString::format ("'%s' - Web Service OFF or Bad Connection", __FUNC__).c_str (), __FILE__, __LINE__);
      }
   }
   return buffer;
}

/*
 * @param request : The POST Request
 * @param other_url : To change the url
 * @return Response for request
 */
string CurlCall::sendPOSTRequest (const string& request, const string & server_url)
{
   CURL *curl;
   CURLcode result;
   string buffer;

   LOG_TRACE(logger, fString::format("POST REQUEST : %s/%s", server_url.c_str(), request.c_str()));

   curl = curl_easy_init ();
   if (curl) {
      // Now set up all of the curl options
      curl_easy_setopt(curl, CURLOPT_URL, server_url.c_str ());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str ());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writerCallBack);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

      // Attempt to retrieve the remote page
      result = curl_easy_perform (curl);

      // Always cleanup
      curl_easy_cleanup (curl);

      if (result == CURLE_OK) {
         LOG_TRACE(logger, fString::format ("POST RESPONSE - result:\n%s", buffer.c_str ()));
      }
      else {
         string msg = "POST RESPONSE - Web Service is OFF or Bad Connection";
         LOG_WARN(logger, msg);
         throw SmdException (msg.c_str (), __FILE__, __LINE__);
      }
   }
   return buffer;
}

