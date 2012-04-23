#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "shape.h"

class Cylinder : public Shape {
  
 public:
  /* constructs a unit cylinder (radius=1, height=1) with base at the x-y plane
     and axis along positive z-axis. The transformation matrix is adjusted in
     accordance with the requested parameters.
  */
  Cylinder( const Vec3& p1 = Vec3(0,0,0), const Vec3& p2 = Vec3(0,3,0), 
	    double r = 2,
	    const Shape& s = Shape() );

  /* intersect a circle (radius=1) on the x-y plane. 
   */
  static bool intersect_unit_circle( const Ray& inc, Ray& hit );

 protected:
  virtual bool _intersect( const Ray& inc, Ray& hit_point ) const;

 private:
  
};

#endif
