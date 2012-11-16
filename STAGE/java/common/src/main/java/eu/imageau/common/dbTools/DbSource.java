/* $Id: DbSource.java 141 2011-09-27 08:29:39Z bdm $ */
package eu.imageau.common.dbTools;

import java.io.Serializable;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.hibernate.FlushMode;
import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.SQLQuery;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.cfg.AnnotationConfiguration;

import eu.imageau.common.LogFactory;

/**
 * This class is an accessor to Database Service, handles connection, disconnection, session and commiting data
 * 
 * @todo must check https://community.jboss.org/wiki/OpenSessionInView
 *       http://jpa.ezhibernate.com/Javacode/learn.jsp?tutorial=05samplehibernateutilclassexample
 * @author bdm
 */
public class DbSource implements Serializable
{
   private static final String HIBERNATE_DEBUG_CONFIGURATION_FILE = "src/main/resources/hibernate.cfg.xml";
   private static final String HIBERNATE_CONFIGURATION_FILE = "/WEB-INF/classes/hibernate.cfg.xml";
   private static final long serialVersionUID = -8608179508845928740L;
   private static SessionFactory sessionFactory;
   private transient ThreadLocal<Session> tlSession;

   static {
      if (DbSource.LOGGER == null) {
         System.err.println ("WTF !!! DbSource.LOGGER == null");
      }
      DbSource.sessionFactory = null;
      DbSource.configureSessionFactory ();
   }

   /**
    * Class constructor Setup configuration and connect to DB
    */
   public DbSource ()
   {
      DbSource.log (Level.INFO, "====================== new dbsource !!!! =====================");
      this.tlSession = new ThreadLocal<Session> ();
   }

   private static void log (Level level, String msg)
   {
      if (DbSource.LOGGER == null) {
         System.err.println ("WTF !!! DbSource.LOGGER == null, msg:'" + msg + "'");
      }
      else {
         DbSource.LOGGER.log (level, msg);
      }
   }

   private static void log (Level level, String msg, Exception ex)
   {
      if (DbSource.LOGGER == null) {
         System.err.println ("WTF !!! DbSource.LOGGER == null, msg: '" + msg + "', exception: '" + ex.getLocalizedMessage () + "'");
      }
      else {
         DbSource.LOGGER.log (level, msg, ex);
      }
   }

   @Override
   public void finalize ()
   {
      closeConnection ();
   }

   /**
    * Returns the DB Configuration path
    * 
    * @return path to hibernate.cfg.xml
    */
   public static String getDbConfigurationFile (Object context)
   {
      return context == null ? HIBERNATE_DEBUG_CONFIGURATION_FILE : HIBERNATE_CONFIGURATION_FILE;
   }

   /**
    * Configure and create a BDD Session
    * 
    * @return Current configured Session
    */
   private static void configureSessionFactory ()
   {
      try {
         if (DbSource.sessionFactory == null || DbSource.sessionFactory.isClosed ()) {
            AnnotationConfiguration anConf = new AnnotationConfiguration ();
            DbSource.sessionFactory = anConf.configure ().buildSessionFactory ();
         }
      }
      catch (Exception ex) {
         DbSource.log (Level.SEVERE, "Unable to configure hibernate session. Caused by: " + ex.getLocalizedMessage ());
      }
      catch (Throwable t) {
         DbSource.log (Level.SEVERE, "Unable to configure hibernate session. Caused by: " + t.getLocalizedMessage ());
      }
   }

   /**
    * Can be overridden
    * 
    * @param s
    */
   protected void _saveSession (Session s)
   {
      if (this.tlSession != null) {
         this.tlSession.set (s);
      }
   }

   /**
    * Can be overridden
    * 
    * @return
    */
   protected Session _retrieveSession ()
   {
      Session out = null;
      if (this.tlSession != null) {
         out = this.tlSession.get ();
      }
      return out;
   }

   public Session getSession ()
   {
      Session session = _retrieveSession ();
      if (session == null || !session.isOpen ()) {
         if (DbSource.sessionFactory == null) {
            DbSource.configureSessionFactory ();
         }
         if (DbSource.sessionFactory != null && !DbSource.sessionFactory.isClosed ()) {
            session = DbSource.sessionFactory.openSession ();
            session.setFlushMode (FlushMode.AUTO);
            DbSource.log (Level.INFO, "====================== new db SESSION !!!! ===================== id:" + session.hashCode ());
         }
         else {
            session = null;
         }
         _saveSession (session);
      }
      return session;
   }

   /**
    * Submit a named query to DB
    * 
    * @param aName Named Query Identifier
    * @param aTokens Query Parameters
    * @return List of persitent objects returned by query
    */
   public <T> List<T> fetchNamedQuery (Class<T> clazz, String aName, QueryDatas[] aTokens)
   {
      return fetchNamedQuery (clazz, aName, aTokens, -1);
   }

