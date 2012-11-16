package eu.imageau.monitorSMD.operationSMD;

import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.OneToMany;
import javax.persistence.Table;

import org.apache.tapestry5.beaneditor.Validate;
import org.apache.tapestry5.ioc.annotations.Inject;
import org.hibernate.Session;
import org.hibernate.annotations.Cascade;
import org.hibernate.annotations.CascadeType;
import org.hibernate.criterion.Restrictions;

import com.google.gson.annotations.Expose;

import eu.imageau.monitorSMD.data.ProgressData;

@Entity
@Table(name = "monitor_smd")
@SuppressWarnings("unchecked")
public class SiteSMD
{
   @Id
   @GeneratedValue(strategy = GenerationType.IDENTITY)
   @Expose
   protected long id;

   @Expose
   @Validate("required, minlength=3")
   private String name;

   @Expose
   private String version_code;

   @Expose
   private String version_system;

   @Expose
   private boolean connected;

   @Expose
   private boolean sealed;

   @Expose
   private boolean have_a_probleme;

   @OneToMany(mappedBy = "smd", fetch = FetchType.LAZY)
   @Cascade({ CascadeType.ALL })
   @Expose
   private List<OperationAbstract> operations = new LinkedList<OperationAbstract> ();

   @OneToMany(mappedBy = "smd", fetch = FetchType.LAZY)
   @Cascade({ CascadeType.ALL })
   @Expose
   private List<ProgressData> progress = new LinkedList<ProgressData> ();

   @Inject
   public SiteSMD ()
   {

   }

   /**
    * SETTERS AND GETTERS
    * 
    * @return values of atturibute
    */
   public long getId ()
   {
      return this.id;
   }

   public String getName ()
   {
      return this.name;
   }

   public SiteSMD (String name)
   {
      this.name = name;
      this.connected = false;
   }

   public void setNom (String nom)
   {
      this.name = nom;
   }

   public boolean isConnected ()
   {
      return this.connected;
   }

   public void setConnected (boolean isConnected)
   {
      this.connected = isConnected;
   }

   public void setName (String name)
   {
      this.name = name;
   }

   public String getVersion_code ()
   {
      return this.version_code;
   }

   public void setVersion_code (String version_code)
   {
      this.version_code = version_code;
   }

   public String getVersion_system ()
   {
      return this.version_system;
   }

   public void setVersion_system (String version_system)
   {
      this.version_system = version_system;
   }

   public boolean isSealed ()
   {
      return this.sealed;
   }

   public void setSealed (boolean sealed)
   {
      this.sealed = sealed;
   }

   @Deprecated
   /**
    * Warning this function will get you all PROGRESS in the SMD
    * be careful when you use it
    * @return All progress 
    */
   public List<ProgressData> getProgress ()
   {
      return this.progress;
   }

   public List<ProgressData> getProgressByFinished (Session connect, boolean status)
   {
      String req = "from " + ProgressData.class.getSimpleName () + " where finished=" + status + " and smd='" + this.id
            + "' order by last_date";
      return connect.createQuery (req).list ();
   }

   @Deprecated
   /**
    * Warning this function will get you all Operation in the SMD
    * be careful when you use it
    * @return All operation 
    */
   public List<OperationAbstract> getOperations ()
   {
      return this.operations;
   }

   public List<OperationAbstract> getOperationsByAchievedNotNested (Session connect, boolean status)
   {
      
      String req = "from OperationAbstract where achieved=" + status + " and smd='" + this.id + "' and nested=false ";
      return connect.createQuery (req).list ();
   }

   public void setId (long id)
   {
      this.id = id;
   }

   /**
    * END OF SETTERS AND GETTERS
    * 
    * @see java.lang.Object#toString()
    */

   @Override
   public String toString ()
   {
      return "\n SMD " + this.name + "\n Nbr Operations " + this.operations.size () + "\t" + this.operations + "\n Nbr Progress "
            + this.progress.size () + "\t" + this.progress;
   }

   /**
    * Is There Operation To DO
    * 
    * @return
    */
   public boolean haveOperationsToDo (Session connect)
   {
      String req = "from " + OperationAbstract.class.getSimpleName () + " where achieved='false' and smd='" + this.id + "'";
      List<OperationAbstract> lst = connect.createQuery (req).list ();

      System.out.println (lst);
      return !lst.isEmpty ();
   }

   /**
    * 
    * @return state of operations
    */
   public double stateOfOperations (Session connect)
   {
      String req = "from " + OperationAbstract.class.getSimpleName () + " where achieved='false' and smd='" + this.id
            + "' and nested=false and state between 1 and 100";
      List<OperationAbstract> lst = connect.createQuery (req).list ();

      double res = 0;
      for (int i = 0; i < lst.size (); i++) {
         res += lst.get (i).getState ();
      }

      if (lst.isEmpty ()) {
         res = 0;
      }
      else {
         res = res / lst.size ();
      }

      return res;
   }

   /**
    * 
    * @return all SMDs
    */
   public static List<SiteSMD> findall (Session session)
   {
      List<SiteSMD> out = session.createCriteria (SiteSMD.class).list ();
      return out;
   }

   /**
    * The SMD getter By NAME
    * 
    * @param session : Hibernate
    * @param name : of SMD
    * @return SMD
    */
   public static SiteSMD findByName (Session session, String name)
   {
      SiteSMD out = (SiteSMD) session.createCriteria (SiteSMD.class).add (Restrictions.eq ("name", name)).uniqueResult ();
      return out;
   }

