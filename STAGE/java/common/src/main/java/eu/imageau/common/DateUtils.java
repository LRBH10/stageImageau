/* $Id: DateUtils.java 140 2011-09-21 08:57:00Z bdm $ */
package eu.imageau.common;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * 
 * @author lionel
 */
public class DateUtils
{

   /**
    * Convert a date to a displayable String
    * 
    * @param aDate
    * @return
    */
   public static String convertForDisplay (Date aDate)
   {
      return new SimpleDateFormat ("MM/dd/yyyy").format (aDate);
   }

   /**
    * Convert a date from a String to MySQL Format
    * 
    * @param aDate
    * @return
    * @throws ParseException
    */
   public static String convertFromMySQLFormat (Date aDate) throws ParseException
   {
      return new SimpleDateFormat ("yyyy-mm-dd").format (aDate);
   }
}
