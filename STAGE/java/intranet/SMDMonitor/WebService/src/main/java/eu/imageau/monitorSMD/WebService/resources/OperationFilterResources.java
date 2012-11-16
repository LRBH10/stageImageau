package eu.imageau.monitorSMD.WebService.resources;

import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;

import javax.ws.rs.FormParam;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

import org.hibernate.classic.Session;

import eu.imageau.monitorSMD.HibernateSMD.HibernateUtil;
import eu.imageau.monitorSMD.operationSMD.OperationAbstract;
import eu.imageau.monitorSMD.operationSMD.SiteSMD;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;


@Path("/operations/")
@Produces("application/json")
public class OperationFilterResources
{
   /**
    * 
    * @param nameSMD: le nom de SMD, source de la requete.
    * @return JSON de toutes les operations.
    */
   @POST
   @Path("getOperations")
   public String getOperations (@FormParam("nameSMD") String nameSMD)
   {
      OperationInformationResources.logger.log (Level.INFO, "getOperations with (nameSMD = " + nameSMD + ")");
      Session connect = HibernateUtil.getSessionFactory ().openSession ();
      SiteSMD smd = SiteSMD.findByName (connect, nameSMD);
      String result = "SMD Not Found";
      if (smd != null) {

         Gson gson = new GsonBuilder ().disableHtmlEscaping ().excludeFieldsWithoutExposeAnnotation ().setPrettyPrinting ()
               .serializeNulls ().create ();

         List<OperationAbstract> lst = new ArrayList<OperationAbstract> ();
         lst = smd.getOperationsByAchievedNotNested (connect, false);
         result = gson.toJson (lst);
      }
      connect.close ();
      OperationInformationResources.logger.log (Level.FINEST, "Returned Value " + "{\"operations\": " + result + "}");
      return "{\"operations\": " + result + "}";
   }

   /**
    * @param idOperation : Id d'operation
    * @param type : le type d'opeartion
    * @return JSON correspondent a cette Operation si elle existe Erreur Sinon.
    */
   @SuppressWarnings("null")
   @POST
   @Path("getOperationById")
   public String getOperationById (@FormParam("idOperation") long idOperation, @FormParam("type") String type)
   {
      OperationInformationResources.logger.log (Level.INFO, "Progress with (id = " + idOperation + ", type = " + type + ")");

      String result = "Operation With id= '" + idOperation + "' and type ='" + type + "' does not exist";
      Session connect = HibernateUtil.getSessionFactory ().openSession ();

      OperationAbstract getOperation = OperationAbstract.findById (connect, idOperation);
      if (getOperation != null) {
         Gson gson = new GsonBuilder ().disableHtmlEscaping ().excludeFieldsWithoutExposeAnnotation ().setPrettyPrinting ()
               .serializeNulls ().create ();
         result = gson.toJson (getOperation);
      }
      OperationInformationResources.logger.log (Level.FINEST, getOperation.toString ());

      connect.close ();
      OperationInformationResources.logger.log (Level.FINEST, "Returned Value " + result);
      return result;
   }
}
