package eu.imageau.monitorSMD.svnmanager;

import org.junit.Test;

public class SvnKitConfigFileTest
{

   @Test
   public void testSvnCheckout ()
   {
      SvnMangerConfigSMD svnStub = new SvnMangerConfigSMD ("testSMD");
      svnStub.svnCheckout ();
      System.out.println (svnStub.getFilesConfigs ());

      svnStub.svnCommit ();// */

   }

   @Test
   public void testGetFilesConfigs ()
   {
      SvnMangerConfigSMD svnStub = new SvnMangerConfigSMD ("testSMD");
      svnStub.getFilesConfigs ();
   }

   @Test
   public void testSvnCommit ()
   {
      SvnMangerConfigSMD svnStub = new SvnMangerConfigSMD ("testSMD");
      svnStub.svnCommit ();
   }

}
