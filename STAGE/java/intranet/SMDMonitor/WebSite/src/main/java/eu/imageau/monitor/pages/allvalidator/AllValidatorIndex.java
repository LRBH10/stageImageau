package eu.imageau.monitor.pages.allvalidator;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.tapestry5.annotations.InjectPage;
import org.apache.tapestry5.annotations.Log;
import org.apache.tapestry5.annotations.PageLoaded;
import org.apache.tapestry5.annotations.Persist;
import org.apache.tapestry5.annotations.Property;
import org.apache.tapestry5.hibernate.annotations.CommitAfter;
import org.apache.tapestry5.ioc.annotations.Inject;
import org.hibernate.Session;

import eu.imageau.monitorSMD.operationSMD.SiteSMD;

import eu.imageau.monitor.pages.smd.SmdIndex;

public class AllValidatorIndex
{
   @Inject
   Session connect;

   @Property
   @Persist
   private SiteSMD SMDCaller;

   @InjectPage
   SmdIndex indexSmd;

   private List<String> smds;

   public void setup (SiteSMD smd)
   {
      this.SMDCaller = smd;
   }

   @PageLoaded
   void getSMDSCheckBox ()
   {
      this.smds = new ArrayList<String> ();
      List<SiteSMD> lst = SiteSMD.findall (this.connect);
      for (SiteSMD site : lst) {
         this.smds.add (site.getName ());
      }
   }

   @Property
   private String smd;

   @Persist
   private Set<String> selectedSmds;

   @Log
   @CommitAfter
   public Object onSubmitFromMyForm ()
   {
      // SiteSMD all =SiteSMD.findByName (connect, "all");
      for (String siteName : this.selectedSmds) {
         SiteSMD.cloned (this.connect, "all", siteName);
      }

      this.indexSmd.setup (this.SMDCaller);
      return this.indexSmd;
   }

   public boolean isSelected ()
   {
      return false;
   }

   /**
    * Add color to the selected set if selected.
    */
   @Log
   public void setSelected (boolean selected)
   {
      if (selected) {
         this.selectedSmds.add (this.smd);
      }
      else {
         this.selectedSmds.remove (this.smd);
      }
   }

   /**
    * @return SMDs Names
    */
   public List<String> getSmds ()
   {
      return this.smds;
   }

   /**
    * @return Initialized Ok or NNot
    */
   public boolean beginRender ()
   {
      if (this.selectedSmds == null) {
         this.selectedSmds = new HashSet<String> ();
      }
      return true;
   }
}

/*
 * for (String siteName : selectedSmds) { SiteSMD site = SiteSMD.findByName (connect, siteName); List<OperationAbstract> opCloned = new
 * ArrayList<OperationAbstract> (); for (OperationAbstract op : all.getOperations ()) { opCloned.add ((OperationAbstract) op.clone ()); }
 * 
 * System.out.println (System.identityHashCode (opCloned));
 * 
 * OperationAbstract op =(OperationAbstract) all.getOperations ().get (0).clone (); site.addOperation (op); //connect.saveOrUpdate (site); }
 * //
 */