   /**
    * Submit a named query to DB
    * 
    * @param clazz defined for java runtime to infer T type
    * @param aName Named Query Identifier
    * @param aTokens Query Parameters
    * @return List of persitent objects returned by query
    */
   @SuppressWarnings("unchecked")
   public <T> List<T> fetchNamedQuery (Class<T> clazz, String aName, QueryDatas[] aTokens, int limit)
   {
      List<T> out = null;
      // begin a transaction
      try {
         Query query = getSession ().getNamedQuery (aName);
         if (aTokens.length != 0) {
            for (int i = 0; i < aTokens.length; i++) {
               query.setString (aTokens [i].getName (), aTokens [i].getValue ());
            }
         }
         if (limit > 0) {
            query.setMaxResults (limit);
         }
         out = query.list ();
         disconnect ();
      }
      catch (Exception ex) {
         DbSource.log (Level.SEVERE, "Unable to fetchNamedQuery", ex);
         closeConnection ();
      }
      return out;
   }

   /**
    * Submit a specified query to DB
    * 
    * @param aQuery SQL Query
    * @param classe Class entity to drive
    * @return List of persitent objects returned by query
    */
   @SuppressWarnings("unchecked")
   public <T> List<T> fetchQuery (Class<T> clazz, String aQuery)
   {
      if (aQuery == null) {
         throw new NullPointerException ("aQuery is null");
      }
      if (clazz == null) {
         throw new NullPointerException ("Classe is null");
      }

      List<T> qList = null;

      try {

         SQLQuery q = getSession ().createSQLQuery (aQuery);
         if (q == null) {
            throw new NullPointerException ("Query is null");
         }
         q = q.addEntity (clazz);
         if (q == null) {
            throw new NullPointerException ("Final query is null");
         }
         qList = q.list ();
         disconnect ();
      }
      catch (Exception ex) {
         DbSource.log (Level.SEVERE, "Unable to fetchQuery: " + aQuery, ex);
         closeConnection ();
      }

      return qList;
   }

   /**
    * Commit specified object in Database
    * 
    * @param data : object to commit
    * @return State of commitment
    */
   public boolean commit (PersistentObject data)
   {
      boolean out = false;
      Transaction tx = null;
      try {
         tx = getSession ().beginTransaction ();
         PersistentObject dataTmp = (PersistentObject) getSession ().merge (data);
         getSession ().update (dataTmp);
         tx.commit ();
         // tx = getSession ().beginTransaction ();
         // getSession ().refresh (data);
         // tx.commit ();
         disconnect ();
         out = true;
      }
      catch (IllegalArgumentException t) {
         DbSource.log (Level.SEVERE, "Unable to commit", t);
         if (tx != null) {
            tx.rollback ();
         }
         closeConnection ();
      }
      catch (HibernateException t) {
         DbSource.log (Level.SEVERE, "Unable to commit", t);
         if (tx != null) {
            tx.rollback ();
         }
         closeConnection ();
      }
      catch (RuntimeException t) {
         DbSource.log (Level.SEVERE, "Unable to commit", t);
         if (tx != null) {
            tx.rollback ();
         }
         closeConnection ();
      }
      return out;
   }

   /**
    * Commit specified object in Database
    * 
    * @param data : object to commit
    * @return State of commitment
    */
   public <T extends PersistentObject> T insertAndLoad (PersistentObject data, Class<T> clazz)
   {
      T out = null;
      Integer id = insert (data);
      if (id != null) {
         out = load (id, clazz);
      }
      return out;
   }

   /**
    * Commit specified object in Database
    * 
    * @param data : object to commit
    * @return State of commitment
    */
   @SuppressWarnings("unchecked")
   public <T extends PersistentObject> T load (Integer id, Class<T> clazz)
   {
      T out = null;
      Transaction tx = null;
      try {
         tx = getSession ().beginTransaction ();
         out = (T) getSession ().load (clazz, id);
         tx.commit ();
         disconnect ();
      }
      catch (IllegalArgumentException t) {
         DbSource.log (Level.SEVERE, "Unable to commit", t);
         if (tx != null) {
            tx.rollback ();
         }
         closeConnection ();
      }
      catch (HibernateException t) {
         DbSource.log (Level.SEVERE, "Unable to commit", t);
         if (tx != null) {
            tx.rollback ();
         }
         closeConnection ();
      }
      catch (RuntimeException t) {
         DbSource.log (Level.SEVERE, "Unable to commit", t);
         if (tx != null) {
            tx.rollback ();
         }
         closeConnection ();
      }
      return out;
   }

   /**
    * insert specified object in Database
    * 
    * @param data : object to insert
    * @return id of new inserted object (Integer)
    */
   public Integer insert (PersistentObject data)
   {
      return insert (data, Integer.class);
   }

