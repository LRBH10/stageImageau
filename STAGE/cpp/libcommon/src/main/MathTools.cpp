#include "MathTools.h"

MathTools::MathTools ()
{
   //nop
}

MathTools::~MathTools ()
{
   // nop
}

double MathTools::dist (double dx, double dy, double dz)
{
   double out = dx * dx + dy * dy + dz * dz;
   if (out >= 0.0) {
      out = sqrt (out);
   }
   return out;
}

double MathTools::dist (const Point3d & a, const Point3d & b)
{
   return MathTools::dist (b.x - a.x, b.y - a.y, b.z - a.z);
}
