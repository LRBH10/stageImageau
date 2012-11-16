/**
 * Covariance.h
 *
 * $Id: Covariance.h 617 2012-07-12 08:26:26Z bdm $
 */

#ifndef COVARIANCE_H_
#define COVARIANCE_H_

#include "commtype.h"
#include <vector>
#include <cmath>
#include <algorithm>

namespace imaGeau
{
   namespace Math
   {
      /**
       * @ingroup commontools
       * @defgroup math Mathematic tool module
       * imaGeau::Covariance
       * Storage for covariance info. Compute dynamically mean and standard deviation for a set of values.
       * @since 22 nov. 2010
       * @author bdm
       */
      class Covariance
      {
         private:
            double sum;
            double squareSum;

         public:
            double mean;
            double stdDev;
            uint32 nbVal;
            vector<double> values;

            /**
             * default constructor
             */
            Covariance ();

            /**
             * Constructor with array of value
             * @param values values
             * @param nbVal array size
             */
            Covariance (double values [], uint32 nbVal);

            /**
             * Copy constructor
             * @param obj to be copied
             */
            Covariance (const Covariance& obj);

            /**
             * Copy operator
             * @param obj to be copied
             * @return
             */
            Covariance & operator= (const Covariance& obj);

            /**
             * Destructor
             */
            virtual ~Covariance ();

            /**
             * @return median value
             */
            double getMedian () const;

            /**
             * @return mean value
             */
            double getMean () const;

            /**
             * @return standard deviation
             */
            double getStdDev () const;

            /**
             * @return value array
             */
            vector<double> getValues () const;

            /**
             * @return array size
             */
            uint32 getNbVal () const;

            /**
             * add a new value to the set and adjust mean, std dev, etc.
             * @param val value to add
             */
            void add (double val);

      };
   }
} // end of namespace

using namespace imaGeau;
using namespace imaGeau::Math;

#endif /* COVARIANCE_H_ */
