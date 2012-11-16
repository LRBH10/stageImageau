#include "Covariance.h"

Covariance::Covariance () :
   sum (0), squareSum (0), mean (0.0), stdDev (0.0), nbVal (0), values ()
{
   values.clear ();
}

Covariance::~Covariance ()
{
}

Covariance::Covariance (double values [], uint32 nb) :
   sum (0), squareSum (0), mean (0.0), stdDev (0.0), nbVal (0), values ()
{
   for (uint32 i = 0; i < nb; ++i) {
      add (values [i]);
   }
}

Covariance::Covariance (const Covariance& cov) :
   sum (cov.sum), squareSum (cov.squareSum), mean (cov.mean), stdDev (cov.stdDev), nbVal (cov.nbVal), values (cov.values)
{
}

Covariance & Covariance::operator= (const Covariance& cov)
{
   sum = cov.sum;
   squareSum = cov.squareSum;
   mean = cov.mean;
   stdDev = cov.stdDev;
   nbVal = cov.nbVal;
   values = cov.values;

   return *this;
}

//Add new value to mean/stdDev
void Covariance::add (double val)
{
   double value = val;
   values.push_back (value);
   sum += value;
   squareSum += value * value;
   ++nbVal;
   mean = sum / (double) nbVal;
   stdDev = sqrt (fabs (squareSum / nbVal - (mean * mean)));
}

double Covariance::getMedian () const
{
   double out = 0.0;
   std::sort (getValues ().begin (), getValues ().end ());
   int mid = this->nbVal / 2;
   if (this->nbVal % 2 == 1) {
      out = this->values [mid];
   }
   else {
      if (this->nbVal != 0) {
         out = (this->values [mid] + this->values [mid - 1]) / 2.0;
      }
   }
   return out;
}

double Covariance::getMean () const
{
   return this->mean;
}

double Covariance::getStdDev () const
{
   return this->stdDev;
}

vector<double> Covariance::getValues () const
{
   return this->values;
}

uint32 Covariance::getNbVal () const
{
   return this->nbVal;
}
