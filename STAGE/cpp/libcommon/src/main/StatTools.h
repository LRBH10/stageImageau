/**
 * StatTools.h
 *
 * $Id: StatTools.h 617 2012-07-12 08:26:26Z bdm $
 */

#ifndef STATTOOLS_H_
#define STATTOOLS_H_

#include <cmath>
#include <cstdlib>
#include "commtype.h"
#include <vector>
#include <cmath>
#include "Covariance.h"

namespace imaGeau
{
   namespace Math
   {
      /**
       * @ingroup math
       * StatTools
       * @since 22 nov. 2010
       * @author bdm
       */
      class StatTools
      {
         public:
            /**
             * try to compute a mean without outlier values
             * @param toPurify covariance to purify
             * @param maxStdDevCoef
             * @return purified mean
             */
            static double purifiedMean (const Covariance & toPurify, double maxStdDevCoef);

            /**
             * try to compute a mean without outlier values
             * @param toPurify covariance to purify
             * @param maxStdDevCoef
             * @return purified covariance
             */
            static Covariance purifiedCov (const Covariance & toPurify, double maxStdDevCoef);

      };
   }
} // end of namespace

using namespace imaGeau;
using namespace imaGeau::Math;

#endif /* STATTOOLS_H_ */
