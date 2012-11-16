package eu.imageau.monitor.OperationsEntities;

public class CheckBoxMatch
{
   public boolean selected;
   public String label;

   public CheckBoxMatch (String label)
   {
      // TODO Auto-generated constructor stub
      this.label = label;
   }

   @Override
   public String toString ()
   {
      // TODO Auto-generated method stub
      return "(" + this.label + " : " + this.selected + ")";
   }
}
