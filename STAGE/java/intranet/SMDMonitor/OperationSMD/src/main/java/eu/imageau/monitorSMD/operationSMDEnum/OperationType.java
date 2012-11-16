package eu.imageau.monitorSMD.operationSMDEnum;

public enum OperationType {
   OperationAbstract, OperationMulti, OperationScript, OperationShellFile, OperationSvn, OperationSvnUpdate, OperationSvnSwitch, ;

   public static final String OperationTypeAbstract = "OperationAbstract";
   public static final String OperationTypeMulti = "OperationMulti";
   public static final String OperationTypeScript = "OperationScript";
   public static final String OperationTypeShellFile = "OperationShellFile";
   public static final String OperationTypeSvn = "OperationSvn";
   public static final String OperationTypeSvnUpdate = "OperationSvnUpdate";
   public static final String OperationTypeSvnSwitch = "OperationSvnSwitch";

   public String getValue ()
   {
      return name ();
   }
}
