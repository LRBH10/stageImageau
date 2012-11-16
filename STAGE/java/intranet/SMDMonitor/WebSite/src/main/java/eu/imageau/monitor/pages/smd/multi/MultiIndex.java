package eu.imageau.monitor.pages.smd.multi;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

import org.apache.tapestry5.annotations.InjectPage;
import org.apache.tapestry5.annotations.Log;
import org.apache.tapestry5.annotations.Persist;
import org.apache.tapestry5.annotations.Property;
import org.apache.tapestry5.hibernate.annotations.CommitAfter;

import eu.imageau.monitor.OperationsEntities.FOperationAbstract;
import eu.imageau.monitor.base.FOpConverter;
import eu.imageau.monitor.base.OperationDetails;
import eu.imageau.monitor.pages.smd.SmdIndex;
import eu.imageau.monitorSMD.operationSMD.OperationAbstract;
import eu.imageau.monitorSMD.operationSMD.OperationMulti;
import eu.imageau.monitorSMD.operationSMD.SiteSMD;
import eu.imageau.monitorSMD.operationSMDEnum.OperationType;

@SuppressWarnings("unused")
public class MultiIndex extends OperationDetails
{

   @Property
   @Persist
   private SiteSMD SMDCaller;

   @Property
   @Persist
   private OperationMulti whoIam;

   @Property
   @Persist
   private String report;

   /**
    * Pages
    */
   @InjectPage
   private SmdIndex indexSmd;

   /**
    * Initialization
    * 
    * @param site is Site Caller
    * @param id est le id de l'operations
    */
   public void setup (SiteSMD site, long id)
   {
      this.whoIam = (OperationMulti) OperationAbstract.findById (this.connect, id);
      this.report = this.whoIam.getReport ().getContents ().replaceAll ("\n", "<br/>");
      this.SMDCaller = site;
   }

   Object onActionFromGotoSMD ()
   {
      this.indexSmd.setup (this.SMDCaller);
      return this.indexSmd;
   }

   public void onActivate ()
   {
      List<FOperationAbstract> res = new LinkedList<FOperationAbstract> ();
      this.whoIam = (OperationMulti) OperationAbstract.findById (this.connect, this.whoIam.getId ());
      for (int i = 0; i < this.whoIam.getOperationsNested ().size (); i++) {
         res.add (FOpConverter.toFOperationAbstract (this.whoIam.getOperationsNested ().get (i)));
      }
      this.listOps = res;
      this.report = this.whoIam.getReport ().getContents ().replaceAll ("\n", "<br/>");
   }

   /**
    * Functions
    */

   /***
    * Multiple Create
    */
   @Log
   Object onActionFromOperationFactoryCreate (OperationType type)
   {
      switch (type) {
         default:
            this.newOperation.setup (this.SMDCaller, this.whoIam.getId (), type);
            return this.newOperation;
      }

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
      this.whoIam = (OperationMulti) OperationAbstract.findById (this.connect, this.whoIam.getId ());
      List<OperationAbstract> lst = new ArrayList<OperationAbstract> ();
      for (int i = 0; i < this.whoIam.getOperationsNested ().size (); i++) {
         if (this.whoIam.getOperationsNested ().get (i).getId () != id) {
            lst.add (this.whoIam.getOperationsNested ().get (i));
         }
      }

      this.whoIam.setOperationsNested (lst);

      this.connect.saveOrUpdate (this.whoIam);

      return this;
   }

   Object onActionFromUpdate (long id)
   {
      OperationAbstract res = OperationAbstract.findById (this.connect, id);
      this.updateOperation.setup (this.SMDCaller, this.whoIam.getId (), id);
      return this.updateOperation;
   }

   /***
    * Select Operations
    */
   @Override
   public Object onActionFromSelect (long id)
   {
      OperationAbstract res = OperationAbstract.findById (this.connect, id);
      this.indexOperation.setup (id);
      return this.indexOperation;

   }

   @Override
   public List<OperationType> getOpsTypes ()
   {
      OperationType ss = OperationType.OperationAbstract;

      List<OperationType> result = new LinkedList<OperationType> ();
      for (int i = 0; i < ss.getDeclaringClass ().getEnumConstants ().length; i++) {
         result.add (ss.getDeclaringClass ().getEnumConstants () [i]);
      }

      result.remove (OperationType.OperationAbstract);
      result.remove (OperationType.OperationSvn);
      result.remove (OperationType.OperationMulti);

      return result;
   }

}
