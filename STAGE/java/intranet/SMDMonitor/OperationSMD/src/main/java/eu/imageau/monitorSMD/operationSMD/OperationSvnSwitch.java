package eu.imageau.monitorSMD.operationSMD;

import javax.persistence.DiscriminatorValue;
import javax.persistence.Entity;
import javax.persistence.Table;

import org.apache.tapestry5.beaneditor.Validate;
import org.apache.tapestry5.ioc.annotations.Inject;

import com.google.gson.annotations.Expose;

import eu.imageau.monitorSMD.operationSMDEnum.OperationType;

@Entity
@Table(name = "operation_subversion_switch")
@DiscriminatorValue(OperationType.OperationTypeSvnSwitch)
public class OperationSvnSwitch extends OperationSvn
{
   @Expose
   @Validate("required")
   String url_switch;

   @Inject
   public OperationSvnSwitch ()
   {
      // TODO Auto-generated constructor stub
      super ();
      setSvn_type (OperationType.OperationSvnSwitch);
   }

   public String getUrl_switch ()
   {
      return this.url_switch;
   }

   public void setUrl_switch (String url_switch)
   {
      this.url_switch = url_switch;
   }

}
