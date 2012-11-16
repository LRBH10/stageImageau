/* $Id: LogFactory.java 140 2011-09-21 08:57:00Z bdm $ */
package eu.imageau.common;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.Hashtable;
import java.util.Properties;
import java.util.Set;
import java.util.logging.ConsoleHandler;
import java.util.logging.Formatter;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.LogManager;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;

/**
 * @author bdm
 * 
 */
public class LogFactory
{
   private static Hashtable<String, Logger> loggerMap;
   private static Logger rootLogger;

   static {
      // ************** UPPER STAGE INIT *****************
      // ************** todo: MUST be elsewhere **********

      System.setProperty ("java.awt.headless", "true"); // to avoid X11 forbidden accesses

      // ************** LOGGER INIT **********************
      String logConfFileName = "logging.properties";
      URL u = ClassLoader.getSystemResource (logConfFileName);
      InputStream conf = ClassLoader.getSystemResourceAsStream (logConfFileName);

      if (conf == null) {
         System.out.println ("LogFactory: Default class loader failed !");
         u = LogFactory.class.getResource (logConfFileName);
         conf = LogFactory.class.getResourceAsStream (logConfFileName);
      }

      if (conf == null) {
         System.out.println ("LogFactory: LogFactory class loader failed !");
         u = ClassLoader.getSystemResource ("/" + logConfFileName);
         conf = ClassLoader.getSystemResourceAsStream ("/" + logConfFileName);
      }

      if (conf == null) {
         System.out.println ("LogFactory: LogFactory class loader failed (+ /) !");
         u = LogFactory.class.getResource ("/" + logConfFileName);
         conf = LogFactory.class.getResourceAsStream ("/" + logConfFileName);
      }

      if (conf == null) {
         System.err.println ("LogFactory: Could not find '" + logConfFileName + "' in classpath !");
         System.err.println ("LogFactory: \tJava classpath:");
         System.err.println ("LogFactory: " + System.getProperty ("java.class.path"));
         System.err.println ("LogFactory: \tJava libpath:");
         System.err.println ("LogFactory: " + System.getProperty ("java.library.path"));
      }
      else {
         System.out.println ("LogFactory: Success ! Will use logging file: " + u);
         try {
            Properties prop = new Properties ();
            prop.load (conf);

            LogFactory.loggerMap = new Hashtable<String, Logger> ();
            LogFactory.rootLogger = Logger.getLogger (Logger.GLOBAL_LOGGER_NAME);

            Set<String> props = prop.stringPropertyNames ();

            String levelSuffixe = ".level";
            String formatterSuffixe = ".formatter";

            String handlerNames = prop.getProperty ("handlers");
            if (handlerNames == null || handlerNames.isEmpty ()) {
               // if no handlers defines, we create a default one
               Handler console = new ConsoleHandler ();
               console.setLevel (Level.FINE); // default level for all
               console.setFormatter (new SimpleFormatter ());
               LogFactory.rootLogger.addHandler (console);
            }
            else {
               for (String handName : handlerNames.split (",")) {
                  try {
                     Handler hand = null;

                     // check if it already exists
                     boolean handlerExist = false;
                     for (Handler h : LogFactory.rootLogger.getHandlers ()) {
                        if (h.getFormatter () != null && h.getFormatter ().getClass ().getName ().equals (handName)) {
                           hand = h;
                           handlerExist = true;
                           break;
                        }
                     }

                     if (!handlerExist) { // if not exists, try to create it
                        hand = (Handler) Class.forName (handName).newInstance ();
                     }

                     if (hand != null) {
                        String handlerLevel = prop.getProperty (handName + levelSuffixe);
                        if (handlerLevel != null && !handlerLevel.isEmpty ()) {
                           hand.setLevel (Level.parse (handlerLevel));
                        }

                        String handlerFormatter = prop.getProperty (handName + formatterSuffixe);
                        if (handlerFormatter != null && !handlerFormatter.isEmpty ()) {
                           Formatter fmt = (Formatter) Class.forName (handlerFormatter).newInstance ();
                           hand.setFormatter (fmt);
                        }

                        LogFactory.rootLogger.addHandler (hand);
                     }
                  }
                  catch (Exception e) {
                     System.err.println ("LogFactory: catch an exception during init ! See below:");
                     e.printStackTrace ();
                  }
               }
            }

            Logger logger = null;
            for (String key : props) {
               if (key.endsWith (levelSuffixe)) {
                  if (!key.contains ("handler") && !key.contains ("Handler")) {
                     String loggerName = key.substring (0, key.length () - levelSuffixe.length ());
                     if (loggerName == null || loggerName.isEmpty ()) {
                        logger = LogFactory.rootLogger;
                     }
                     else {
                        logger = Logger.getLogger (loggerName);
                        LogFactory.loggerMap.put (loggerName, logger); // save it to avoid weak references !
                     }
                     logger.setLevel (Level.parse (prop.getProperty (key)));
                     boolean newLogger = LogManager.getLogManager ().addLogger (logger);
                     if (!newLogger) {
                        System.err.println ("LogFactory: logger '" + loggerName + "' already exist !");
                     }
                  }
               }
            }

         }
         catch (SecurityException e) {
            System.err.println ("LogFactory: Could not access to a valid '" + logConfFileName + "' !");
            e.printStackTrace ();
         }
         catch (IOException e) {
            System.err.println ("LogFactory: Could not access to a valid '" + logConfFileName + "' !");
            e.printStackTrace ();
         }
      }
   }

   public static Logger get (String name)
   {
      Logger out = Logger.getLogger (name);
      if (out.getParent () != null && out.getParent () != LogFactory.rootLogger && LogFactory.rootLogger != null) {
         // System.err.println ("LogFactory: root has changed for '" + name + "' (new root: " + out.getParent ()
         // + ") !! Setting root to our root.");
         // out.getParent ().addHandler (LogFactory.rootLogger.getHandlers () [0]);
         out.getParent ().setLevel (LogFactory.rootLogger.getLevel ());
         // out.setParent (LogFactory.rootLogger);
      }
      return out;
   }
}