   /**
    * insert specified object in Database
    * 
    * @param data : object to insert
    * @param clazz defined for java runtime to infer T type
    * @return id of new inserted object (inferred T type)
    */
   @SuppressWarnings("unchecked")
   public <T extends Serializable> T insert (PersistentObject data, Class<T> clazz)
   {
      T out = null;
      Transaction tx = null;
      try {
         tx = getSession ().beginTransaction ();
         out = (T) getSession ().save (data);
         // PersistentObject dataTmp = (PersistentObject) getSession ().merge (data);
         // out = (T) dataTmp.getId ();
         tx.commit ();

         disconnect ();
      }
      catch (IllegalArgumentException t) {
         DbSource.log (Level.SEVERE, "Unable to insert", t);
         if (tx != null) {
            tx.rollback ();
         }
         out = null;
         closeConnection ();
      }
      catch (HibernateException t) {
         DbSource.log (Level.SEVERE, "Unable to insert", t);
         if (tx != null) {
            tx.rollback ();
         }
         out = null;
         closeConnection ();
      }
      catch (RuntimeException t) {
         DbSource.log (Level.SEVERE, "Unable to insert", t);
         if (tx != null) {
            tx.rollback ();
         }
         out = null;
         closeConnection ();
      }
      return out;
   }

   /**
    * execute the specified Query
    * 
    * @param hqlquery query to exec
    * @return State of deletion
    */
   public int execQuery (String hqlquery)
   {
      int out;
      try {
         Query q = getSession ().createSQLQuery (hqlquery);
         out = q.executeUpdate ();
         disconnect ();
      }
      catch (Exception t) {
         DbSource.log (Level.SEVERE, "Unable to execute query: " + hqlquery, t);
         closeConnection ();
         out = 0;
      }
      return out;
   }

   /**
    * @param data data to be refresh from db
    */
   public <PersistentClass extends PersistentObject> void reload (PersistentClass data)
   {
      try {
         Transaction tx = getSession ().beginTransaction ();
         getSession ().refresh (data);
         tx.commit ();
         disconnect ();
      }
      catch (IllegalArgumentException t) {
         DbSource.log (Level.SEVERE, "Unable to reload", t);
         closeConnection ();
      }
      catch (HibernateException t) {
         DbSource.log (Level.SEVERE, "Unable to reload", t);
         closeConnection ();
      }
      catch (RuntimeException t) {
         DbSource.log (Level.SEVERE, "Unable to reload", t);
         closeConnection ();
      }
   }

   /**
    * @param data data to be refresh from db
    */
   public <PersistentClass extends PersistentObject> void merge (PersistentClass data)
   {
      try {
         Transaction tx = getSession ().beginTransaction ();
         getSession ().merge (data);
         tx.commit ();
         disconnect ();
      }
      catch (IllegalArgumentException t) {
         DbSource.log (Level.SEVERE, "Unable to reload", t);
         closeConnection ();
      }
      catch (HibernateException t) {
         DbSource.log (Level.SEVERE, "Unable to reload", t);
         closeConnection ();
      }
      catch (RuntimeException t) {
         DbSource.log (Level.SEVERE, "Unable to reload", t);
         closeConnection ();
      }
   }

   /**
    * Close the MySQL connection
    */
   public void disconnect ()
   {
      Session session = _retrieveSession ();
      if (session != null) {
         session.disconnect ();
      }
   }

   /**
    * Close the MySQL connection
    */
   public void closeConnection ()
   {
      Session session = _retrieveSession ();
      if (session != null) {
         _saveSession (null);
         DbSource.log (Level.INFO, "====================== close db SESSION !!!! ===================== id:" + session.hashCode ());
         session.flush ();
         session.close ();
      }
      if (DbSource.sessionFactory != null) {
         DbSource.sessionFactory.close ();
         DbSource.sessionFactory = null;
      }
   }

   /**
    * Returs the connection close state
    * 
    * @return Connection close State in boolean value
    */
   public boolean isClosed ()
   {
      return DbSource.sessionFactory == null || DbSource.sessionFactory.isClosed ();
   }

   protected <PersistentClass extends PersistentObject> void delete (PersistentClass data)
   {
      try {
         Transaction tx = getSession ().beginTransaction ();
         getSession ().delete (data);
         tx.commit ();
         disconnect ();
      }
      catch (IllegalArgumentException t) {
         DbSource.log (Level.SEVERE, "Unable to delete", t);
         closeConnection ();
         throw t;
      }
      catch (HibernateException t) {
         DbSource.log (Level.SEVERE, "Unable to delete", t);
         closeConnection ();
         throw t;
      }
      catch (RuntimeException t) {
         DbSource.log (Level.SEVERE, "Unable to delete", t);
         closeConnection ();
         throw t;
      }
   }

   private final static Logger LOGGER = LogFactory.get (DbSource.class.getName ());
}
