package eu.imageau.monitorSMD.operationSMD;

import org.hibernate.Session;

import eu.imageau.monitorSMD.HibernateSMD.HibernateUtil;

public class TestMain
{

   /**
    * @param args
    */
   public static void main (String[] args)
   {
      Session connect = HibernateUtil.getSessionFactory ().openSession ();
      connect.beginTransaction ();

      //Calendar now = Calendar.getInstance ();

      /*
       * SiteSMD site = SiteSMD.findByName (connect, "testSMD");
       * 
       * ProgressData ss = new ProgressData ("progress AQui Terminer", 150, site); now.add (Calendar.MONTH, -5); ss.setLast_date
       * (now.getTime ());
       * 
       * 
       * ss.setId (0); ss.setFinished (true); connect.save (ss);
       * 
       * 
       * ProgressData ss1 = new ProgressData ("progress AQui ", 50, site); now.add (Calendar.MONTH, -1); ss1.setLast_date (now.getTime ());
       * 
       * ss1.setId (1); connect.save (ss1);
       * 
       * 
       * 
       * 
       * ProgressData ss2 = new ProgressData ("progress AQui Terminer", 120, site); ss2.setId (2); ss2.setFinished (true); connect.save
       * (ss2);
       * 
       * 
       * ProgressData ss3 = new ProgressData ("progress AQui ", 20, site); ss3.setId (3); connect.save (ss3);//
       */

      /*
       * ProgressData fs = ProgressData.findById (connect, 0); //
       */

      /*
       * SiteSMD site = SiteSMD.findByName (connect, "gds1"); site.setConnected (true); site.setVersion_code ("20"); connect.saveOrUpdate
       * (site);//
       */

      /*
       * SiteSMD site = SiteSMD.findByName (connect, "cit1"); System.out.println (site.stateOfOperations (connect));//
       */

      /*
       * SiteSMD site = SiteSMD.findByName (connect, "testSMD"); System.out.println(site.getProgressByFinished (connect, false));//
       */

      /*
       * SiteSMD site = SiteSMD.findByName (connect, "testSMD"); now.add (Calendar.MONTH, -4); site.cleanProgress (connect,now.getTime (),
       * false);//
       */

      connect.getTransaction ().commit ();
      connect.close ();
   }
}
