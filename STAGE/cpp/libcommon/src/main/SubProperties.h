/**
 * SubProperties.h
 *
 * $Id: SubProperties.h 617 2012-07-12 08:26:26Z bdm $
 */

#ifndef SUBPROPERTIES_H_
#define SUBPROPERTIES_H_

#include "Properties.h"

using namespace imaGeau;

/**
 * @ingroup commontools
 * SubProperties. Manage properties with a key prefixed by a specific value
 * @since 29 sept. 2011
 * @author bdm
 */
class SubProperties
{
   private:
      /// associated property bundle
      const Properties & prop;

      /// key prefix
      string prefix;

   public:
      /**
       * Default constructor from a property bundle
       * @param prop property bundle
       * @param prefix fix prefix
       */
      SubProperties (const Properties & prop, const string & prefix);

      /**
       * Sub Sub ... construct
       * @param prop sub property bundle for which we get the root property bundle
       * @param prefix fix prefix
       */
      SubProperties (const SubProperties & prop, const string & prefix);

      /**
       * destructor
       */
      virtual ~SubProperties ();

      /**
       * Look for a string property
       * @param key property name
       * @param optional if true and the property does not exist returns the default value, else throws an exception
       * @param defVal default value
       * @return string value for key property name
       */
      string get (const string & key, bool optional = false, const string & defVal = "") const;

      /**
       * Look for an int property
       * @param key property name
       * @param optional if true and the property does not exist returns the default value, else throws an exception
       * @param defVal default value
       * @return int value for key property name
       */
      int getInt (const string & key, bool optional = false, int defVal = 0) const;

      /**
       * Look for a double property
       * @param key property name
       * @param optional if true and the property does not exist returns the default value, else throws an exception
       * @param defVal default value
       * @return double value for key property name
       */
      double getDouble (const string & key, bool optional = false, double defVal = 0.0) const;

      /**
       * Look for a boolean property. true is valid with 'true' 'TRUE' 'YES' 'yes' '1'
       * @param key property name
       * @param optional if true and the property does not exist returns the default value, else throws an exception
       * @param defVal default value
       * @return boolean value for key property name
       */
      bool getBoolean (const string & key, bool optional = false, bool defVal = true) const;

      /**
       * @return the associated property bundle
       */
      const Properties & getProperties () const;

      /**
       * @return the prefix
       */
      string getPrefix () const;

      /**
       * @param prefix new prefix
       */
      void setPrefix (string prefix);
};

#endif /* SUBPROPERTIES_H_ */
