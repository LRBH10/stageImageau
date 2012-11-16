/* $Id: ProbeMock.java 188 2011-11-17 09:59:14Z bdm $ */
package eu.imageau.common.dbTools.mock;

import java.util.Date;

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
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;

import org.hibernate.annotations.Cascade;
import org.hibernate.annotations.CascadeType;

import eu.imageau.common.DateUtils;
import eu.imageau.common.dbTools.PersistentObject;

/**
 * This class describes entity for probe datas in DB
 * 
 * @author lionel
 */
@Entity
@Table(name = "PROBE")
@NamedQueries({
      @NamedQuery(name = "ProbeMock.findAll", query = "SELECT s FROM ProbeMock s"),
      @NamedQuery(name = "ProbeMock.findById", query = "SELECT s FROM ProbeMock s WHERE s.id = :id"),
      @NamedQuery(name = "ProbeMock.findByName", query = "SELECT s FROM ProbeMock s WHERE s.nom = :name"),
      @NamedQuery(name = "ProbeMock.findByDateDeMiseEnFonctionnement",
            query = "SELECT s FROM ProbeMock s WHERE s.dateDeMiseEnFonctionnement = :dateDeMiseEnFonctionnement"),
      @NamedQuery(name = "ProbeMock.findByGeolocalisation", query = "SELECT s FROM ProbeMock s WHERE s.geolocalisation = :geolocalisation"),
      @NamedQuery(name = "ProbeMock.findByDateDerniereCalibration",
            query = "SELECT s FROM ProbeMock s WHERE s.dateDerniereCalibration = :dateDerniereCalibration"),
      @NamedQuery(name = "ProbeMock.findByTypeEnergie", query = "SELECT s FROM ProbeMock s WHERE s.typeEnergie = :typeEnergie"),
      @NamedQuery(name = "ProbeMock.findByTypeAcquisition", query = "SELECT s FROM ProbeMock s WHERE s.typeAcquisition = :typeAcquisition"),
      @NamedQuery(name = "ProbeMock.findByTypeCommunication",
            query = "SELECT s FROM ProbeMock s WHERE s.typeCommunication = :typeCommunication"),
      @NamedQuery(name = "ProbeMock.findByMaj", query = "SELECT s FROM ProbeMock s WHERE s.maj = :maj") })
public class ProbeMock implements PersistentObject
{
   private static final long serialVersionUID = 1502653078178006926L;

   @Id
   @GeneratedValue(strategy = GenerationType.IDENTITY)
   @Column(name = "id")
   private int id;

   @Column(name = "nom")
   private String nom = "";

   @Column(name = "date_de_mise_en_fonctionnement")
   @Temporal(TemporalType.DATE)
   private Date dateDeMiseEnFonctionnement;

   @Column(name = "geolocalisation")
   private String geolocalisation = "";

   @Column(name = "date_derniere_calibration")
   @Temporal(TemporalType.DATE)
   private Date dateDerniereCalibration = new Date ();

   @Column(name = "notification")
   private Integer notification = 0;

   @Column(name = "type_energie")
   private String typeEnergie = "";

   @Column(name = "type_acquisition")
   private String typeAcquisition = "";

   @Column(name = "type_communication")
   private String typeCommunication = "";

   @Column(name = "MAJ")
   @Temporal(TemporalType.DATE)
   private Date maj = new Date ();

   @ManyToOne(fetch = FetchType.LAZY)
   // no automatic parent delete
   @Cascade({ CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.SAVE_UPDATE, CascadeType.REPLICATE,
         CascadeType.LOCK, CascadeType.DETACH })
   @JoinColumn(name = "SITE_id", nullable = false)
   private SiteMock site = null;

   @Column(name = "num_tel")
   private String numTel = "";

   public ProbeMock ()
   {
   }

   public ProbeMock (int id)
   {
      this.id = id;
   }

   public String getNom ()
   {
      return this.nom;
   }

   public void setNom (String aName)
   {
      this.nom = aName;
   }

   public Date getDateDeMiseEnFonctionnement ()
   {
      return this.dateDeMiseEnFonctionnement;
   }

   public String getFormattedDateDeMiseEnFonctionnement ()
   {
      return DateUtils.convertForDisplay (this.dateDeMiseEnFonctionnement);
   }

   public void setDateDeMiseEnFonctionnement (Date dateDeMiseEnFonctionnement)
   {
      this.dateDeMiseEnFonctionnement = dateDeMiseEnFonctionnement;
   }

   public String getGeolocalisation ()
   {
      return this.geolocalisation;
   }

   public void setGeolocalisation (String geolocalisation)
   {
      this.geolocalisation = geolocalisation;
   }

   public Date getDateDerniereCalibration ()
   {
      return this.dateDerniereCalibration;
   }

   public String getFormattedDateDerniereCalibration ()
   {
      return DateUtils.convertForDisplay (this.dateDerniereCalibration);
   }

   public void setDateDerniereCalibration (Date dateDerniereCalibration)
   {
      this.dateDerniereCalibration = dateDerniereCalibration;
   }

   public Boolean getNotification ()
   {
      return this.notification == 1;
   }

   public void setNotification (Boolean notification)
   {

      this.notification = notification ? 1 : 0;
   }

   public String getTypeEnergie ()
   {
      return this.typeEnergie;
   }

   public void setTypeEnergie (String typeEnergie)
   {
      this.typeEnergie = typeEnergie;
   }

   public String getTypeAcquisition ()
   {
      return this.typeAcquisition;
   }

   public void setTypeAcquisition (String typeAcquisition)
   {
      this.typeAcquisition = typeAcquisition;
   }

   public String getTypeCommunication ()
   {
      return this.typeCommunication;
   }

   public void setTypeCommunication (String typeCommunication)
   {
      this.typeCommunication = typeCommunication;
   }

   public Date getMaj ()
   {
      return this.maj;
   }

   public String getFormattedMaj ()
   {
      return DateUtils.convertForDisplay (this.maj);
   }

   public void setMaj (Date maj)
   {
      this.maj = maj;
   }

   @Override
   public String toString ()
   {
      return "eu.imageau.extranet.measureCenter.data.persistence.ProbeMock[id=" + this.id + "]";
   }

   /**
    * @return the numTel
    */
   public String getNumTel ()
   {
      return this.numTel;
   }

   /**
    * @param numTel the numTel to set
    */
   public void setNumTel (String numTel)
   {
      this.numTel = numTel;
   }

   @Override
   public Integer getId ()
   {
      return this.id;
   }

   /**
    * @return the site
    */
   public SiteMock getSiteMock ()
   {
      return this.site;
   }

   /**
    * @param site the site to set
    */
   public void setSiteMock (SiteMock site)
   {
      this.site = site;
   }

}
