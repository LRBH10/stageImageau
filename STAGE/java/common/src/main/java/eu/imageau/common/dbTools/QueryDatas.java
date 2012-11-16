/* $Id: QueryDatas.java 140 2011-09-21 08:57:00Z bdm $ */
package eu.imageau.common.dbTools;

import java.util.Date;

/**
 * This class is the query representation for parameters manipulations with Name / Value Dual
 * 
 * @author lionel cavaglia
 */
public class QueryDatas
{
   /** Alphanumeric identifier */
   private String Name;

   /** Related object */
   private String value;

   public QueryDatas (String Name, String value)
   {
      setName (Name);
      setValue (value);
   }

   public QueryDatas (String Name, int value)
   {
      setName (Name);
      setValue (Integer.valueOf (value).toString ());
   }

   public QueryDatas (String Name, double value)
   {
      setName (Name);
      setValue (Double.valueOf (value).toString ());
   }

   public QueryDatas (String Name, Date value)
   {
      setName (Name);
      setValue (value.toString ());
   }

   public QueryDatas (String Name, java.sql.Date value)
   {
      setName (Name);
      setValue (value.toString ());
   }

   /**
    * Affect the name of the query
    * 
    * @param name
    */
   public void setName (String name)
   {
      this.Name = name;
   }

   /**
    * get accessor for Query's Name
    * 
    * @return
    */
   public String getName ()
   {
      return this.Name;
   }

   /**
    * Affect the name of the query
    * 
    * @param name
    */
   public void setValue (String name)
   {
      this.value = name;
   }

   /**
    * get accessor for Query's Name
    * 
    * @return
    */
   public String getValue ()
   {
      return this.value;
   }

}
