package eu.imageau.monitor.pages.fileeditor;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Calendar;

import org.apache.commons.lang3.StringEscapeUtils;
import org.apache.tapestry5.annotations.InjectPage;
import org.apache.tapestry5.annotations.Persist;
import org.apache.tapestry5.annotations.Property;
import org.apache.tapestry5.hibernate.annotations.CommitAfter;
import org.apache.tapestry5.ioc.annotations.Inject;
import org.hibernate.Session;

import eu.imageau.monitorSMD.operationSMD.OperationSvnUpdate;
import eu.imageau.monitorSMD.operationSMD.SiteSMD;
import eu.imageau.monitorSMD.operationSMDEnum.SvnAcceptAction;
import eu.imageau.monitorSMD.svnmanager.SvnMangerConfigSMD;

import eu.imageau.monitor.pages.smd.SmdIndex;

public class FileConfigEditor
{
   @Inject
   Session connect;

   @Property
   @Persist
   private SiteSMD SMDCaller;

   @Property
   @Persist
   String edition;

   @Property
   @Persist
   private String path;

   @InjectPage
   SmdIndex indexSmd;

   @CommitAfter
   Object onValidateFromEditionForm ()
   {
      this.edition = this.edition.replace ("<br />", "");
      this.edition = this.edition.replace ("<p>", "");
      this.edition = this.edition.replace ("</p>", "");
      this.edition = StringEscapeUtils.unescapeHtml4 (this.edition);

      System.out.println (this.edition);
      try {
         FileWriter fw = new FileWriter (this.path);
         BufferedWriter bw = new BufferedWriter (fw);
         PrintWriter fichierSortie = new PrintWriter (bw);
         fichierSortie.println (this.edition);
         fichierSortie.close ();
      }
      catch (Exception e) {
         System.out.println (e.toString ());
      }

      SvnMangerConfigSMD svnStub = new SvnMangerConfigSMD (this.SMDCaller.getName ());
      svnStub.svnCommit ();

      /**
       * @todo Pour l'operation juste a pres le commit
       */

      OperationSvnUpdate svn = new OperationSvnUpdate ();
      svn.setDescription ("commited file" + this.path);
      svn.setDate_added (Calendar.getInstance ().getTime ());
      svn.setConflict (SvnAcceptAction.SvnAcceptActionTHEIRS);
      svn.setForced (true);
      svn.setHead ("HEAD");
      svn.setSmd (this.SMDCaller);

      File filee = new File (this.path);
      svn.setPath (filee.getParent ());

      svn.setUser ("auto commit genereted");
      this.connect.save (svn);

      this.indexSmd.setup (this.SMDCaller);
      return this.indexSmd;
   }

   public void setup (SiteSMD site, String path)
   {
      this.SMDCaller = site;
      this.path = path;
      this.edition = "";

      try {
         InputStream ips = new FileInputStream (path);
         InputStreamReader ipsr = new InputStreamReader (ips);
         BufferedReader br = new BufferedReader (ipsr);
         String ligne;
         while ((ligne = br.readLine ()) != null) {
            this.edition += ligne + "<br/>";
         }
         br.close ();
      }
      catch (Exception e) {
         System.out.println (e.toString ());
      }

   }

}
