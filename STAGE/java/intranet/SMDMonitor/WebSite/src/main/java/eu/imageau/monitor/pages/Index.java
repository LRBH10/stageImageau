package eu.imageau.monitor.pages;

import java.util.ArrayList;
import java.util.List;

import org.apache.tapestry5.EventConstants;
import org.apache.tapestry5.annotations.InjectPage;
import org.apache.tapestry5.annotations.OnEvent;
import org.apache.tapestry5.annotations.Persist;
import org.apache.tapestry5.annotations.Property;
import org.apache.tapestry5.hibernate.annotations.CommitAfter;
import org.apache.tapestry5.ioc.annotations.Inject;
import org.apache.tapestry5.services.Request;
import org.hibernate.Session;

import eu.imageau.monitor.pages.smd.SmdCreate;
import eu.imageau.monitor.pages.smd.SmdIndex;
import eu.imageau.monitor.pages.tests.TestPersistance;
import eu.imageau.monitorSMD.data.ProgressData;
import eu.imageau.monitorSMD.operationSMD.SiteSMD;

/**
 * Start page of application Monitor.
 */

@SuppressWarnings("unused")
public class Index
{
   /**
    * Attributes
    */
   @Property
   private SiteSMD smd;

   @Inject
   Session connect;

   /**
    * Pages
    **/
   @InjectPage
   private SmdCreate newSmd;

   /**
    * To lock in SMD properties
    */

   @InjectPage
   private SmdIndex smdindex;

   /**
    * Progress Presentation
    */
   @Property
   private ProgressData progress;
   /**
    * Test Pages
    */
   @InjectPage
   private TestPersistance simpletest;

   @InjectPage
   private Index refreshIndex;

   /**
    * Data Index
    */
   @Property
   @Persist
   private List<SiteSMD> lstsmds; 

   @Property
   @Persist
   private List<String> indexSmds;

   @Persist
   private boolean boolSearch;

   /**
    * Functions
    */
   public void setup (String searchWord, boolean search)
   {
      this.search = searchWord;
      this.boolSearch = search;
   }

   /***
    * Create a new SMD
    */
   Object onActionFromSmdCreate ()
   {
      return this.newSmd;
   }

   /**
    * To Choose an SMD an lock it properties
    */
   @CommitAfter
   Object onActionFromSmdChoose (String idName)
   {
      SiteSMD site = SiteSMD.findByName (this.connect, idName);
      this.smdindex.setup (site);
      return this.smdindex;
   }

   @CommitAfter
   Object onActionFromSmdDelete (long id)
   {
      SiteSMD smddelete = SiteSMD.findById (this.connect, id);
      this.connect.delete (smddelete);
      this.boolSearch = true;
      return this;
   }

   /**
    * For test
    */
   Object onActionFromTest ()
   {
      this.simpletest.setup (21);
      return this.simpletest;
   }

   /**
    * List of smd string
    */
   @OnEvent(value = "provideCompletions")
   public List<String> autoComplete (String start)
   {
      List<String> strings = new ArrayList<String> ();

      if (start != null) {
         for (SiteSMD value : this.lstsmds) {
            if (value.getName ().toUpperCase ().startsWith (start.toUpperCase ())) {
               strings.add (value.getName ());
            }
         }
      }
      return strings;
   }

   /**
    * List SMDs
    */
   public List<SiteSMD> getListSMDs ()
   {
      if (!this.boolSearch) {
         this.lstsmds = SiteSMD.findall (this.connect);
      }
      else {
         this.lstsmds = SiteSMD.findByLike (this.connect, this.search);
      }

      if (this.indexSmds == null) {
         this.indexSmds = new ArrayList<String> ();
      }

      for (int i = 0; i < this.lstsmds.size (); i++) {
         String str = "smd_" + i;
         if (!this.indexSmds.contains (str)) {
            this.indexSmds.add (str);
         }
      }
      return this.lstsmds;
   }

   @Property
   private int rowAdded;

   @Property
   @Persist
   private String search;

   Object onValidateFromSearchForm ()
   {
      if (this.search == null) {
         this.refreshIndex.setup (this.search, false);
      }
      else {
         this.refreshIndex.setup (this.search, true);
      }
      return this.refreshIndex;
   }

   /**
    * For new Line GRID
    * 
    * @return
    */
   public boolean getnewRow ()
   {
      boolean test;
      if (this.rowAdded % 4 == 0) {
         test = true;
      }
      else {
         test = false;
      }
      return test;
   }

   /**
    * For the Zone SMD Refresh
    */

   @Property
   private List<ProgressData> ProgressRunning;

   @Property
   @Persist
   boolean connectedSmd;

   @Property
   @Persist
   boolean haveOperationToDo;

   @Property
   @Persist
   double stateOfOperations;

   @Property
   @Persist
   String versionSystem;

   @Property
   @Persist
   String versionCode;

   @Property
   @Persist
   boolean haveAProblem;

   /**
    * Function that return the values properties of SMD
    */
   @OnEvent(value = EventConstants.REFRESH)
   void refreshZoneSMD (String x)
   {
      if (this.request.isXHR ()) {
         String smdName = this.request.getPath ().split ("/") [this.request.getPath ().split ("/").length - 1];

         int is = Integer.parseInt (smdName.split ("_") [1]);
         this.ProgressRunning = this.lstsmds.get (is).getProgressRunning (this.connect);

         this.connectedSmd = this.lstsmds.get (is).isConnectedRefresh (this.connect);
         this.haveAProblem = this.lstsmds.get (is).isHave_a_problemeRefresh (this.connect);

         this.haveOperationToDo = this.lstsmds.get (is).haveOperationsToDo (this.connect);

         this.stateOfOperations = this.lstsmds.get (is).stateOfOperations (this.connect);

         String[] version = this.lstsmds.get (is).getVersion (this.connect).split ("_");
         this.versionSystem = version [1];
         this.versionCode = version [0];
      }

   }

   @Inject
   private Request request;
}
