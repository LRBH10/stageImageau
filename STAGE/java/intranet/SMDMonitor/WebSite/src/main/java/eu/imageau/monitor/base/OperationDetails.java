package eu.imageau.monitor.base;

import java.util.LinkedList;
import java.util.List;

import org.apache.tapestry5.annotations.InjectPage;
import org.apache.tapestry5.annotations.Persist;
import org.apache.tapestry5.annotations.Property;
import org.apache.tapestry5.ioc.annotations.Inject;
import org.hibernate.Session;

import eu.imageau.monitor.OperationsEntities.FOperationAbstract;
import eu.imageau.monitor.pages.smd.operationfactory.OperationFactoryCreate;
import eu.imageau.monitor.pages.smd.operationfactory.OperationFactoryIndex;
import eu.imageau.monitor.pages.smd.operationfactory.OperationFactoryUpdate;
import eu.imageau.monitorSMD.operationSMDEnum.OperationType;

public abstract class OperationDetails
{
   /**
    * Session of Hibernate
    */
   @Inject
   protected Session connect;

   /**
    * For the containers Operation
    */
   @Property
   protected FOperationAbstract operation;

   /**
    * Operation Type
    */
   @Property
   protected OperationType op;

   /**
    * new Operation
    */
   @InjectPage
   protected OperationFactoryCreate newOperation;

   /**
    * index Operation
    */
   @InjectPage
   protected OperationFactoryIndex indexOperation;

   /**
    * Update Operation
    */
   @InjectPage
   protected OperationFactoryUpdate updateOperation;

   /**
    * Grid of operation Source
    */
   @Property
   @Persist
   protected List<FOperationAbstract> listOps;

   /**
    * Zone REfresh
    */

   /**
    * get all OperationType Strings
    * 
    * @return OperationTypes
    */
   public List<OperationType> getOpsTypes ()
   {
      OperationType ss = OperationType.OperationAbstract;

      List<OperationType> result = new LinkedList<OperationType> ();
      for (int i = 0; i < ss.getDeclaringClass ().getEnumConstants ().length; i++) {
         result.add (ss.getDeclaringClass ().getEnumConstants () [i]);
      }
      result.remove (OperationType.OperationAbstract);
      result.remove (OperationType.OperationSvn);

      return result;
   }

   /**
    * Abstract Function
    */
   public abstract Object onActionFromSelect (long id);

}
