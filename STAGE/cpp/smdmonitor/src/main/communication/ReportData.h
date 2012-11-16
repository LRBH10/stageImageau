/*
 * ReportData.h
 *
 *  Created on: 25 mai 2012
 *      Author: rli
 */

#ifndef OPERATIONREPORT_H_
#define OPERATIONREPORT_H_
#include <iostream>

using namespace std;

namespace OperationsSMD
{

   /**
    * Contains the report  execution of an  operation
    */
   class ReportData
   {
      public:
         /**
          * Constructor with all parameters
          * @param id       represent the id of operation
          * @param status   true if OK , false else
          * @param contents a message descriptive
          */
         ReportData (long id, bool status, const string &contents);

         /**
          * Constructor with id only
          * @param id  represent the id of operation
          */
         ReportData (long id);
         /**
          * Default destructor
          */
         virtual ~ReportData ();

         /**
          * Append two report
          * @param other
          */
         void append (ReportData *other);

         long id;
         bool status;
         string contents;
   };

} /* namespace OperationsSMD */
#endif /* OPERATIONREPORT_H_ */
