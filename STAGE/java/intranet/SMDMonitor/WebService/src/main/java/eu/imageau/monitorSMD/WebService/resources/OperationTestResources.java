package eu.imageau.monitorSMD.WebService.resources;

import javax.ws.rs.FormParam;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

import org.hibernate.Session;

import eu.imageau.monitorSMD.HibernateSMD.HibernateUtil;
import eu.imageau.monitorSMD.data.ReportData;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;


@Path("/tests/")
@Produces("application/json")
public class OperationTestResources
{

   @GET
   @Path("testGet")
   public String testGET ()
   {
      String str = " ¹ # { [ | ` \\ ^ @]}øỗœîûÿþê€âæäßë‘’ðüïŀö÷×¿¬↓ ©»«≤³1234567890°+£µM§/.?,;:!ù$=)àç_è-('\"é&²";
      ReportData ss = new ReportData ();
      ss.setContents (str);
      ss.setStatus (false);

      Session connect = HibernateUtil.getSessionFactory ().openSession ();
      connect.beginTransaction ();
      connect.save (ss);
      connect.getTransaction ().commit ();

      ReportData r = ReportData.findById (connect, 20);
      connect.close ();
      if (r == null) {
         return "NO OBJECT\n";
      }

      Gson gson2 = new GsonBuilder ().disableHtmlEscaping ().create ();

      String res = "";
      res += "\n   " + gson2.toJson (r.getContents ());

      return res + "\n";
   }

   @POST
   @Path("testPost")
   public String testPost (@FormParam("name") String name)
   {
      System.out.println ("Beta");
      return "nameSMD : " + name;
   }

}
