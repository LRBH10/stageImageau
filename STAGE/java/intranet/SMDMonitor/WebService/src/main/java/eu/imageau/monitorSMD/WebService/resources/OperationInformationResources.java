package eu.imageau.monitorSMD.WebService.resources;

import java.util.Calendar;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.ws.rs.FormParam;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

import org.hibernate.classic.Session;

import eu.imageau.monitorSMD.HibernateSMD.HibernateUtil;
import eu.imageau.monitorSMD.data.ProgressData;
import eu.imageau.monitorSMD.operationSMD.OperationAbstract;
import eu.imageau.monitorSMD.operationSMD.SiteSMD;


@Path("/informations/")
@Produces("application/json")
public class OperationInformationResources
{
   static Logger logger = Logger.getLogger (OperationInformationResources.class.getName ());

   /**
    * @param nameSMD : le Nom de SMD a mettre à jour
    * @return "SMD Not Found" : si le SMD n'existe pas "Connected OK" : sinon
    */
   @POST
   @Path("setConnected")
   public String setConnected (@FormParam("nameSMD") String nameSMD, @FormParam("what") boolean what)
   {
      OperationInformationResources.logger.log (Level.INFO, "Connect with (SMD = " + nameSMD + ", what = " + what + " )");

      Session connect = HibernateUtil.getSessionFactory ().openSession ();
      SiteSMD smd = SiteSMD.findByName (connect, nameSMD);
      String result = "SMD Not Found";

      if (smd != null) {
         smd.setConnected (what);

         connect.beginTransaction ();
         connect.saveOrUpdate (smd);
         connect.getTransaction ().commit ();

         result = "Connected OK";
      }
      connect.close ();

      OperationInformationResources.logger.log (Level.FINEST, "Returned Value " + result);
      return result;
   }

   /**
    * 
    * @param nameSMD : le Nom de SMD qui a envoyer la requete
    * @param newVersion : la nouvelle versions (le cas de MAJ)
    * @return "SMD Not Found" : si le SMD n'existe pas "Version Updated" : sinon
    */
   @POST
   @Path("setVersion")
   public String setVersion (@FormParam("nameSMD") String nameSMD, @FormParam("newVersion") String newVersion)
   {
      OperationInformationResources.logger.log (Level.INFO, "Version with (SMD = " + nameSMD + ", newVersion = " + newVersion + ")");

      Session connect = HibernateUtil.getSessionFactory ().openSession ();
      SiteSMD smd = SiteSMD.findByName (connect, nameSMD);
      String result = "SMD Not Found";

      if (smd != null) {
         String lst[] = newVersion.split ("-");
         if (lst.length == 2) {
            smd.setVersion_code (lst [0]);
            smd.setVersion_system (lst [1]);

            connect.beginTransaction ();
            connect.saveOrUpdate (smd);
            connect.getTransaction ().commit ();

            result = "Version Updated";
         }
         else {
            result = "Version Envoyer Incorrect";
         }
      }
      connect.close ();

      OperationInformationResources.logger.log (Level.FINEST, "Returned Value " + result);
      return result;
   }

   /**
    * 
    * @param idOperation : id de l'operations a mettre à jour
    * @param achievedStatus: true : pour un bon deroulement (succes). false : si c'est un échec.
    * @return "Operation With id= '@var idOperation' and Status ='@var achievedStatus' does not exist" : si l'operation n'existe pas dans la
    *         base "Status is updated" : Sinon.
    * 
    */
   @POST
   @Path("setAchieved")
   public String setAchieved (@FormParam("idOperation") long idOperation, @FormParam("achievedStatus") boolean achievedStatus,
         @FormParam("reportStatus") String reportStatus)
   {
      OperationInformationResources.logger.log (Level.INFO, "Achieved with (id = " + idOperation + ", etat = " + achievedStatus
            + ")+ resultat =\n " + reportStatus);

      String result = "Operation With id= '" + idOperation + "' and Status ='" + achievedStatus + "' does not exist";
      Session connect = HibernateUtil.getSessionFactory ().openSession ();

      OperationAbstract getOperation = OperationAbstract.findById (connect, idOperation);
      if (getOperation != null) {
         getOperation.setAchieved (achievedStatus);
         if (reportStatus != null) {
            getOperation.getReport ().setStatus (achievedStatus);
         }
         getOperation.getReport ().setContents (reportStatus);// */
         connect.beginTransaction ();
         connect.saveOrUpdate (getOperation);
         connect.getTransaction ().commit ();
         result = "Status is updated";
      }
      connect.close ();

      OperationInformationResources.logger.log (Level.FINEST, "Returned Value " + result);
      return result;
   }

