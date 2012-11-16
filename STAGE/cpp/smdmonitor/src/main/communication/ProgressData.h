/*
 * OperatipnProgress.h
 *
 *  Created on: 20 juin 2012
 *      Author: rli
 */

#ifndef OPERATIPNPROGRESS_H_
#define OPERATIPNPROGRESS_H_
#include <iostream>
#include "commtype.h"

using namespace std;

namespace OperationsSMD
{
   /**
    * Contains a progress details
    */
   class ProgressData
   {
      public:
         /**
          * Default constructor
          */
         ProgressData ();
         /**
          * Constructor with all parameters
          * @param id     @warning must be unique (id database in web service)
          * @param title  of the progress
          * @param value  of progress (between 0 .. 100)
          */
         ProgressData (uint32 id, const string &title, double value);

         /**
          * default destructor
          */
         virtual ~ProgressData ();

         /**
          * Show Progress
          */
         void debug ();

         long id;
         string title;
         double value;
   };

}
#endif /* OPERATIPNPROGRESS_H_ */
