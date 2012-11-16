#include "StatTools.h"

double StatTools::purifiedMean (const Covariance & toPurify, double maxStdDevCoef)
{
   return StatTools::purifiedCov (toPurify, maxStdDevCoef).mean;
}

Covariance StatTools::purifiedCov (const Covariance & toPurify, double maxStdDevCoef)
{
   Covariance purifiedCov;
   double maxDev = toPurify.stdDev * (1.0 + maxStdDevCoef);

   for (uint32 i = 0; i < toPurify.nbVal && toPurify.nbVal > 2; ++i) {
      if (abs (toPurify.values [i] - toPurify.mean) <= maxDev) {
         purifiedCov.add (toPurify.values [i]);
      }
   }

   return purifiedCov;
}
