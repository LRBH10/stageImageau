/**
 * OptionParser.h
 *
 * $Id: OptionParser.h 617 2012-07-12 08:26:26Z bdm $
 */

#ifndef OPTIONPARSER_H_
#define OPTIONPARSER_H_

#include "LogFactory.h"
#include <map>
#include <list>

using namespace std;

namespace imaGeau
{
   /**
    * Save data to define a needded option
    */
   typedef struct _optionQuery
   {
         /// option name, ie. 'myName' for a cmd line option '-myName'
         string name;

         /// option documentation
         string doc;

         /// does it need a param
         bool needParam;

         /// is it optional or not
         bool isOptional;

         /**
          * Default constructor
          * @param n option name
          * @param d documentation
          * @param np does it need a param ?
          * @param op is it optionnal ?
          */
         _optionQuery (const string &n = "", const string &d = "", bool np = false, bool op = false) :
            name (n), doc (d), needParam (np), isOptional (op)
         {
         }

         /**
          * Comparison operator
          * @param o1 first obj
          * @param o2 second obj
          * @return string comparison based on the name.
          */
         bool operator() (const _optionQuery & o1, const _optionQuery & o2) const
         {
            return o1.name.compare (o2.name) < 0;
         }
   } OptionQuery;

   /**
    * Option parsing result for one OptionQuery
    */
   typedef struct _optionResult
   {
         /// read value from the command line
         string value;

         /// says if the option was set on the command line or not
         bool isSet;

         /**
          * Default constructor. isSet set to false.
          * @param v value to set
          */
         _optionResult (const string &v = "") :
            value (v), isSet (false)
         {
         }

         /**
          * Set the value and isSet to true
          * @param v value to set
          */
         void set (const string &v = "")
         {
            value = v;
            isSet = true;
         }

   } OptionResult;

   /**
    * map OptionResult according to OptionQuery, compare operator in OptionQuery
    */
   typedef map<OptionQuery, OptionResult, OptionQuery> OptionMap;

   /**
    * OptionParser
    * @since 5 août 2011
    * @author bdm
    */
   class OptionParser
   {
         OptionMap options;
      public:
         /**
          * Default construtor with a list of OptionQuery
          * @param optQ list of OptionQuery
          */
         OptionParser (const list<OptionQuery> & optQ);

         /**
          * default destructor
          */
         virtual ~OptionParser ();

         /**
          * Really parse the command line
          * @param argc size of argv
          * @param argv command line parameter array
          */
         void readOptions (int argc, char ** argv);

         /**
          * Generate available option list
          * @return generated string
          */
         string listOptions () const;

         /**
          * Generate available option documentation
          * @return generated string of all option documentation
          */
         string listOptionDetails () const;

         /**
          * Get command line parameter for a named option (CONST)
          * @param optName option name
          * @return parameter value
          */
         OptionMap::const_iterator get (const string & optName) const;

         /**
          * Get command line parameter for a named option
          * @param optName option name
          * @return parameter value
          */
         OptionMap::iterator get (const string & optName);

         /**
          * @param optName option name
          * @return true if the command line has set this option
          */
         bool isSet (const string & optName) const;

         static LoggerPtr logger;
   };
}

using namespace imaGeau;

#endif /* OPTIONPARSER_H_ */
