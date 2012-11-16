package eu.imageau.monitorSMD.operationSMD;

import javax.persistence.DiscriminatorColumn;
import javax.persistence.DiscriminatorValue;
import javax.persistence.Entity;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.Table;

import org.apache.tapestry5.beaneditor.NonVisual;
import org.apache.tapestry5.beaneditor.Validate;

import com.google.gson.annotations.Expose;

import eu.imageau.monitorSMD.operationSMDEnum.OperationType;
import eu.imageau.monitorSMD.operationSMDEnum.SvnAcceptAction;

@Entity
@Table(name = "operation_subversion")
@DiscriminatorValue(OperationType.OperationTypeSvn)
@DiscriminatorColumn(name = "svn_type")
public class OperationSvn extends OperationAbstract
{
   @NonVisual
   @Enumerated(EnumType.STRING)
   @Expose
   OperationType svn_type;

   @Expose
   @Validate("required")
   private String head = "HEAD";

   @Validate("required")
   @Enumerated(EnumType.STRING)
   @Expose
   private SvnAcceptAction conflict = SvnAcceptAction.SvnAcceptActionNOTHING;

   @Expose
   private boolean forced = false;

   public OperationSvn ()
   {

      setType (OperationType.OperationSvn);
   }

   public void setSvn_type (OperationType svn_type)
   {
      this.svn_type = svn_type;
   }

   public OperationType getSvn_type ()
   {
      return this.svn_type;
   }

   public String getHead ()
   {
      return this.head;
   }

   public void setHead (String head)
   {
      this.head = head;
   }

   public SvnAcceptAction getConflict ()
   {
      return this.conflict;
   }

   public void setConflict (SvnAcceptAction conflit)
   {
      this.conflict = conflit;
   }

   public boolean isForced ()
   {
      return this.forced;
   }

   public void setForced (boolean forced)
   {
      this.forced = forced;
   }

   @Override
   public String toString ()
   {

      return super.toString () + "\n TypeSvn : " + this.svn_type + "\n HEAD : " + this.head + "\n Conflict : " + this.conflict
            + "\n Forced : " + this.forced;
   }
}
