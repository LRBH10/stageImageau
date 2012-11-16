package eu.imageau.monitorSMD.data;

import java.util.Calendar;
import java.util.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;

import org.hibernate.Session;
import org.hibernate.annotations.Cascade;
import org.hibernate.annotations.CascadeType;
import org.hibernate.criterion.Restrictions;

import eu.imageau.monitorSMD.operationSMD.SiteSMD;

@Entity
@Table(name = "operation_progress")
public class ProgressData
{
   @Id
   @Column(name = "id_progress")
   protected long id;// */
   String title;
   double value;
   boolean finished;
   Date last_date;

   @ManyToOne
   @Cascade({ CascadeType.ALL })
   @JoinColumn(name = "id_smd", nullable = false)
   private SiteSMD smd;

   public ProgressData ()
   {

   }

   public ProgressData (String title, double val, SiteSMD site)
   {
      this.smd = site;
      this.id = hashCode ();
      this.title = title;
      this.value = val;
      this.finished = false;
      this.last_date = Calendar.getInstance ().getTime ();
   }

   public String getTitle ()
   {
      return this.title;
   }

   public long getId ()
   {
      return this.id;
   }

   public double getValue ()
   {
      return this.value;
   }

   public boolean isFinished ()
   {
      return this.finished;
   }

   public void setFinished (boolean finished)
   {
      this.finished = finished;
   }

   public void setId (long id)
   {
      this.id = id;
   }

   public void setTitle (String title)
   {
      this.title = title;
   }

   public void setValue (double value)
   {
      this.value = value;
   }

   public Date getLast_date ()
   {
      return this.last_date;
   }

   public void setLast_date (Date last_date)
   {
      this.last_date = last_date;
   }

   public SiteSMD getSmd ()
   {
      return this.smd;
   }

   public void setSmd (SiteSMD smd)
   {
      this.smd = smd;
   }

   public static ProgressData findById (Session session, long id)
   {
      ProgressData out = null;
      out = (ProgressData) session.createCriteria (ProgressData.class).add (Restrictions.eq ("id", id)).uniqueResult ();
      return out;
   }

   @Override
   public String toString ()
   {

      return "ID:'" + this.id + "'  TITLE:'" + this.title + "'\tValue:'" + this.value + "'\tLAST DATE:'" + this.last_date + "'\tFinished:'"
            + this.finished + "'";
   }

}
