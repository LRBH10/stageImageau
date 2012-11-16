package eu.imageau.monitorSMD.operationSMD;

import javax.persistence.DiscriminatorValue;
import javax.persistence.Entity;
import javax.persistence.Table;

import org.apache.tapestry5.beaneditor.Validate;
import org.apache.tapestry5.ioc.annotations.Inject;

import com.google.gson.annotations.Expose;

import eu.imageau.monitorSMD.operationSMDEnum.OperationType;

@Entity
@Table(name = "operation_script")
@DiscriminatorValue(OperationType.OperationTypeScript)
public class OperationScript extends OperationAbstract
{

   @Expose
   @Validate("required")
   private String command;

   @Expose
   private String parameters = "";

   @Inject
   public OperationScript ()
   {
      setType (OperationType.OperationScript);
   }

   public OperationScript (SiteSMD pere)
   {
      setType (OperationType.OperationScript);
      setSmd (pere);

   }

   public OperationScript (String cmd, String parms)
   {
      setType (OperationType.OperationScript);
      this.command = cmd;
      this.parameters = parms;
   }

   public OperationScript (String cmd, String parms, SiteSMD pere)
   {
      setType (OperationType.OperationScript);
      this.command = cmd;
      this.parameters = parms;
      setSmd (pere);
   }

   public String getCommand ()
   {
      return this.command;
   }

   public String getParameters ()
   {
      return this.parameters;
   }

   public void setCommand (String command)
   {
      this.command = command;
   }

   public void setParameters (String parameters)
   {
      this.parameters = parameters;
   }

   @Override
   public String toString ()
   {
      return super.toString () + "\n Commande : " + this.command + "\n Parametres : " + this.parameters;
   }

}
