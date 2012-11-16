package eu.imageau.monitorSMD.data;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Table;

import org.apache.tapestry5.ioc.annotations.Inject;
import org.hibernate.Session;
import org.hibernate.criterion.Restrictions;

import com.google.gson.annotations.Expose;

@Entity
@Table(name = "operation_report")
public class ReportData
{
   @Id
   @GeneratedValue(strategy = GenerationType.IDENTITY)
   @Column(name = "id_report")
   @Expose
   protected long id;// */

   @Expose
   boolean status;

   @Expose
   String contents;

   @Inject
   public ReportData ()
   {
      this.status = false;
      this.contents = "Aucun rapport";
   }

   public void setContents (String contents)
   {
      this.contents = contents;
   }

   public String getContents ()
   {
      return this.contents;
   }

   public boolean getStatus ()
   {
      return this.status;
   }

   public void setStatus (boolean status)
   {
      this.status = status;
   }

   public static ReportData findById (Session session, long id)
   {
      ReportData out = null;
      out = (ReportData) session.createCriteria (ReportData.class).add (Restrictions.eq ("id", id)).uniqueResult ();
      return out;
   }

   @Override
   public String toString ()
   {
      return "ID: " + this.id + "\tStatus : " + this.status + "\tContents : " + this.contents;
   }

   /*
    * public String getAddressMemoire () { return super.toString (); }//
    */
}
