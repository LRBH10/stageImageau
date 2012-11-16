package eu.imageau.monitorSMD.operationSMD;

import javax.persistence.DiscriminatorValue;
import javax.persistence.Entity;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.Table;

import org.apache.tapestry5.beaneditor.Validate;
import org.apache.tapestry5.ioc.annotations.Inject;

import com.google.gson.annotations.Expose;

import eu.imageau.monitorSMD.operationSMDEnum.OperationType;
import eu.imageau.monitorSMD.operationSMDEnum.ShellFileAction;

@Entity
@Table(name = "operation_shell_file")
@DiscriminatorValue(OperationType.OperationTypeShellFile)
public class OperationShellFile extends OperationAbstract
{
   @Enumerated(EnumType.STRING)
   @Expose
   private ShellFileAction action = ShellFileAction.ShellFileActionCopy;

   @Expose
   @Validate("required,minlength=3")
   private String filename;

   @Expose
   private String contents = "#!/bin/bash";

   @Expose
   private boolean execute_after = true;

   @Expose
   private boolean delete_after = false;

   @Inject
   public OperationShellFile ()
   {
      setType (OperationType.OperationShellFile);
   }

   public OperationShellFile (SiteSMD pere)
   {
      setType (OperationType.OperationShellFile);
      setSmd (pere);
   }

   public OperationShellFile (ShellFileAction action, String filename, boolean execute_after)
   {
      setType (OperationType.OperationShellFile);
      this.action = action;
      this.filename = filename;
      this.execute_after = execute_after;
   }

   public OperationShellFile (ShellFileAction action, String filename, boolean execute_after, SiteSMD smd)
   {
      setType (OperationType.OperationShellFile);
      this.action = action;
      this.filename = filename;
      this.execute_after = execute_after;
      setSmd (smd);
   }

   public boolean isExecute_after ()
   {
      return this.execute_after;
   }

   public void setExecute_after (boolean execute_after)
   {
      this.execute_after = execute_after;
   }

   public ShellFileAction getAction ()
   {
      return this.action;
   }

   public void setAction (ShellFileAction action)
   {
      this.action = action;
   }

   public String getFilename ()
   {
      return this.filename;
   }

   public void setFilename (String filename)
   {
      this.filename = filename;
   }

   public void setContents (String contents)
   {
      this.contents = contents;
   }

   public String getContents ()
   {
      return this.contents;
   }

   public boolean isDelete_after ()
   {
      return this.delete_after;
   }

   public void setDelete_after (boolean delete_after)
   {
      this.delete_after = delete_after;
   }

   @Override
   public String toString ()
   {
      // TODO Auto-generated method stub
      return super.toString () + "\n action : " + this.action + "\n filename : " + this.filename + "\n Contenu : " + this.contents
            + "\n Execute After : " + this.execute_after;
   }
}
