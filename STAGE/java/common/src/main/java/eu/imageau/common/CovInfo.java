/**
 * 
 */
package eu.imageau.common;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.Vector;

/**
 * @author bdm
 * 
 */
public class CovInfo<T extends Number>
{
   static public <T extends Number> CovInfo<T> purifiedCov (final CovInfo<T> toPurify, double maxStdDevCoef)
   {
      CovInfo<T> purifiedCov = new CovInfo<T> ();
      double maxDev = toPurify.stdDev * (1.0 + maxStdDevCoef);

      for (int i = 0; i < toPurify.nbVal && toPurify.nbVal > 2; ++i) {
         if (Math.abs (toPurify.values.get (i).doubleValue () - toPurify.mean) <= maxDev) {
            purifiedCov.add (toPurify.values.get (i));
         }
      }

      return purifiedCov;
   }

   static public <T extends Number> double purifiedMean (final CovInfo<T> toPurify, double maxStdDevCoef)
   {
      return CovInfo.purifiedCov (toPurify, maxStdDevCoef).mean;
   }

   double sum;
   double squareSum;
   double mean;
   double stdDev;
   T min;
   T max;

   int nbVal;

   Vector<T> values;

   public CovInfo ()
   {
      this.sum = 0.0;
      this.squareSum = 0.0;
      this.mean = 0.0;
      this.stdDev = 0.0;
      this.min = null;
      this.max = null;

      this.nbVal = 0;
      this.values = new Vector<T> ();
      this.values.clear ();
   }

   public CovInfo (final CovInfo<T> cov)
   {
      this.sum = cov.sum;
      this.squareSum = cov.squareSum;
      this.mean = cov.mean;
      this.stdDev = cov.stdDev;
      this.nbVal = cov.nbVal;
      this.values = cov.values;
      this.min = cov.min;
      this.max = cov.max;
   }

   // Add new value to mean/stdDev
   public void add (T val)
   {
      double value = val.doubleValue ();
      this.values.add (val);
      this.sum += value;
      this.squareSum += value * value;
      ++this.nbVal;
      this.mean = this.sum / this.nbVal;
      this.stdDev = Math.sqrt (Math.abs (this.squareSum / this.nbVal - this.mean * this.mean));

      if (this.min == null) {
         this.min = val;
         this.max = val;
      }
      else if (val.doubleValue () < this.min.doubleValue ()) {
         this.min = val;
      }
      else if (val.doubleValue () > this.max.doubleValue ()) {
         this.max = val;
      }
   }

   /**
    * @return the max
    */
   public T getMax ()
   {
      return this.max;
   }

   /**
    * @return the mean
    */
   public double getMean ()
   {
      return this.mean;
   }

   /**
    * @return the min
    */
   public T getMin ()
   {
      return this.min;
   }

   /**
    * @return the nbVal
    */
   public int getNbVal ()
   {
      return this.nbVal;
   }

   /**
    * @return the stdDev
    */
   public double getStdDev ()
   {
      return this.stdDev;
   }

   /**
    * @return the values
    */
   public Vector<T> getValues ()
   {
      return this.values;
   }

   public void reset ()
   {
      this.values.clear ();
   }

   @Override
   public String toString ()
   {
      DecimalFormatSymbols.getInstance ().setDecimalSeparator ('.');
      DecimalFormat fmt = new DecimalFormat ("0.000");
      return "min:" + this.min + " / max:" + this.max + " / mean:" + fmt.format (this.mean) + " / stdDev:" + fmt.format (this.stdDev)
            + " / nbVal:" + this.nbVal;
   }

}
