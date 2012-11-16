package eu.imageau.monitor.pages;

import org.apache.tapestry5.annotations.Log;

public class Contact implements Runnable
{

   @Override
   @Log
   public void run ()
   {
      // TODO Auto-generated method stub
      while (true) {
         System.out.println ("rabah");
         try {
            Thread.sleep (2000);
         }
         catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace ();
         }
      }
   }

}
