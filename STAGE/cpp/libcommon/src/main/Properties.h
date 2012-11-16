/**
 * Properties.h
 *
 * $Id: Properties.h 617 2012-07-12 08:26:26Z bdm $
 */

#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include "commtype.h"
#include <map>
#include "StationException.h"
#include "IOTools.h"

// to avoid conflicts ...
namespace imaGeau
{

   using namespace std;

   /**
    * @ingroup commontools
    * Properties. Manage key=value pairs as java property file.
    * @since 28 juin 2011
    * @author bdm
    */
   class Properties
   {
         map<string, string> properties;

      public:
         /**
          * Default constructor
          */
         Properties ();

         /**
          * Copy constructor
          * @param prop object to copy
          */
         Properties (const Properties & prop);

         /**
          * Construct from file
          * @param fileName
          */
         Properties (const string & fileName);

         /**
          * Destructor
          */
         virtual ~Properties ();

         /**
          * Load all properties in the specified file
          * @param fileName property filename
          */
         void loadFromFile (const string & fileName);

         /**
          * Add property in the map
          * @param key property name
          * @param value property value
          */
         void add (const string & key, const string & value);

         /**
          * Add an existing property bundle to this
          * @param prop property bundle to merge
          */
         void add (const Properties & prop);

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
   };
} // end of namespace

#endif /* PROPERTIES_H_ */
