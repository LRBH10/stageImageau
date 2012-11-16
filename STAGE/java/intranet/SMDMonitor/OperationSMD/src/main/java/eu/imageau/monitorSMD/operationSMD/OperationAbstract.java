/**
 * 
 */

package eu.imageau.monitorSMD.operationSMD;

import java.util.Calendar;
import java.util.Date;

import javax.persistence.DiscriminatorColumn;
import javax.persistence.Entity;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Inheritance;
import javax.persistence.InheritanceType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToOne;
import javax.persistence.Table;

import org.apache.tapestry5.beaneditor.NonVisual;
import org.apache.tapestry5.beaneditor.Validate;
import org.hibernate.Session;
import org.hibernate.annotations.Cascade;
import org.hibernate.annotations.CascadeType;
import org.hibernate.criterion.Restrictions;

import com.google.gson.annotations.Expose;

import eu.imageau.monitorSMD.HibernateSMD.HibernateUtil;
import eu.imageau.monitorSMD.data.ReportData;
import eu.imageau.monitorSMD.operationSMDEnum.OperationType;

@Entity
@Inheritance(strategy = InheritanceType.JOINED)
@Table(name = "operation")
@DiscriminatorColumn(name = "type")
public abstract class OperationAbstract
{
   @Id
   @GeneratedValue(strategy = GenerationType.IDENTITY)
   @Expose
   long id;

   @ManyToOne
   @Cascade({ CascadeType.ALL })
   @JoinColumn(name = "id_smd")
   private SiteSMD smd;

   @Validate("required,minlength=5")
   private String description;

   @NonVisual
   @Enumerated(EnumType.STRING)
   @Expose
   private OperationType type;

   @Expose
   @Validate("required,minlength=3")
   private String user;

   @Expose
   @Validate("required")
   private String path = "/home/";

   @Expose
   private double state = 0.0;

   @Expose
   private boolean achieved = false;

   @Expose
   private boolean nested = false;

   @Expose
   private boolean report_on_success = false;

   Date date_added;

   public OperationAbstract ()
   {

      this.date_added = Calendar.getInstance ().getTime ();
   }

   public Date getDate_added ()
   {
      return this.date_added;
   }

   public void setDate_added (Date date_added)
   {
      this.date_added = date_added;
   }

   @OneToOne
   @Cascade({ CascadeType.ALL })
   @JoinColumn(name = "id_report", nullable = false)
   private ReportData report = new ReportData ();

   public void setType (OperationType type)
   {
      this.type = type;
   }

   public boolean isReport_on_success ()
   {
      return this.report_on_success;
   }

   public void setId (long id)
   {
      this.id = id;
   }

   public void setReport_on_success (boolean report_on_success)
   {
      this.report_on_success = report_on_success;
   }

   public void setReport (ReportData report)
   {
      this.report = report;
   }

   public ReportData getReport ()
   {
      return this.report;
   }

   public long getId ()
   {
      return this.id;
   }

   public String getPath ()
   {
      return this.path;
   }

   public boolean isAchieved ()
   {
      return this.achieved;
   }

   public SiteSMD getSmd ()
   {
      return this.smd;
   }

   public OperationType getType ()
   {
      return this.type;
   }

   public String getUser ()
   {
      return this.user;
   }

   public double getState ()
   {
      return this.state;
   }

   public void setPath (String path)
   {
      this.path = path;
   }

   public void setAchieved (boolean isAchieved)
   {
      this.achieved = isAchieved;
   }

   public void setNested (boolean isNested)
   {
      this.nested = isNested;
   }

   public boolean isNested ()
   {
      return this.nested;
   }

   public void setUser (String user)
   {
      this.user = user;
   }

   public void setSmd (SiteSMD smd)
   {
      this.smd = smd;
   }

   public void setState (double state)
   {
      this.state = state;
   }

   public static OperationAbstract findById (Session session, long id)
   {
      OperationAbstract out = null;
      out = (OperationAbstract) session.createCriteria (OperationAbstract.class).add (Restrictions.eq ("id", id)).uniqueResult ();
      return out;
   }

   public static OperationAbstract findByIdWithType (long id)
   {
      Session session = HibernateUtil.getSessionFactory ().openSession ();
      OperationAbstract out = null;
      out = (OperationAbstract) session.createCriteria (OperationAbstract.class).add (Restrictions.eq ("id", id)).uniqueResult ();
      session.close ();
      return out;
   }

   public String getDescription ()
   {
      return this.description;
   }

   public void setDescription (String description)
   {
      this.description = description;
   }

   @Override
   public String toString ()
   {

      return super.toString () + "\n id: " + this.id + "\n SMD : " + this.smd.getName () + "\n User : " + this.user + "\n Type : "
            + this.type + "\n Path : " + this.path + "\n State : " + this.state + "\n Achieved : " + this.achieved + "\n report on succes"
            + this.report_on_success + "(" + this.report + ")\n Date : " + this.date_added;
   }

   /*
    * public String getAddressMemoire () { return super.toString (); }//
    */

   public OperationAbstract duplicateInBase (Session connect, SiteSMD newSmd)
   {
      connect.evict (this);
      connect.evict (getReport ());

      setSmd (newSmd);

      connect.save (getReport ());
      connect.save (this);

      return this;

   }

}
