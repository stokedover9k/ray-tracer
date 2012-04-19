#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "shape.h"

class Cylinder : public Shape {
  
 public:
  Cylinder( const Vec3& p1 = Vec3(0,0,0), const Vec3& p2 = Vec3(0,3,0), 
	    double r = 2,
	    const Shape& s = Shape() );

  virtual bool intersect( const Ray& inc, Ray& hit_point ) const;

 private:
  Vec3 _p1, _p2;
  double _r;
  
};

#endif
