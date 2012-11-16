/**
 * Electrode.h
 *
 * $Id: Electrode.h 206 2011-07-01 12:54:58Z bdm $
 */

#ifndef _ELECTRODE_H
#define _ELECTRODE_H

/**
 * Only a struct for electrode data storage.
 */
class Electrode
{
   public:
      uint32 id;

      double x;

      double y;

      double z;

      Electrode (uint32 i = 0U, double _x = 0.0, double _y = 0.0, double _z = 0.0) :
         id (i), x (_x), y (_y), z (_z)
      {

      }
};

#endif
