/* $Id: ContactMock.java 188 2011-11-17 09:59:14Z bdm $ */
package eu.imageau.common.dbTools.mock;

import java.util.Date;
import java.util.LinkedList;
import java.util.List;
import java.util.logging.Logger;

import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.persistence.Transient;

import org.hibernate.annotations.Cascade;
import org.hibernate.annotations.CascadeType;
import org.hibernate.annotations.LazyCollection;
import org.hibernate.annotations.LazyCollectionOption;

import eu.imageau.common.LogFactory;
import eu.imageau.common.dbTools.PersistentObject;

/**
 * imaGeau client contact
 * 
 * @author lionel
 */
@Entity
@Table(name = "CONTACT")
@NamedQueries({ @NamedQuery(name = "ContactMock.findAll", query = "SELECT c FROM ContactMock c ORDER BY c.societe"),
      @NamedQuery(name = "ContactMock.findById", query = "SELECT c FROM ContactMock c WHERE c.id = :id"),
      @NamedQuery(name = "ContactMock.findByNom", query = "SELECT c FROM ContactMock c WHERE c.nom = :nom"),
      @NamedQuery(name = "ContactMock.findBySociete", query = "SELECT c FROM ContactMock c WHERE c.societe = :societe"),
      @NamedQuery(name = "ContactMock.findByFonction", query = "SELECT c FROM ContactMock c WHERE c.fonction = :fonction"),
      @NamedQuery(name = "ContactMock.findByTel", query = "SELECT c FROM ContactMock c WHERE c.tel = :tel"),
      @NamedQuery(name = "ContactMock.findByEmail", query = "SELECT c FROM ContactMock c WHERE c.email = :email"),
      @NamedQuery(name = "ContactMock.findByLogin", query = "SELECT c FROM ContactMock c WHERE c.login = :login"),
      @NamedQuery(name = "ContactMock.findByPwd", query = "SELECT c FROM ContactMock c WHERE c.pwd = :pwd"),
      @NamedQuery(name = "ContactMock.connect", query = "SELECT c FROM ContactMock c WHERE c.login = :login AND c.pwd = :pwd") })
public class ContactMock implements PersistentObject
{
   private static final long serialVersionUID = 4946581386310303709L;

   @Id
   @GeneratedValue(strategy = GenerationType.IDENTITY)
   @Basic(optional = false)
   @Column(name = "id")
   private Integer id;

   @Column(name = "nom")
   private String nom;

   @Column(name = "societe")
   private String societe;

   @Column(name = "fonction")
   private String fonction;

   @Column(name = "tel")
   private String tel;

   @Column(name = "email")
   private String email;

   @Column(name = "login")
   private String login;

   @Column(name = "pwd")
   private String pwd;

   @Column(name = "adminFlag")
   private int admin = 0;

   @Column(name = "conditionsacquitmentdate")
   @Temporal(TemporalType.TIMESTAMP)
   private Date conditionAcquitmentdate = null;

   @Column(name = "conditionsacquitmentIP")
   private String conditionAcquitmentIP;

   @OneToMany(mappedBy = "contact")
   // NO DELETE !!!
   @Cascade({ CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.SAVE_UPDATE, CascadeType.REPLICATE,
         CascadeType.LOCK, CascadeType.DETACH })
   @LazyCollection(LazyCollectionOption.TRUE)
   private List<SiteMock> sites = new LinkedList<SiteMock> ();

   @Transient
   private SiteMock consultingSiteMock;

   public ContactMock ()
   {
   }

   public ContactMock (Integer id)
   {
      this.id = id;
   }

   public ContactMock (String name)
   {
      this.nom = name;
   }

   @Override
   public Integer getId ()
   {
      return this.id;
   }

   public void setId (Integer id)
   {
      this.id = id;
   }

   public String getNom ()
   {
      return this.nom;
   }

   public void setNom (String nom)
   {
      this.nom = nom;
   }

   public String getSociete ()
   {
      return this.societe;
   }

   public void setSociete (String societe)
   {
      this.societe = societe;
   }

   public String getFonction ()
   {
      return this.fonction;
   }

   public void setFonction (String fonction)
   {
      this.fonction = fonction;
   }

   public String getTel ()
   {
      return this.tel;
   }

   public void setTel (String tel)
   {
      this.tel = tel;
   }

   public String getEmail ()
   {
      return this.email;
   }

   public void setEmail (String email)
   {
      this.email = email;
   }

   public String getLogin ()
   {
      return this.login;
   }

   public void setLogin (String login)
   {
      this.login = login;
   }

   public String getPwd ()
   {
      return this.pwd == null ? null : this.pwd;
   }

   public String getEncryptedPwd ()
   {
      return this.pwd;
   }

   /**
    * Set password from unencrypted to encrypted
    * 
    * @param pwd not encrypted password
    */
   public void setPwd (String pwd)
   {
      this.pwd = pwd;
   }

   public boolean isAdministrator ()
   {
      return this.admin == 1;
   }

   @Override
   public int hashCode ()
   {
      int hash = 0;
      hash += this.id != null ? this.id.hashCode () : 0;
      return hash;
   }

   @Override
   public boolean equals (Object object)
   {
      boolean out = false;
      if (object instanceof ContactMock) {
         ContactMock other = (ContactMock) object;
         out = this.id != null && other.id != null && this.id.equals (other.id);
      }
      return out;
   }

   @Override
   public String toString ()
   {
      return "eu.imageau.extranet.measureCenter.data.persistence.ContactMock[id=" + this.id + "]";
   }

   /**
    * Affect the selecte sit index
    * 
    * @param siteId
    */
   public void setConsultingSiteMock (SiteMock site)
   {
      this.consultingSiteMock = site;
   }

   /**
    * Return current selected site object
    * 
    * @return Slected site
    */
   public SiteMock getConsultingSiteMock ()
   {
      return this.consultingSiteMock; // this.sites.get (this.consultingSiteMockIndex);
   }

   public List<SiteMock> getSiteMocks ()
   {
      return this.sites;
   }

   public void addSiteMock (SiteMock aSiteMock)
   {
      this.sites.add (aSiteMock);
   }

   public Date getConditionAcquitmentdate ()
   {
      return this.conditionAcquitmentdate;
   }

   public void setConditionAcquitmentdate (Date conditionAcquitmentdate)
   {
      this.conditionAcquitmentdate = conditionAcquitmentdate;
   }

   /**
    * @param conditionAcquitmentIP the conditionAcquitmentIP to set
    */
   public void setConditionAcquitmentIP (String conditionAcquitmentIP)
   {
      this.conditionAcquitmentIP = conditionAcquitmentIP;
   }

   /**
    * @return the conditionAcquitmentIP
    */
   public String getConditionAcquitmentIP ()
   {
      return this.conditionAcquitmentIP;
   }

   @SuppressWarnings("unused")
   private static final Logger LOGGER = LogFactory.get (ContactMock.class.getName ());
}
