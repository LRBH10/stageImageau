package eu.imageau.monitor.pages.smd;

import org.apache.tapestry5.annotations.InjectPage;
import org.apache.tapestry5.annotations.Property;
import org.apache.tapestry5.hibernate.annotations.CommitAfter;
import org.apache.tapestry5.ioc.annotations.Inject;
import org.hibernate.Session;

import eu.imageau.monitorSMD.operationSMD.SiteSMD;

import eu.imageau.monitor.pages.Index;

public class SmdCreate
{
   @Inject
   Session connect;

   @Property
   private SiteSMD smd;

   @InjectPage
   private Index index;

   @CommitAfter
   Object onSuccess ()
   {
      if (SiteSMD.findByName (this.connect, this.smd.getName ()) == null) {
         this.connect.save (this.smd);
      }
      return this.index;
   }
}
