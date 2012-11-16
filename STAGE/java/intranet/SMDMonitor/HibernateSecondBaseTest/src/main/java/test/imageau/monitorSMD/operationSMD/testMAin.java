package test.imageau.monitorSMD.operationSMD;

import org.hibernate.Session;

import test.imageau.monitorSMD.HibernateSMD.HibernateUtil;

public class testMAin
{

   /**
    * @param args
    */
   public static void main (String[] args)
   {
      Session data = HibernateUtil.getSessionFactory ().openSession ();
      data.beginTransaction ();
      TestSpec ss = new TestSpec ("rabah", "sdq");
      data.save (ss);
      data.getTransaction ().commit ();
      data.close ();
   }

}