   @POST
   @Path("progress")
   public String setProgress (@FormParam("idOperation") long idOperation, @FormParam("progressStatus") double progressStatus)
   {
      OperationInformationResources.logger.log (Level.INFO, "Progress with (id = " + idOperation + ", etat = " + progressStatus + ")");

      String result = "Operation With id= '" + idOperation + "' does not exist";
      Session connect = HibernateUtil.getSessionFactory ().openSession ();

      if (progressStatus < 0) {
         result = progressStatus + "is invalid (must be between 0..100)";
      }
      else {
         OperationAbstract getOperation = OperationAbstract.findById (connect, idOperation);
         if (getOperation != null) {
            getOperation.setState (progressStatus);

            connect.beginTransaction ();
            connect.saveOrUpdate (getOperation);
            connect.getTransaction ().commit ();

            result = "Status is updated";
         }
      }
      connect.close ();
      OperationInformationResources.logger.log (Level.FINEST, "Returned Value " + result);
      return result;
   }

   @POST
   @Path("progressSMD")
   public String setProgressSMD (@FormParam("nameSMD") String nameSMD, @FormParam("id_progress") long idProgress,
         @FormParam("title") String title, @FormParam("value") double valueProgress)
   {

      OperationInformationResources.logger.log (Level.INFO, "NAME: " + nameSMD + " Progress with (id = " + idProgress + ", etat = "
            + valueProgress + ")  \n contenu " + title);

      String result = "Progress with (id = " + idProgress + ", etat = " + valueProgress + ")";
      Session connect = HibernateUtil.getSessionFactory ().openSession ();

      if (idProgress < 0) {
         result += "   PROBLEM ID <0";
      }
      else {

         if (valueProgress < 0) {

            SiteSMD site = SiteSMD.findByName (connect, nameSMD);
            site.setHave_a_probleme (true);
            connect.beginTransaction ();
            connect.saveOrUpdate (site);
            connect.getTransaction ().commit ();

         }

         ProgressData getProgress = ProgressData.findById (connect, idProgress);
         if (getProgress != null) {

            // remplire DATA
            getProgress.setTitle (title);
            getProgress.setValue (valueProgress);
            getProgress.setLast_date (Calendar.getInstance ().getTime ());
            if (valueProgress == 100) {
               getProgress.setFinished (true);
            }

            connect.beginTransaction ();
            connect.saveOrUpdate (getProgress);
            connect.getTransaction ().commit ();

            result = "Status is updated";
         }
         else {
            // remplire DATA
            SiteSMD site = SiteSMD.findByName (connect, nameSMD);
            if (site == null) {
               result = "SMD Not Found";
            }
            else {
               getProgress = new ProgressData ();
               getProgress.setId (idProgress);
               getProgress.setTitle (title);
               getProgress.setValue (valueProgress);
               getProgress.setLast_date (Calendar.getInstance ().getTime ());
               if (valueProgress == 100) {
                  getProgress.setFinished (true);
               }
               getProgress.setSmd (site);

               connect.beginTransaction ();
               connect.save (getProgress);
               connect.getTransaction ().commit ();
               result = "Status is updated";
            }
         }
      }
      connect.close ();
      OperationInformationResources.logger.log (Level.FINEST, "Returned Value " + result);
      return result;
   }

}
