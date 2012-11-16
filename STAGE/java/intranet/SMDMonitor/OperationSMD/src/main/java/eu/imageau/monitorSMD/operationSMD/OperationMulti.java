package eu.imageau.monitorSMD.operationSMD;

import java.util.LinkedList;
import java.util.List;

import javax.persistence.DiscriminatorValue;
import javax.persistence.Entity;
import javax.persistence.JoinColumn;
import javax.persistence.JoinTable;
import javax.persistence.OneToMany;
import javax.persistence.Table;

import org.apache.tapestry5.ioc.annotations.Inject;
import org.hibernate.Session;
import org.hibernate.annotations.Cascade;
import org.hibernate.annotations.CascadeType;

import com.google.gson.annotations.Expose;

import eu.imageau.monitorSMD.operationSMDEnum.OperationType;

@Entity
@Table(name = "operation_multi")
@DiscriminatorValue(OperationType.OperationTypeMulti)
public class OperationMulti extends OperationAbstract
{

   @Expose
   private boolean fail_on_error;

   @OneToMany
   @JoinTable(name = "operation_multi_operation", //
         joinColumns = @JoinColumn(name = "id_multi"), //
         inverseJoinColumns = @JoinColumn(name = "id_operation"))
   @Cascade({ CascadeType.ALL })
   @Expose
   private List<OperationAbstract> operations = new LinkedList<OperationAbstract> ();

   @Inject
   public OperationMulti ()
   {
      setType (OperationType.OperationMulti);
      this.fail_on_error = false;
   }

   public OperationMulti (SiteSMD pere)
   {
      setSmd (pere);
      this.fail_on_error = false;
      setType (OperationType.OperationMulti);
   }

   public void addOperationNested (OperationAbstract operation)
   {
      operation.setNested (true);
      operation.setSmd (getSmd ());
      this.operations.add (operation);
   }

   public List<OperationAbstract> getOperationsNested ()
   {
      return this.operations;
   }

   public void setOperationsNested (List<OperationAbstract> lst)
   {
      this.operations = lst;
   }

   public boolean isFail_on_error ()
   {
      return this.fail_on_error;
   }

   public void setFail_on_error (boolean linked)
   {
      this.fail_on_error = linked;
   }

   @Override
   public void setSmd (SiteSMD smd)
   {
      super.setSmd (smd);
      for (int i = 0; i < this.operations.size (); i++) {
         this.operations.get (i).setSmd (smd);
      }
   }

   @Override
   public void setAchieved (boolean isAchieved)
   {
      super.setAchieved (isAchieved);
      for (int i = 0; i < this.operations.size (); i++) {
         this.operations.get (i).setAchieved (isAchieved);
      }
   }

   @Override
   public String toString ()
   {
      return super.toString () + "\n Operation : " + this.operations + "\n";
   }

   @Override
   public OperationAbstract duplicateInBase (Session connect, SiteSMD newSmd)
   {
      List<OperationAbstract> inters = new LinkedList<OperationAbstract> ();
      for (OperationAbstract op : this.operations) {
         connect.evict (op);
         connect.evict (op.getReport ());

         op.setSmd (newSmd);
         inters.add (op);
      }

      connect.evict (this);
      connect.evict (getReport ());
      connect.evict (this.operations);

      setSmd (newSmd);
      this.operations = inters;

      connect.save (getReport ());
      connect.save (this);

      return this;
   }

   @Override
   public double getState ()
   {
      // TODO Auto-generated method stub
      double res = 0;
      for (int i = 0; i < this.operations.size (); i++) {
         res += this.operations.get (i).getState ();
      }

      if (this.operations.isEmpty ()) {
         res = 0;
      }
      else {
         res = res / this.operations.size ();
      }
      return res;
   }
}
