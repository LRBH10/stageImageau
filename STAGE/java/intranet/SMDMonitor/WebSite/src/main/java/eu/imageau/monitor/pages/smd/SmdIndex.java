package eu.imageau.monitor.pages.smd;

import java.util.ArrayList;
import java.util.List;

import org.apache.tapestry5.Block;
import org.apache.tapestry5.annotations.InjectPage;
import org.apache.tapestry5.annotations.Log;
import org.apache.tapestry5.annotations.Persist;
import org.apache.tapestry5.annotations.Property;
import org.apache.tapestry5.hibernate.annotations.CommitAfter;
import org.apache.tapestry5.ioc.annotations.Inject;
import org.tmatesoft.svn.core.SVNException;

import eu.imageau.monitor.OperationsEntities.FOperationAbstract;
import eu.imageau.monitor.base.FOpConverter;
import eu.imageau.monitor.base.OperationDetails;
import eu.imageau.monitor.pages.allvalidator.AllValidatorIndex;
import eu.imageau.monitor.pages.fileeditor.FileConfigEditor;
import eu.imageau.monitor.pages.smd.cleans.OperationCleanCreate;
import eu.imageau.monitor.pages.smd.cleans.ProgressCleanCreate;
import eu.imageau.monitor.pages.smd.multi.MultiIndex;
import eu.imageau.monitorSMD.data.ProgressData;
import eu.imageau.monitorSMD.operationSMD.OperationAbstract;
import eu.imageau.monitorSMD.operationSMD.SiteSMD;
import eu.imageau.monitorSMD.operationSMDEnum.OperationType;
import eu.imageau.monitorSMD.svnmanager.FileConfig;
import eu.imageau.monitorSMD.svnmanager.SvnMangerConfigSMD;

@SuppressWarnings("unused")
public class SmdIndex extends OperationDetails
{
   /**
    * Attributes
    */

   /**
    * The SITE SMD SELECTED VARIABLe
    */
   @Property
   @Persist
   private SiteSMD whoIam;

   /**
    * Pages
    */

   @InjectPage
   private MultiIndex indexMulti;

   @InjectPage
   private FileConfigEditor fileEditor;

   /**
    * Functions
    */
   /***
    * Initializations
    * */
   public void setup (SiteSMD smd)
   {
      this.whoIam = smd;
      if (smd.getName ().equals ("all")) {
         this.all = true;
      }
      else {
         this.all = false;
      }
   }

   /***
    * Multiple Create
    */
   Object onActionFromOperationFactoryCreate (OperationType type)
   {
      this.newOperation.setup (this.whoIam, type);
      return this.newOperation;
   }

   /**
    * Suppression d'une operation
    * 
    * @param id
    * @return
    */
   @CommitAfter
   Object onActionFromDelete (long id)
   {
      OperationAbstract opdelete = OperationAbstract.findById (this.connect, id);
      opdelete.setSmd (null);
      this.connect.delete (opdelete);
      return this;
   }

   Object onActionFromUpdate (long id)
   {
      OperationAbstract res = OperationAbstract.findById (this.connect, id);
      this.updateOperation.setup (this.whoIam, id);
      return this.updateOperation;
   }

   /***
    * Select Operations
    */
   @Override
   public Object onActionFromSelect (long id)
   {
      OperationAbstract res = OperationAbstract.findById (this.connect, id);

      switch (res.getType ()) {
         case OperationMulti:
            this.indexMulti.setup (this.whoIam, id);
            return this.indexMulti;

         default:
            this.indexOperation.setup (id);
            return this.indexOperation;
      }
   }

   public void onActivate ()
   {
      if (this.listOps == null) {
         this.listOps = new ArrayList<FOperationAbstract> ();
      }
      else {
         this.listOps.clear ();
      }

      List<OperationAbstract> tmplst = this.whoIam.getOperationsByAchievedNotNested (this.connect, this.termintedOrNot);
      for (int i = 0; i < tmplst.size (); i++) {
         this.listOps.add (FOpConverter.toFOperationAbstract (tmplst.get (i)));
      }

      if (this.listProgress == null) {
         this.listProgress = new ArrayList<ProgressData> ();
      }
      else {
         this.listProgress.clear ();
      }
      this.listProgress = this.whoIam.getProgressByFinished (this.connect, this.termintedOrNot);

   }

