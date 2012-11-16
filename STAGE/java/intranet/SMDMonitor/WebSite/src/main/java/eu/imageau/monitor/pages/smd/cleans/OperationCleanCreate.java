package eu.imageau.monitor.pages.smd.cleans;

import org.apache.tapestry5.annotations.InjectPage;
import org.apache.tapestry5.annotations.Persist;
import org.apache.tapestry5.annotations.Property;
import org.apache.tapestry5.hibernate.annotations.CommitAfter;
import org.apache.tapestry5.ioc.annotations.Inject;
import org.hibernate.Session;

import eu.imageau.monitor.OperationsEntities.OperationClean;
import eu.imageau.monitor.pages.smd.SmdIndex;
import eu.imageau.monitorSMD.operationSMD.SiteSMD;

public class OperationCleanCreate
{
   @Inject
   Session connect;

   @Property
   @Persist
   private SiteSMD SMDCaller;

   @Property
   private OperationClean operationclean;

   @InjectPage
   private SmdIndex index;

   public void setup (SiteSMD site)
   {
      this.SMDCaller = site;
   }

   @CommitAfter
   Object onSuccess ()
   {
      this.SMDCaller.cleanOperations (this.connect, this.operationclean.avant);
      this.index.setup (this.SMDCaller);
      return this.index;
   }
}
