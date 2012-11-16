package eu.imageau.monitor.OperationsEntities;

import java.util.Date;

import org.apache.tapestry5.annotations.Import;

import eu.imageau.monitorSMD.operationSMDEnum.OperationType;


@Import(stylesheet = "context:layout/css/beandisplay/alldisplay.css")
public class FOperationAbstract
{

   public long id;

   public String path;

   public String user;

   public double state;

   public boolean achieved;
   public boolean nested;
   public boolean report_on_success;

   public OperationType type;
   public String description = "Test";

   public Date dateAdded;

   @Override
   public String toString ()
   {
      // TODO Auto-generated method stub
      return "Id(" + this.id + ")";
   }

}
