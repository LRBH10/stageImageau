package eu.imageau.monitorSMD.svnmanager;

public class FileConfig
{
   public String name;
   public String path;

   public FileConfig (String name, String contets)
   {
      // TODO Auto-generated constructor stub
      this.name = name;
      this.path = contets;
   }

   @Override
   public String toString ()
   {
      // TODO Auto-generated method stub
      return "(" + this.name + " " + this.path + ")";
   }

}
