/**
 * 
 */
package eu.imageau.common;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @author bdm
 * 
 */
public abstract class ResourceResolver
{

   /**
    * 
    */
   public ResourceResolver ()
   {
      // TODO Auto-generated constructor stub
   }

   /**
    * This method open stream through the current context
    * 
    * @param aFileStream file pname to open
    * @return Stream on file specified by aFileStream
    * @throws FileNotFoundException
    */
   public InputStream openStream (String aFileStream) throws FileNotFoundException
   {
      return openStream (aFileStream, ResourceResolver.class);
   }

   /**
    * This method open stream through the current context
    * 
    * @param aFileStream file pname to open
    * @return Stream on file specified by aFileStream
    * @throws FileNotFoundException
    */
   public InputStream openStream (String aFileStream, Class<?> clazz) throws FileNotFoundException
   {
      InputStream out = ClassLoader.getSystemResourceAsStream (aFileStream);

      if (out == null) {
         ResourceResolver.LOGGER.info ("Default class loader failed (file:" + aFileStream + ") !");
         out = clazz.getResourceAsStream (aFileStream);
      }

      if (out == null) {
         ResourceResolver.LOGGER.info ("ResourceResolver class loader failed (file:" + aFileStream + ") !");
         out = ClassLoader.getSystemResourceAsStream ("/" + aFileStream);
      }

      if (out == null) {
         ResourceResolver.LOGGER.info ("ResourceResolver class loader failed (+ /) (file:" + aFileStream + ") !");
         out = clazz.getResourceAsStream ("/" + aFileStream);
      }

      if (out == null) {
         try {
            URL temp = resourceResolverHook (aFileStream);
            if (temp != null) {
               out = temp.openStream ();
            }
         }
         catch (IOException e) {
            ResourceResolver.LOGGER.log (Level.SEVERE, "IOException", e);
         }
      }

      if (out == null) {
         try {
            out = new FileInputStream (aFileStream);
         }
         catch (FileNotFoundException ex) {
            ResourceResolver.LOGGER.log (Level.SEVERE, "FileNotFoundException", ex);
         }
      }

      if (out != null) {
         ResourceResolver.LOGGER.info ("Success found file:" + aFileStream + " !");
      }
      else {
         throw new FileNotFoundException ("File not found !!! " + aFileStream + " !");
      }

      return out;
   }

   /**
    * @param aFileStream  
    */
   URL resourceResolverHook (String aFileStream)
   {
      URL out = null;
      // if (getFacesContext () != null) {
      // out = FacesContext.getCurrentInstance ().getExternalContext ().getResource (aFileStream);
      // }
      return out;
   }

   /**
    * This method open stream through the current context
    * 
    * @param aFileStream file pname to open
    * @return Stream on file specified by aFileStream
    * @throws MalformedURLException
    */
   public URL openStreamUrl (String aFileStream) throws MalformedURLException
   {
      return openStreamUrl (aFileStream, ResourceResolver.class);
   }

   /**
    * This method open stream through the current context
    * 
    * @param aFileStream file pname to open
    * @return Stream on file specified by aFileStream
    * @throws MalformedURLException
    */
   public URL openStreamUrl (String aFileStream, Class<?> clazz) throws MalformedURLException
   {
      URL out = ClassLoader.getSystemResource (aFileStream);

      if (out == null) {
         ResourceResolver.LOGGER.info ("Default class loader failed (file:" + aFileStream + ") !");
         out = clazz.getResource (aFileStream);
      }

      if (out == null) {
         ResourceResolver.LOGGER.info ("ResourceResolver class loader failed (file:" + aFileStream + ") !");
         out = ClassLoader.getSystemResource ("/" + aFileStream);
      }

      if (out == null) {
         ResourceResolver.LOGGER.info ("ResourceResolver class loader failed (+ /) (file:" + aFileStream + ") !");
         out = clazz.getResource ("/" + aFileStream);
      }

      if (out == null) {
         out = resourceResolverHook (aFileStream);
      }

      if (out == null) {
         ResourceResolver.LOGGER.log (Level.SEVERE, "FileNotFoundException");
      }

      if (out != null) {
         ResourceResolver.LOGGER.info ("Success found file:" + aFileStream + " !");
      }

      return out;
   }

   private static final Logger LOGGER = LogFactory.get (ResourceResolver.class.getName ());
}
