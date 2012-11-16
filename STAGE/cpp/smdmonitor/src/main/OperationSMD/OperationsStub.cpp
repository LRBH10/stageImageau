/*
 * EntitieGlobal.cpp
 *
 *  Created on: 25 avr. 2012
 *      Author: rli
 */

#include "OperationsStub.h"

using namespace OperationsSMD;

LoggerPtr OperationStub::logger (LogFactory::getLogger ("OperationStub"));

OperationStub::OperationStub () :
   url_server ()
{
}

OperationStub::OperationStub (const string &_url_server) :
   url_server (_url_server)
{
}

OperationStub::~OperationStub ()
{
}

/*
 * set the server url
 */
void OperationStub::setUrlServer (const string & url)
{
   url_server = url;
}

/*
 * Get Operation JSON TEXT from Server
 */
string OperationStub::retrieveTextOperationList (const string & nameSMD)
{
   string _url = url_server;
   _url += "operations/getOperations";

   string request = "nameSMD=" + nameSMD;
   string response;

   try {
      response = CurlCall::sendPOSTRequest (request, _url);
   }
   catch (SmdException &e) {
      e.calledBy (__FUNC__, __FILE__, __LINE__);
   }
   return response;
}

/*
 * Get Operations vector<OperationAbstract>
 */
vector<OperationAbstract*> OperationStub::deserializeOperationList (const string & json_text)
{
   vector<OperationAbstract*> returnedArray;

   json_spirit::Value val;
   json_spirit::read (json_text, val);

   json_spirit::Array operations = val.get_obj () [0].value_.get_array ();

   //recuperer le tableaux contenant les  Operations
   for (size_t i = 0; i < operations.size (); i++) {

      //recuperer l'operation at i
      json_spirit::Object obj = operations [i].get_obj ();
      try {
         OperationAbstract *op = OperationSMDFunctions::OperationFactory (obj);
         returnedArray.push_back (op);
      }
      catch (SmdException &exc) {
         exc.calledBy (__FUNC__, __FILE__, __LINE__);
         throw;
      }

   }
   return returnedArray;
}

/**
 *   Get Operations Associed for this SMD
 */
vector<OperationAbstract*> OperationStub::retrieveOperationList (const string & nameSMD)
{
   string text = retrieveTextOperationList (nameSMD);
   vector<OperationAbstract*> outs;
   try {
      outs = deserializeOperationList (text);
   }
   catch (SmdException &exc) {
      exc.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }
   return outs;
}

/**
 *  Get Operation by ID (FOR imbriqued Operation)
 */
OperationAbstract* OperationStub::retrieveOperationById (const long &id, const string & type = "OperationAbstract")
{
   string _url = url_server;
   _url += "operations/getOperationById";

   string data = "idOperation=" + fString::format ("%ld", id);
   data += "&type=" + type;

   string json_text;

   try {
      json_text = CurlCall::sendPOSTRequest (data, _url);
   }
   catch (SmdException& e) {
      e.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }

   json_spirit::Value val;
   json_spirit::read (json_text, val);

   json_spirit::Object obj = val.get_obj ();

   OperationAbstract * out;
   try {
      out = OperationSMDFunctions::OperationFactory (obj);
   }
   catch (SmdException &exc) {
      exc.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }
   return out;

}

/**
 *  Get Operation by ID (FOR imbriqued Operation)
 */
OperationAbstract* OperationStub::retrieveOperationById (OperationAbstract *op)
{
   string _url = url_server;
   _url += "operations/getOperationById";

   long id = op->getIdOperation ();
   string type = OperationSMDFunctions::OperationTypeToString (op->getTypeOperation ());

   return retrieveOperationById (id, type);
}

/**
 * connect To Server
 */
bool OperationStub::connectToServer (const string & nameSMD, const bool what)
{
   string _url = url_server;
   _url += "informations/setConnected";

   string request = "nameSMD=" + nameSMD + "&what=" + OperationSMDFunctions::bool_to_string (what);

   string response;
   try {
      response = CurlCall::sendPOSTRequest (request, _url);
   }
   catch (SmdException& e) {
      e.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }

   if (response.compare ("Connected OK") != 0) {
      throw SmdException ("Not a good request", __FILE__, __LINE__);
   }
   return true;
}

/**
 * update version in SERVER
 */
bool OperationStub::sendVersion (const string & nameSMD, const string & version)
{
   string _url = url_server;
   _url += "informations/setVersion";

   string request = "nameSMD=" + nameSMD + "&newVersion=" + version;
   string response;

   try {
      response = CurlCall::sendPOSTRequest (request, _url);
   }
   catch (SmdException &e) {
      e.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }

   if (response.compare ("Version Updated") != 0) {
      throw SmdException ("Not a good request", __FILE__, __LINE__);
   }
   return true;
}

/*
 *    Operation report
 */
bool OperationStub::sendReportData (ReportData *report)
{
   string _url = url_server;
   _url += "informations/setAchieved";

   string request = "idOperation=" + fString::format ("%ld", report->id);
   request += "&achievedStatus=" + OperationSMDFunctions::bool_to_string (report->status);

   string escaped = CurlCall::escape (report->contents);
   request += "&reportStatus=" + escaped;

   string response;
   try {
      response = CurlCall::sendPOSTRequest (request, _url);
   }
   catch (SmdException &e) {
      e.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }

   if (response.compare ("Status is updated") != 0) {
      throw SmdException ("Not a good request", __FILE__, __LINE__);
   }
   return true;
}

bool OperationStub::sendOperationProgress (const long & id_operation, const double & value)
{
   string _url = url_server;
   _url += "informations/progress";

   string request = "idOperation=" + fString::format ("%ld", id_operation);
   request += "&progressStatus=" + fString::format ("%lf", value);

   string response;
   try {
      response = CurlCall::sendPOSTRequest (request, _url);
   }
   catch (SmdException &e) {
      e.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }

   if (response.compare ("Status is updated") != 0) {
      throw SmdException ("Not a good request", __FILE__, __LINE__);
   }
   return true;
}

bool OperationStub::sendProgressSMD (const string & nameSMD, ProgressData *progress)
{
   string _url = url_server;
   _url += "informations/progressSMD";

   string request = "nameSMD=" + nameSMD;
   request += "&id_progress=" + fString::format ("%lu", progress->id);

   string escaped = CurlCall::escape (progress->title);
   request += "&title=" + escaped;

   request += "&value=" + fString::format ("%lf", progress->value);

   string response;
   try {
      response = CurlCall::sendPOSTRequest (request, _url);
   }
   catch (SmdException &e) {
      e.calledBy (__FUNC__, __FILE__, __LINE__);
      throw;
   }

   if (response.compare ("Status is updated") != 0) {
      throw SmdException ("Not a good request", __FILE__, __LINE__);
   }
   return true;
}
