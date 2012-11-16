package eu.imageau.monitor.pages.smd.multi;

import org.apache.tapestry5.annotations.InjectPage;
import org.apache.tapestry5.annotations.Persist;
import org.apache.tapestry5.annotations.Property;
import org.apache.tapestry5.hibernate.annotations.CommitAfter;
import org.apache.tapestry5.ioc.annotations.Inject;
import org.hibernate.Session;

import eu.imageau.monitorSMD.operationSMD.OperationMulti;
import eu.imageau.monitorSMD.operationSMD.SiteSMD;

import eu.imageau.monitor.pages.smd.SmdIndex;

public class MultiCreate
{
   @Property
   @Persist
   private SiteSMD SMDCaller;

   @Inject
   Session connect;

   @Property
   private OperationMulti multi;

   /**
    * Pages
    */
   @InjectPage
   private SmdIndex indexSMD;

   /**
    * Initialization
    */
   public void setup (SiteSMD site)
   {
      this.SMDCaller = site;
   }

   @CommitAfter
   Object onSuccess ()
   {
      this.SMDCaller.addOperation (this.multi);
      this.connect.saveOrUpdate (this.SMDCaller);
      return this.indexSMD;
   }

}
