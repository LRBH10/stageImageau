/* $Id: DbSourceTest.java 141 2011-09-27 08:29:39Z bdm $ */
package eu.imageau.common.dbTools;

import java.util.List;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import eu.imageau.common.dbTools.mock.ContactMock;
import eu.imageau.common.dbTools.mock.SiteMock;
import eu.imageau.common.dbTools.mock.ProbeMock;

/**
 * 
 * @author lionel
 */
public class DbSourceTest
{

   public DbSourceTest ()
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
    * Test of fetchNamedQuery method, of class DbSource.
    */
   @Test
   public void testFetchNamedQuery ()
   {
      System.out.println ("fetchNamedQuery");
      String aName = ProbeMock.class.getSimpleName () + ".findByName";
      QueryDatas[] aTokens = new QueryDatas[] { new QueryDatas ("name", "sonde1") };
      DbSource instance = new DbSource ();
      List<ProbeMock> result = instance.fetchNamedQuery (ProbeMock.class, aName, aTokens);
      Assert.assertEquals (1, result.size ());
      Assert.assertEquals (aTokens [0].getValue (), result.get (0).getNom ());
   }

   /**
    * Test of fetchQuery method, of class DbSource.
    */
   @Test
   public void testFetchQuery ()
   {
      System.out.println ("fetchQuery");
      // First test check the
      String aQuery = "SELECT * FROM SITE WHERE id=9";
      DbSource instance = new DbSource ();
      List<SiteMock> result = instance.fetchQuery (SiteMock.class, aQuery);
      Assert.assertTrue (!result.isEmpty ());
      Assert.assertEquals ("Hossegor", result.get (0).getNom ());
      // Second test check an administrator in db
      String anotherQuery = "SELECT * FROM CONTACT WHERE adminFlag=1";
      List<ContactMock> result2 = instance.fetchQuery (ContactMock.class, anotherQuery);
      Assert.assertEquals ("admin", result2.get (0).getNom ());
   }

   /**
    * Test of commit method, of class DbSource.
    */
   @Test
   public void testCommit ()
   {
      System.out.println ("commit");

      // First test with null data
      PersistentObject data = null;
      DbSource instance = new DbSource ();
      Integer expResult = 0;

      expResult = instance.insert (data);
      Assert.assertNull (expResult);
      // Assert.assertTrue ("Should have failed !", false);

      // Second with a not null entity
      expResult = 0;
      String userName = "junit test contact";
      ContactMock c = new ContactMock (userName);

      expResult = instance.insert (c);
      Assert.assertNotNull (expResult);

      c.setSociete ("test societe");
      boolean ret = instance.commit (c);
      Assert.assertTrue (ret);

      instance.execQuery ("delete from CONTACT where nom='" + userName + "'");
   }

   /**
    * Test of closeConnection method, of class DbSource.
    */
   @Test
   public void testCloseConnection ()
   {
      System.out.println ("closeConnection");
      DbSource instance = new DbSource ();
      Assert.assertFalse (instance.isClosed ());
      instance.closeConnection ();
      Assert.assertTrue (instance.isClosed ());
   }

   /**
    * Test of getDbConfigurationFile method, of class ContextManager.
    */
   @Test
   public void testGetDbConfigurationFile ()
   {
      System.out.println ("getDbConfigurationFile");
      String result = DbSource.getDbConfigurationFile (new Integer (0));
      Assert.assertNotNull (result);
   }

}
