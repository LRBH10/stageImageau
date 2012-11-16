package eu.imageau.monitor.pages.tests;

import org.apache.tapestry5.annotations.Persist;
import org.apache.tapestry5.annotations.Property;

public class TestPersistance
{

   @Property
   @Persist
   String testEscape;

   public void setup (int i)
   {
      // TODO Auto-generated method stub
   }

   public void onActivate ()
   {
      
      this.testEscape = "rabah <br/> laouadi ";
   }

}
