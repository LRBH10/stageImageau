package eu.imageau.monitor.pages.smd.operationfactory;

import org.apache.tapestry5.ComponentResources;
import org.apache.tapestry5.annotations.InjectPage;
import org.apache.tapestry5.annotations.Log;
import org.apache.tapestry5.annotations.Persist;
import org.apache.tapestry5.annotations.Property;
import org.apache.tapestry5.beaneditor.BeanModel;
import org.apache.tapestry5.hibernate.annotations.CommitAfter;
import org.apache.tapestry5.ioc.annotations.Inject;
import org.apache.tapestry5.services.BeanModelSource;
import org.hibernate.Session;

import eu.imageau.monitorSMD.operationSMD.OperationAbstract;
import eu.imageau.monitorSMD.operationSMD.OperationMulti;
import eu.imageau.monitorSMD.operationSMD.OperationScript;
import eu.imageau.monitorSMD.operationSMD.OperationShellFile;
import eu.imageau.monitorSMD.operationSMD.OperationSvn;
import eu.imageau.monitorSMD.operationSMD.OperationSvnSwitch;
import eu.imageau.monitorSMD.operationSMD.OperationSvnUpdate;
import eu.imageau.monitorSMD.operationSMD.SiteSMD;
import eu.imageau.monitorSMD.operationSMDEnum.OperationType;

import eu.imageau.monitor.pages.smd.SmdIndex;
import eu.imageau.monitor.pages.smd.multi.MultiIndex;

@SuppressWarnings("unused")
public class OperationFactoryUpdate
{

   @Inject
   protected BeanModelSource beanModelSource;

   @Inject
   protected ComponentResources resources;

   @Persist
   OperationType type;
   /**
    * Attribute
    */
   @Inject
   Session connect;

   @Property
   @Persist
   private SiteSMD SMDCaller;

   /**
    * Operation
    */
   @Property
   @Persist
   OperationAbstract operation;

   /**
    * Titre
    */
   @Property
   @Persist
   private String titre;

   /**
    * automatic Form generator
    * 
    * @return
    */
   @SuppressWarnings("null")
   @Log
   public BeanModel<? extends OperationAbstract> getDataModel ()
   {
      BeanModel<? extends OperationAbstract> inter = null;

      switch (this.type) {
         case OperationScript:
            inter = this.beanModelSource.createEditModel (OperationScript.class, this.resources.getMessages ());
            inter.reorder ("description", "user", "path", "command", "parameters", "report_on_success", "state", "achieved");

            break;

         case OperationMulti:
            inter = this.beanModelSource.createEditModel (OperationMulti.class, this.resources.getMessages ());
            inter.reorder ("description", "user", "path", "report_on_success", "fail_on_error", "state", "achieved");

            break;

         case OperationShellFile:
            inter = this.beanModelSource.createEditModel (OperationShellFile.class, this.resources.getMessages ());
            inter.get ("contents").dataType ("longtext");
            inter.reorder ("description", "user", "path", "action", "filename", "contents", "report_on_success", "execute_after",
                  "delete_after", "state", "achieved");

            break;

         case OperationSvnUpdate:
            inter = this.beanModelSource.createEditModel (OperationSvnUpdate.class, this.resources.getMessages ());
            inter.reorder ("description", "user", "path", "head", "conflict", "report_on_success", "forced", "state", "achieved");
            inter.exclude ("svn_type");
            break;

         case OperationSvnSwitch:
            inter = this.beanModelSource.createEditModel (OperationSvnSwitch.class, this.resources.getMessages ());
            inter.reorder ("description", "user", "path", "head", "conflict", "url_switch", "report_on_success", "forced", "state",
                  "achieved");
            inter.exclude ("svn_type");
            break;

         default:
            break;
      }
      inter.exclude ("type", "id", "nested", "date_added");
      return inter;
   }

   /**
    * for the nested Call
    */
   @Persist
   private long idfather;

   /**
    * Pages
    */
   @InjectPage
   private SmdIndex indexSMD;

   @InjectPage
   private MultiIndex indexMulti;

   void onActivate ()
   {
      // operation = new OperationTemplate<OperationScript> ();
   }

   /**
    * setup
    * 
    * @param site le nom de site pere
    * @param nes is nested(true) or Not(false)
    */
   public void setup (SiteSMD site, long idfather, long id)
   {
      this.operation = OperationAbstract.findById (this.connect, id);
      this.SMDCaller = site;
      this.idfather = idfather;
      this.type = getTypeFromOperation ();
   }

   /**
    * setup : For not nested Operations
    * 
    * @param site le nom de site pere
    */
   public void setup (SiteSMD site, long id)
   {
      this.operation = OperationAbstract.findById (this.connect, id);
      this.SMDCaller = site;
      this.idfather = -1;
      this.type = getTypeFromOperation ();
      this.titre = this.type.getValue ();
   }

   /**
    * if success
    * 
    * @return IndexSMD if not nested, MultiIndex else
    */
   @CommitAfter
   Object onSuccess ()
   {
      Object out;
      if (this.idfather > 0) {
         this.connect.saveOrUpdate (this.operation);
         this.indexMulti.setup (this.SMDCaller, this.idfather);
         out = this.indexMulti;
      }
      else {
         this.operation.setSmd (this.SMDCaller);
         this.connect.saveOrUpdate (this.operation);
         this.indexSMD.setup (this.SMDCaller);
         out = this.indexSMD;
      }
      return out;
   }

   OperationType getTypeFromOperation ()
   {
      OperationType res = OperationType.OperationAbstract;
      if (this.operation.getType () == OperationType.OperationSvn) {
         OperationSvn opsvn = (OperationSvn) this.operation;
         res = opsvn.getSvn_type ();
      }
      else {
         res = this.operation.getType ();
      }
      return res;

   }
}
