package eu.imageau.monitor.OperationsEntities;

import java.util.Calendar;
import java.util.Date;

public class ProgressClean
{
   public Date avant;
   public boolean fini;

   public ProgressClean ()
   {
      Calendar now = Calendar.getInstance ();
      now.add (Calendar.MONTH, -4);
      this.avant = now.getTime ();
   }
}
