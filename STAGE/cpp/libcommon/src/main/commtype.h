/**
 * $Id: commtype.h 617 2012-07-12 08:26:26Z bdm $
 */

#ifndef _COMM_TYPE_H_
#define _COMM_TYPE_H_

#include "fString.h"
#include "string.h"

namespace imaGeau
{
   /**
    * @ingroup commontools
    * @defgroup commontype basic data type
    */
   //@{
   /// default separator for CSV (comma) files
#   define CSV_SEP '\t'

   using namespace std;

#   if !defined ulong || (defined _LINUX_ && !defined _SYS_TYPES_H)
#      define ulong unsigned long int
#   endif

#   define MAX_NAME_SIZE      1024

   typedef unsigned char byte;
   typedef unsigned char uint8;
   typedef unsigned short int uint16;
   typedef unsigned int uint32;
   typedef unsigned long uint64;

/// degree to radian
#   define DE2RA           0.01745329252 // = PI / 180.0
//@}

}// end of namespace

using namespace imaGeau;

#endif

