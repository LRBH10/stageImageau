package eu.imageau.monitor.base;

import eu.imageau.monitorSMD.operationSMD.OperationAbstract;
import eu.imageau.monitorSMD.operationSMD.OperationSvn;
import eu.imageau.monitorSMD.operationSMDEnum.OperationType;
import eu.imageau.monitor.OperationsEntities.FOperationAbstract;

public class FOpConverter
{

   public static FOperationAbstract toFOperationAbstract (OperationAbstract op)
   {
      FOperationAbstract res = new FOperationAbstract ();
      res.id = op.getId ();
      res.achieved = op.isAchieved ();
      res.nested = op.isNested ();
      res.path = op.getPath ();
      res.state = op.getState ();
      res.type = op.getType ();
      if (res.type == OperationType.OperationSvn) {
         OperationSvn svn = (OperationSvn) op;
         res.type = svn.getSvn_type ();
      }
      res.user = op.getUser ();
      res.description = op.getDescription ();
      res.dateAdded = op.getDate_added ();

      return res;
   }
}