   /**
    * THIS SECTION FOR FILE EDITOR
    */
   /**
    * page of editor File
    */
   @Property
   private FileConfig fileC;

   /**
    * Do the Checkout of file.
    */
   public List<FileConfig> getFileConfigSMD () throws SVNException
   {
      SvnMangerConfigSMD svnStub = new SvnMangerConfigSMD (this.whoIam.getName ());
      svnStub.svnCheckout ();// */
      return svnStub.getFilesConfigs ();// */
   }

   /**
    * @param path of the file
    * @return
    */
   Object onActionFromEditFile (String path)
   {
      this.fileEditor.setup (this.whoIam, path);
      return this.fileEditor;
   }

   /**
    * THIS SECTION FOR THE ALL SMD to affect many of SMD with the same Operation
    */

   /**
    * @return PAGE ALL VALidator
    */
   Object onActionFromValider ()
   {
      this.validator.setup (this.whoIam);
      return this.validator;
   }

   @Property
   @Persist
   private boolean all;

   @InjectPage
   private AllValidatorIndex validator;

   /**
    * THIS SECTION is to get the OPERATION ACHIEVED OR NOT
    */
   /**
    * The zone of refresh
    */
   @Property
   @Inject
   private Block operationRefresh;

   @Property
   @Inject
   private Block progressRefresh;

   /**
    * to get Achieved = TRUE
    */
   Object onActionFromClickerTrue ()
   {
      this.termintedOrNot = true;
      onActivate ();

      Block returned;
      if (!this.progressORoperation) {
         returned = this.operationRefresh;
      }
      else {
         returned = this.progressRefresh;
      }
      return returned;
   }

   /**
    * to get Achieved = FALSE
    */
   Object onActionFromClickerFalse ()
   {
      this.listOps.clear ();
      this.termintedOrNot = false;

      onActivate ();

      Block returned;

      if (!this.progressORoperation) {
         returned = this.operationRefresh;
      }
      else {
         returned = this.progressRefresh;
      }
      return returned;
   }

   /**
    * PASSING BETWEEN PROGRESS TO OPERATION FALSE = OPERATION TRUE PROGRESS
    */
   @Property
   @Persist
   boolean progressORoperation;

   Object onActionFromChangeType ()
   {
      if (this.progressORoperation) {
         this.progressORoperation = false;
      }
      else {
         this.progressORoperation = true;
      }
      return this;
   }

   @Property
   @Persist
   private List<ProgressData> listProgress;

   @Property
   ProgressData prog;

   @Property
   @Persist
   boolean termintedOrNot;

   @Log
   public String getTitleAbr ()
   {
      String ret = this.prog.getTitle ().replaceAll ("\n", "<br/>");
      return ret;
   }

   public String getTitleProgress ()
   {
      /**
       * @todo qsdqsd
       */
      return "";
   }

   /**
    * Suppression et Terminaison de PROGRESS
    */
   @CommitAfter
   public Object onActionFromAchievedProgress (long id)
   {
      ProgressData progress_ = ProgressData.findById (this.connect, id);
      progress_.setFinished (true);
      progress_.setValue (100);

      this.connect.saveOrUpdate (progress_);

      this.whoIam.updateProgress (this.connect);

      return this;
   }

   @CommitAfter
   public Object onActionFromDeleteProgress (long id)
   {
      ProgressData progress_ = ProgressData.findById (this.connect, id);
      progress_.setSmd (null);

      this.connect.delete (progress_);
      return this;
   }

   /**
    * CLEANING
    */
   @InjectPage
   ProgressCleanCreate cleanprogress;

   @InjectPage
   OperationCleanCreate cleanoperation;

   public Object onActionFromCleanProgress ()
   {
      this.cleanprogress.setup (this.whoIam);
      return this.cleanprogress;
   }

   public Object onActionFromCleanOperation ()
   {
      this.cleanoperation.setup (this.whoIam);
      return this.cleanoperation;
   }

   /**
    * FOR DATE
    */
   public String getDateFormat ()
   {
      String res = this.prog.getLast_date ().toString ();
      return res;
   }
}
