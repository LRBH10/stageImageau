/**
 * MathTools.h
 *
 * $Id: MathTools.h 617 2012-07-12 08:26:26Z bdm $
 */

#ifndef MATHTOOLS_H_
#define MATHTOOLS_H_

#include <cmath>

namespace imaGeau
{
   namespace Math
   {
      /**
       * @ingroup math
       */
      typedef struct _Point3d
      {
            double x, y, z;
      } Point3d;

      /**
       * @ingroup math
       * MathTools
       * @since 28 juin 2011
       * @author bdm
       */
      class MathTools
      {
         public:
            MathTools ();
            virtual ~MathTools ();

            /**
             * Compute the length of a 3D vector
             * @param dx x coord
             * @param dy y coord
             * @param dz z coord
             * @return vector length
             */
            static double dist (double dx, double dy, double dz);

            /**
             * Compute distance between 2 3D points
             * @param a first point
             * @param b second point
             * @return distance
             */
            static double dist (const Point3d & a, const Point3d & b);
      };

   }
} // end of namespace

using namespace imaGeau;
using namespace imaGeau::Math;

#endif /* MATHTOOLS_H_ */