   /**
    * the SMD getter By ID
    * 
    * @param session : Hibenate
    * @param id : of SMD
    * @return SMD
    */
   public static SiteSMD findById (Session session, long id)
   {
      SiteSMD out = (SiteSMD) session.createCriteria (SiteSMD.class).add (Restrictions.eq ("id", id)).uniqueResult ();
      return out;
   }

   /**
    * get Running Progress Only
    * 
    * @return List< OperationProgress>
    */
   public List<ProgressData> getProgressRunning (Session connect)
   {
      String req = "from " + ProgressData.class.getSimpleName () + " where smd='" + this.id + "' and finished=false";
      return connect.createQuery (req).list ();
   }

   /**
    * Add Operation
    * 
    * @param operation
    */
   public void addOperation (OperationAbstract operation)
   {
      operation.setSmd (this);
      this.operations.add (operation);
   }

   public void addOperations (List<OperationAbstract> operationsCaming)
   {
      for (OperationAbstract op : operationsCaming) {
         op.setSmd (this);
      }
      this.operations.addAll (operationsCaming);
   }

   /**
    * Add Progress
    * 
    * @param progressCaming
    */
   public void addProgress (ProgressData progressCaming)
   {
      progressCaming.setSmd (this);
      this.progress.add (progressCaming);
   }

   /**
    * Clonning the Operation of Site Source into site destination
    * 
    * @param connect Session
    * @param siteSource the operation to duplicate
    * @param siteDestination the site whitch have the operation of source
    * @return true if ok, false if one of the SMd Does not exist
    */
   public static boolean cloned (Session connect, String siteSource, String siteDestination)
   {

      SiteSMD all = SiteSMD.findByName (connect, siteSource);
      SiteSMD site = SiteSMD.findByName (connect, siteDestination);

      if (all == null || site == null) {
         return false;
      }
      for (OperationAbstract allOps : all.getOperations ()) {
         allOps.duplicateInBase (connect, site);
      }

      return true;
   }

   /**
    * for Search SMDs
    * 
    * @param connect Session of connection
    * @param likeit the word to search
    * @return
    */
   public static List<SiteSMD> findByLike (Session connect, String likeit)
   {
      String req = "from " + SiteSMD.class.getSimpleName () + " where name like '" + likeit + "%'";
      return connect.createQuery (req).list ();
   }

   public boolean isHave_a_probleme ()
   {
      return this.have_a_probleme;
   }

   public void setHave_a_probleme (boolean have_a_probleme)
   {
      this.have_a_probleme = have_a_probleme;
   }

   public String getVersion (Session connect)
   {
      SiteSMD site = SiteSMD.findById (connect, getId ());
      return site.getVersion_code () + "_" + site.getVersion_system ();
   }

   /**
    * Clean Progress Data
    * 
    * @param connect Session Hibernate
    * @param befor clean all Progress before this date
    * @param finished clean the progress finished only
    */
   public void cleanProgress (Session connect, Date befor, boolean finished)
   {
      String req = "from " + ProgressData.class.getSimpleName () + " where smd='" + this.id + "' ";
      if (finished) {
         req = req + " and  finished = false";
      }

      List<ProgressData> lst = connect.createQuery (req).list ();

      for (int i = 0; i < lst.size (); i++) {
         if (lst.get (i).getLast_date ().compareTo (befor) < 0) {
            lst.get (i).setSmd (null);
            connect.delete (lst.get (i));
         }
      }
   }

   /**
    * Clean Operations
    * 
    * @param connect Session Hibernate
    * @param befor clean all Operation before this date
    */
   public void cleanOperations (Session connect, Date befor)
   {
      String req = "from " + OperationAbstract.class.getSimpleName () + " where smd='" + this.id + "' ";
      List<OperationAbstract> lst = connect.createQuery (req).list ();

      for (int i = 0; i < lst.size (); i++) {
         if (lst.get (i).getDate_added ().compareTo (befor) < 0) {
            lst.get (i).setSmd (null);
            connect.delete (lst.get (i));
         }
      }

   }

   /**
    * To get Connection in Refresh Wb Site
    * 
    * @param connect
    * @return
    */
   public boolean isConnectedRefresh (Session connect)
   {
      SiteSMD site = SiteSMD.findById (connect, this.id);
      return site.isConnected ();
   }

   /**
    * To get if or not the SMd have an Problem
    * 
    * @param connect
    * @return
    */
   public boolean isHave_a_problemeRefresh (Session connect)
   {

      SiteSMD site = SiteSMD.findById (connect, this.id);
      return site.isHave_a_probleme ();
   }

   /**
    * Update on achievd Progress
    * 
    * @param connect
    */
   public void updateProgress (Session connect)
   {
      String req = "from " + ProgressData.class.getSimpleName () + " where smd='" + this.id + "' and finished=false";
      List<ProgressData> lst = connect.createQuery (req).list ();

      SiteSMD site = SiteSMD.findById (connect, this.id);
      if (lst.isEmpty ()) {
         site.setHave_a_probleme (false);
         connect.update (site);
      }
      else {
         site.setHave_a_probleme (true);
         connect.update (site);
      }

   }
}
