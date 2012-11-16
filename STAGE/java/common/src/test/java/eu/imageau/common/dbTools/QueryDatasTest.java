/* $Id: QueryDatasTest.java 141 2011-09-27 08:29:39Z bdm $ */
package eu.imageau.common.dbTools;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import eu.imageau.common.dbTools.QueryDatas;


/**
 * 
 * @author lionel
 */
public class QueryDatasTest
{

   public QueryDatasTest ()
   {
   }

   @BeforeClass
   public static void setUpClass () throws Exception
   {
   }

   @AfterClass
   public static void tearDownClass () throws Exception
   {
   }

   @Before
   public void setUp ()
   {
   }

   @After
   public void tearDown ()
   {
   }

   /**
    * Test of setName method, of class QueryDatas.
    */
   @Test
   public void testSetName ()
   {
      System.out.println ("setName");
      String name = "";
      QueryDatas instance = new QueryDatas ("nom", "value");
      instance.setName (name);
   }

   /**
    * Test of getName method, of class QueryDatas.
    */
   @Test
   public void testGetName ()
   {
      System.out.println ("getName");
      QueryDatas instance = new QueryDatas ("nom", "value");
      String expResult = "nom";
      String result = instance.getName ();
      Assert.assertEquals (expResult, result);

   }

   /**
    * Test of setValue method, of class QueryDatas.
    */
   @Test
   public void testSetValue ()
   {
      System.out.println ("setValue");
      String name = "";
      QueryDatas instance = new QueryDatas ("nom", "value");
      instance.setValue (name);
   }

   /**
    * Test of getValue method, of class QueryDatas.
    */
   @Test
   public void testGetValue ()
   {
      System.out.println ("getValue");
      QueryDatas instance = new QueryDatas ("nom", "value");
      String expResult = "value";
      String result = instance.getValue ();
      Assert.assertEquals (expResult, result);

   }

}
