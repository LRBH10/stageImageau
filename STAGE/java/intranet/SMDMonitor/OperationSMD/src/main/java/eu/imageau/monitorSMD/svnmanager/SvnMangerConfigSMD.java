package eu.imageau.monitorSMD.svnmanager;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.tmatesoft.svn.core.SVNDepth;
import org.tmatesoft.svn.core.SVNException;
import org.tmatesoft.svn.core.SVNURL;
import org.tmatesoft.svn.core.auth.ISVNAuthenticationManager;
import org.tmatesoft.svn.core.io.SVNRepository;
import org.tmatesoft.svn.core.io.SVNRepositoryFactory;
import org.tmatesoft.svn.core.wc.SVNClientManager;
import org.tmatesoft.svn.core.wc.SVNCommitClient;
import org.tmatesoft.svn.core.wc.SVNRevision;
import org.tmatesoft.svn.core.wc.SVNUpdateClient;
import org.tmatesoft.svn.core.wc.SVNWCUtil;

public class SvnMangerConfigSMD
{
   String url = "svn://rli@hal-vm-scm/prog_station/linux/branch/stage_rli/dist/conf/";
   String nameSMD = "testSMD";
   String name = "rli";
   String password = "rli";

   File Smds = null;

   
   SVNRepository repository = null;

   public SvnMangerConfigSMD (String name)
   {
      // TODO Auto-generated constructor stub
      this.nameSMD = name;
      String tmp = System.getProperty ("java.io.tmpdir");

      // System.out.println (tmp + "/SMDs/" + this.nameSMD);

      this.Smds = new File (tmp + "/SMDs/" + this.nameSMD);

      initRepositorySvn ();
   }

   /**
    * initalise Reporesotory
    */
   void initRepositorySvn ()
   {
      try {
         this.repository = SVNRepositoryFactory.create (SVNURL.parseURIDecoded (this.url));
         ISVNAuthenticationManager authManager = SVNWCUtil.createDefaultAuthenticationManager (this.name, this.password);
         this.repository.setAuthenticationManager (authManager);
      }
      catch (SVNException e) {
         e.printStackTrace ();
      }

   }

   /**
    * CHECKOUT
    */
   public void svnCheckout ()
   {
      SVNClientManager ourClientManager = SVNClientManager.newInstance (null, this.repository.getAuthenticationManager ());

      SVNURL urlsvn = null;
      try {
         urlsvn = SVNURL.parseURIEncoded (this.url + this.nameSMD);
      }
      catch (SVNException e) {
         // TODO Auto-generated catch block
         e.printStackTrace ();
      }

      SVNUpdateClient updateClient = ourClientManager.getUpdateClient ();
      updateClient.setIgnoreExternals (false);

      try {
         updateClient.doCheckout (urlsvn, this.Smds, SVNRevision.UNDEFINED, SVNRevision.HEAD, SVNDepth.FILES, true);
      }
      catch (SVNException e) { // TODO Auto-generated catch block
         e.printStackTrace ();
      }
   }

   /**
    * REcuperer les noms des fichiers
    */
   public List<String> getFilesDirectory ()
   {
      List<String> lst = new ArrayList<String> ();
      for (int i = 0; i < this.Smds.list ().length; i++) {
         lst.add (this.Smds.list () [i]);
      }

      return lst;
   }

   /**
    * REcuperer Config File
    */
   public List<FileConfig> getFilesConfigs ()
   {
      List<FileConfig> lst = new ArrayList<FileConfig> ();
      for (File f : this.Smds.listFiles ()) {
         if (f.isFile () && !f.isHidden ()) {
            lst.add (new FileConfig (f.getName (), f.getAbsolutePath ()));
         }
      }

      return lst;
   }

   /**
    * Commit
    */
   public void svnCommit ()
   {
      SVNClientManager ourClientManager = SVNClientManager.newInstance (null, this.repository.getAuthenticationManager ());

      SVNCommitClient commitClient = ourClientManager.getCommitClient ();

      File lst[] = new File[1];
      lst [0] = this.Smds;

      try {
         commitClient.doCommit (lst, true, "TEST SVN JAVA", null, null, false, false, SVNDepth.FILES);
      }
      catch (SVNException e) {
         // TODO Auto-generated catch block
         e.printStackTrace ();
      }

   }

}
