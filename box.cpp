#include "box.h"

Box::Box( const Vec3& c1, const Vec3& c2, const Shape& s ) 
  : Shape(s),  _c1(c1), _c2(c2)
{  }

bool Box::_intersect( const Ray& inc, Ray& hit ) const {
  
  
  return false;
}
