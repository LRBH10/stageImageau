/**
 * Hibernate Operation TEST
 * 
 * @author rli
 */

package eu.imageau.monitorSMD.operationSMD;

import java.util.Calendar;

import org.hibernate.Transaction;
import org.hibernate.classic.Session;
import org.junit.Assert;
import org.junit.Test;

import eu.imageau.monitorSMD.HibernateSMD.HibernateUtil;
import eu.imageau.monitorSMD.data.ProgressData;
import eu.imageau.monitorSMD.operationSMDEnum.ShellFileAction;

public class HibernateTest
{
   /**
    * To create a un SMD for test
    * 
    * @param nameSMD the name of SMD to create for the Test
    * @return instance of the SMD created in database
    */
   public static SiteSMD createTester (String nameSMD)
   {
      Session connect = HibernateUtil.getSessionFactory ().openSession ();
      
      SiteSMD tester = SiteSMD.findByName (connect, nameSMD);
      
      if (tester == null) {
         SiteSMD tester1 = new SiteSMD (nameSMD);
         connect.beginTransaction ();
         connect.save (tester1);
         connect.getTransaction ().commit ();
         connect.close ();
         return tester1;
      }
      return tester;

   }

   /**
    * To delete the SMD of test
    * 
    * @param nameSMD the name of SMD to delete for the Test
    */
   public static void deleteTester (String nameSMD)
   {
      Session connect = HibernateUtil.getSessionFactory ().openSession ();
      SiteSMD tester1 = SiteSMD.findByName (connect, nameSMD);
      connect.beginTransaction ();
      connect.delete (tester1);
      connect.getTransaction ().commit ();
      connect.close ();
   }

   /**
    * Test of Session Connection to database
    */
   // 1
   @Test
   public void testConnection ()
   {
      Session connect = HibernateUtil.getSessionFactory ().openSession ();
      Assert.assertNotNull ("Session de Connexion ", connect);

      Transaction trans = connect.beginTransaction ();
      Assert.assertNotNull ("Session de Transaction  ", trans);

   }

   /**
    * Test of Read and Update an SMD in Database
    */
   // 2
   @Test
   public void testSiteSMDReadUpdate ()
   {
      String SMDTest = "SMDTest2";
      Session connect = HibernateUtil.getSessionFactory ().openSession ();
      HibernateTest.createTester (SMDTest);

      // Corps
      SiteSMD tester = SiteSMD.findByName (connect, SMDTest);
      Assert.assertNotNull ("Read SiteSMD ", tester);

      tester.setVersion_code ("2.0");

      connect.beginTransaction ();
      connect.saveOrUpdate (tester);
      connect.getTransaction ().commit ();

      SiteSMD getTester = SiteSMD.findByName (connect, SMDTest);
      Assert.assertNotNull ("Read SiteSMD ", getTester);

      Assert.assertEquals ("Update SiteSMD", "2.0", getTester.getVersion_code ());
      // End Corps

      Assert.assertNotNull ("Create Or Existance de SMDTEST ", SiteSMD.findByName (connect, SMDTest));
      HibernateTest.deleteTester (SMDTest);
      connect.close ();

   }// */

   /**
    * Test of Read and Update operation of SMD in Database
    */
   @SuppressWarnings("deprecation")
   // 3
   @Test
   public void testSiteSMDwithOperation ()
   {
      String SMDTest = "SMDTest3";
      Session connect = HibernateUtil.getSessionFactory ().openSession ();
      Assert.assertNull ("Create Or Existance de SMDTEST ", SiteSMD.findByName (connect, SMDTest));
      SiteSMD tester = HibernateTest.createTester (SMDTest);

      // Corps
      tester.addOperation (new OperationScript ("simple1", "rien", tester));
      tester.addOperation (new OperationScript ("simple2", "rien", tester));
      tester.addOperation (new OperationScript ("simple3", "rien", tester));
      tester.addOperation (new OperationShellFile (ShellFileAction.ShellFileActionCopy, "test.txt", true, tester));

      connect.beginTransaction ();
      connect.saveOrUpdate (tester);
      connect.getTransaction ().commit ();

      // READ
      SiteSMD getTester = SiteSMD.findByName (connect, SMDTest);
      Assert.assertNotNull ("Read SiteSMD ", getTester);

      Assert.assertTrue ("GetOpertions", 4 == getTester.getOperations ().size ());

      // Modification
      tester.getOperations ().get (0).setAchieved (true);

      connect.beginTransaction ();
      connect.saveOrUpdate (tester);
      connect.getTransaction ().commit ();

      getTester = SiteSMD.findByName (connect, SMDTest);
      Assert.assertNotNull ("Read SiteSMD ", getTester);
      Assert.assertTrue ("Modification Effectuer", getTester.getOperations ().get (0).isAchieved ());
      // EndCorps

      Assert.assertNotNull ("Create Or Existance de SMDTEST ", SiteSMD.findByName (connect, SMDTest));
      HibernateTest.deleteTester (SMDTest);
      connect.close ();
   }

   /**
    * Test of create an multi operation in database
    */
   // 4
   @Test
   public void testSiteSMDwithMultiOperation ()
   {
      String SMDTest = "SMDTest4";
      Session connect = HibernateUtil.getSessionFactory ().openSession ();
      Assert.assertNull ("Create Or Existance de SMDTEST ", SiteSMD.findByName (connect, SMDTest));
      SiteSMD tester = HibernateTest.createTester (SMDTest);

      // Corps

      // Multioperation
      OperationMulti opm1 = new OperationMulti (tester);
      OperationScript ops1 = new OperationScript ("simple1", "rien1", tester);
      OperationScript ops2 = new OperationScript ("simple1", "rien1", tester);

      // Ajoute dans SMD
      connect.beginTransaction ();
      connect.saveOrUpdate (ops1);
      connect.saveOrUpdate (ops2);
      opm1.addOperationNested (ops1);
      opm1.addOperationNested (ops2);
      tester.addOperation (opm1);

      connect.saveOrUpdate (tester);
      connect.getTransaction ().commit ();// */
      // EndCorps

      Assert.assertNotNull ("Create Or Existance de SMDTEST ", SiteSMD.findByName (connect, SMDTest));
      HibernateTest.deleteTester (SMDTest);
      connect.close ();
   }// */

   /**
    * test of Progress status
    */
   @SuppressWarnings("deprecation")
   // 5
   @Test
   public void testProgress ()
   {
      Session c = HibernateUtil.getSessionFactory ().openSession ();

      SiteSMD site = SiteSMD.findByName (c, "testSMD5");
      if (site == null) {
         site = new SiteSMD ("testSMD5");
      }

      ProgressData pro = new ProgressData ();
      pro.setFinished (true);
      pro.setId (72);
      pro.setLast_date (Calendar.getInstance ().getTime ());
      pro.setTitle ("TEST");
      pro.setValue (75);

      site.addProgress (pro);//

      c.beginTransaction ();
      c.saveOrUpdate (site);
      c.getTransaction ().commit ();// */

      SiteSMD site1 = SiteSMD.findByName (c, "testSMD5");
      Assert.assertEquals ("SAME Progress", pro, site1.getProgress ().get (0));

      site.getProgress ().get (0).setValue (50);

      SiteSMD site2 = SiteSMD.findByName (c, "testSMD5");
      double ff = site2.getProgress ().get (0).getValue ();
      Assert.assertEquals ("Progress Value updated", 50.0, ff, 0.0);
      c.close ();

   }// */

   
}
