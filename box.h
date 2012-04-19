#ifndef __BOX_H__
#define __BOX_H__

#include "shape.h"

class Box : public Shape {
  
 public:
  Box( const Vec3& c1 = Vec3(0,0,0), const Vec3& c2 = Vec3(1,1,1), 
       const Shape& s = Shape() );
  
  virtual bool intersect( const Ray& inc, Ray& ip ) const;

 private:
  Vec3 _c1, _c2;    // corners

};

#endif
