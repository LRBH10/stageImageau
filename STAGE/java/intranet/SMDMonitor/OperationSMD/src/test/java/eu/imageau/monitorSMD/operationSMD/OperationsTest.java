package eu.imageau.monitorSMD.operationSMD;

import org.junit.Assert;
import org.junit.Test;

import eu.imageau.monitorSMD.operationSMDEnum.OperationType;

public class OperationsTest
{

   @Test
   public void testSiteSMD ()
   {
      SiteSMD tester1 = new SiteSMD ("SMD");
      Assert.assertNotNull (tester1);
   }

   @Test
   public void testGetNom ()
   {
      SiteSMD tester = new SiteSMD ("SMD");
      Assert.assertEquals ("GetNom ", "SMD", tester.getName ());
   }

   @Test
   public void testGetVersion ()
   {
      SiteSMD tester = new SiteSMD ("SMD");
      tester.setVersion_code ("1.1");
      Assert.assertEquals ("GetVersion ", "1.1", tester.getVersion_code ());
   }

   @Test
   public void testIsConnected ()
   {
      SiteSMD tester = new SiteSMD ("SMD");
      Assert.assertFalse ("GetIsConnectedFalse ", tester.isConnected ());
      tester.setConnected (true);
      Assert.assertTrue ("GetIsConnectedTrue ", tester.isConnected ());
   }

   @SuppressWarnings("deprecation")
   @Test
   public void testAddOperation ()
   {
      SiteSMD tester = new SiteSMD ("SMD");
      OperationAbstract op = new OperationScript ("make", "monfichier");

      Assert.assertNotNull ("Operation Constructure", op);
      Assert.assertEquals ("Type Script Verfiy", OperationType.OperationScript, op.getType ());

      OperationAbstract op2 = new OperationMulti (tester);
      Assert.assertEquals ("Type Script Verfiy", OperationType.OperationMulti, op2.getType ());

      tester.addOperation (op);
      Assert.assertEquals ("AddOperation", 1, tester.getOperations ().size ());
   }

}
