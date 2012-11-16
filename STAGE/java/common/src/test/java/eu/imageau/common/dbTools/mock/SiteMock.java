/* $Id: SiteMock.java 188 2011-11-17 09:59:14Z bdm $ */
package eu.imageau.common.dbTools.mock;

import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.persistence.Transient;

import org.hibernate.annotations.Cascade;
import org.hibernate.annotations.CascadeType;

import eu.imageau.common.DateUtils;
import eu.imageau.common.dbTools.PersistentObject;

/**
 * 
 * @author lionel
 */
@Entity
@Table(name = "SITE")
@NamedQueries({ @NamedQuery(name = "SiteMock.findAll", query = "SELECT s FROM SiteMock s"),
      @NamedQuery(name = "SiteMock.findById", query = "SELECT s FROM SiteMock s WHERE s.id = :id"),
      @NamedQuery(name = "SiteMock.findByNom", query = "SELECT s FROM SiteMock s WHERE s.nom = :nom"),
      @NamedQuery(name = "SiteMock.findByAdresse", query = "SELECT s FROM SiteMock s WHERE s.adresse = :adresse"),
      @NamedQuery(name = "SiteMock.findByDateExpiration", query = "SELECT s FROM SiteMock s WHERE s.dateExpiration = :dateExpiration"),
      @NamedQuery(name = "SiteMock.findByNoContrat", query = "SELECT s FROM SiteMock s WHERE s.noContrat = :noContrat") })
public class SiteMock implements PersistentObject
{
   /** */
   private static final long serialVersionUID = 1236247404624492942L;

   @Id
   @GeneratedValue(strategy = GenerationType.IDENTITY)
   @Column(name = "id")
   protected Integer id;

   @Column(name = "nom")
   private String nom;

   @Column(name = "adresse")
   private String adresse;

   @Column(name = "date_expiration")
   @Temporal(TemporalType.DATE)
   private Date dateExpiration;

   @Column(name = "no_contrat")
   private String noContrat;

   @ManyToOne(fetch = FetchType.LAZY)
   // no automatic parent delete
   @Cascade({ CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.SAVE_UPDATE, CascadeType.REPLICATE,
         CascadeType.LOCK, CascadeType.DETACH })
   @JoinColumn(name = "CONTACT_id", nullable = false)
   private ContactMock contact = null;

   @OneToMany(fetch = FetchType.LAZY, mappedBy = "site")
   @Cascade(CascadeType.ALL)
   private List<ProbeMock> probes = new LinkedList<ProbeMock> ();

   @Transient
   private ProbeMock consultingProbeMock;

   public SiteMock ()
   {
   }

   public SiteMock (String nom)
   {
      this.nom = nom;
   }

   public String getNom ()
   {
      return this.nom;
   }

   public void setId (int id)
   {
      this.id = id;
   }

   public void setNom (String nom)
   {
      this.nom = nom;
   }

   public String getAdresse ()
   {
      return this.adresse;
   }

   public void setAdresse (String adresse)
   {
      this.adresse = adresse;
   }

   public Date getDateExpiration ()
   {
      return this.dateExpiration;
   }

   public String getFormattedDateExpiration ()
   {
      return DateUtils.convertForDisplay (this.dateExpiration);
   }

   public void setDateExpiration (Date dateExpiration)
   {
      this.dateExpiration = dateExpiration;
   }

   public String getNoContrat ()
   {
      return this.noContrat;
   }

   public void setNoContrat (String noContrat)
   {
      this.noContrat = noContrat;
   }

   public void setContactMock (ContactMock c)
   {
      this.contact = c;
   }

   public ContactMock getContactMock ()
   {
      return this.contact;
   }

   @Override
   public String toString ()
   {
      return "eu.imageau.extranet.measureCenter.data.persistence.SiteMock[site=" + this.id + "]";
   }

   public void setConsultingProbeMock (ProbeMock p)
   {
      this.consultingProbeMock = p;
   }

   /**
    * Return current selected probe object
    * 
    * @return Slected site
    */
   public ProbeMock getConsultingProbeMock ()
   {
      return this.consultingProbeMock;
   }

   /**
    * Add a site to contact site list
    * 
    * @param s
    */
   public void addProbeMock (ProbeMock aProbeMock)
   {
      if (this.probes != null) {
         this.probes.add (aProbeMock);
      }
   }

   /**
    * ProbeMocks
    * 
    * @return ProbeMocks
    */
   public List<ProbeMock> getProbeMocks ()
   {
      return this.probes;
   }

   public String getProbeMocksSize ()
   {
      return "" + (this.probes == null ? 0 : this.probes.size ());
   }

   @Override
   public Integer getId ()
   {
      return this.id;
   }
}
