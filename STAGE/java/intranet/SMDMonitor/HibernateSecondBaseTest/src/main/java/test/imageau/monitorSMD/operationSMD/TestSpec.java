/**
 * 
 */

package test.imageau.monitorSMD.operationSMD;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Inheritance;
import javax.persistence.InheritanceType;
import javax.persistence.Table;

@Entity
@Inheritance(strategy = InheritanceType.JOINED)
@Table(name = "test")
public class TestSpec
{
   @Id
   @GeneratedValue(strategy = GenerationType.IDENTITY)
   long id;

   String name;

   String date;

   public TestSpec (String n, String d)
   {
      this.name = n;
      this.date = d;
   }

   public String getDate ()
   {
      return this.date;
   }

   public void setDate (String date)
   {
      this.date = date;
   }

   public String getName ()
   {
      return this.name;
   }

   public void setName (String name)
   {
      this.name = name;
   }
}
