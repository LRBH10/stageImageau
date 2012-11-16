package eu.imageau.monitorSMD.operationSMD;

import javax.persistence.DiscriminatorValue;
import javax.persistence.Entity;
import javax.persistence.Table;

import org.apache.tapestry5.ioc.annotations.Inject;

import eu.imageau.monitorSMD.operationSMDEnum.OperationType;

@Entity
@Table(name = "operation_subversion_update")
@DiscriminatorValue(OperationType.OperationTypeSvnUpdate)
public class OperationSvnUpdate extends OperationSvn
{
   @Inject
   public OperationSvnUpdate ()
   {
      super ();
      setSvn_type (OperationType.OperationSvnUpdate);
   }
}
