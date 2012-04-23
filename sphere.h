#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "shape.h"

class Sphere : public Shape {
  
 public:
  Sphere( const Vec3& loc = Vec3(0,0,0),
	  double r        = 1.0,
	  const Shape& s  = Shape() );
  
 protected:
  virtual bool _intersect( const Ray& inc, Ray& hit_point ) const;
  
 private:
  Vec3 _l;
  double _r;
  
};

#endif
