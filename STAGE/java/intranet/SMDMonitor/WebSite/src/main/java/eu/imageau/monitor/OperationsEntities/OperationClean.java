package eu.imageau.monitor.OperationsEntities;

import java.util.Calendar;
import java.util.Date;

public class OperationClean
{
   public Date avant;

   public OperationClean ()
   {
      Calendar now = Calendar.getInstance ();
      now.add (Calendar.MONTH, -4);
      this.avant = now.getTime ();
   }
}
