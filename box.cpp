#include "box.h"

Box::Box( const Vec3& c1, const Vec3& c2, const Shape& s ) 
  : Shape(s),  _c1(c1), _c2(c2)
{ 
  std::cout << c1 << " -> " << c2 << " : " << c1.min(c2) << std::endl;
  if( c1.min(c2) != c1 )
    throw "Box::Box(const Vec3&, const Vec3&): not all components of corner_1 "
      "are smaller than those of corner_2.";
}

bool Box::_intersect( const Ray& inc, Ray& hit ) const {
  double min_d = DBL_MAX;

  for( int i=0; i<3; i++ ) {
    size_t x = i;
    size_t y = (1+i) % 3;
    size_t z = (2+i) % 3;

    double d = -inc.dir()(z);
    double D1 = (inc.from()(z) - _c1(z)) / d - ZERO;
    double D2 = (inc.from()(z) - _c2(z)) / d - ZERO;

    Vec3 q1 = inc.from() + inc.dir() * D1;
    Vec3 q2 = inc.from() + inc.dir() * D2;
  
    if( D1 < min_d &&
	q1(x) >= _c1(x) && q1(x) <= _c2(x) && 
	q1(y) >= _c1(y) && q1(y) <= _c2(y) ) {

      Vec3 o(0,0,0);  o(z) = -1;
      hit.dir() = o;
      hit.from() = q1;
      min_d = D1;
    }

    if( D2 < min_d && 
	q2(x) >= _c1(x) && q2(x) <= _c2(x) && 
	q2(y) >= _c1(y) && q2(y) <= _c2(y) ) {

      Vec3 o(0,0,0);  o(z) = 1;
      hit.dir() = o;
      hit.from() = q2;
      min_d = D2;
    }
  }
  
  return min_d < DBL_MAX - ZERO;
}
