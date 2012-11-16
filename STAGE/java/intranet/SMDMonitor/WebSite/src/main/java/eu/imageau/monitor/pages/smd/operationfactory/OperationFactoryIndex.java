package eu.imageau.monitor.pages.smd.operationfactory;

import org.apache.tapestry5.annotations.Persist;
import org.apache.tapestry5.annotations.Property;
import org.apache.tapestry5.ioc.annotations.Inject;
import org.hibernate.Session;

import eu.imageau.monitorSMD.operationSMD.OperationAbstract;


@SuppressWarnings("unused")
public class OperationFactoryIndex
{
   @Inject
   Session connect;

   @Property
   @Persist
   private OperationAbstract operation;

   @Property
   @Persist
   private String report;

   public void setup (long id)
   {
      this.operation = OperationAbstract.findById (this.connect, id);
      this.report = this.operation.getReport ().getContents ().replaceAll ("\n", "<br/>");
   }

}
