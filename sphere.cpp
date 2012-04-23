#include "sphere.h"


Sphere::Sphere( const Vec3& l, double r, const Shape& s ) :
  Shape(s), _l(l), _r(r)
{  }


bool Sphere::_intersect( const Ray& ray, Ray& hit ) const 
{
  const Vec3& d = _l - ray.from();
  double v = d.dot(ray.dir());
  double l = d.dot() - v*v;

  if( l > _r*_r ) return false;

  double D = sqrt( _r*_r - l );
  double t_inter = v - D;
  
  hit.from() = Vec3( ray.from() + ray.dir() * t_inter );
  hit.dir() = (hit.from() - _l).dir();

  return true;
}
