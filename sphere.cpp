#include "sphere.h"


Sphere::Sphere( const Vec3& l, double r, const Shape& s ) :
  Shape(s), _l(l), _r(r)
{  }


bool Sphere::intersect( const Ray& inc, Ray& hit ) const 
{
  const Ray& ray = inc.transform( inv_transform() );
  const Vec3& d = _l - ray.from();
  double v = d.dot(ray.dir());
  double l = d.dot() - v*v;

  if( l > _r*_r ) return false;

  double D = sqrt( _r*_r - l );
  double t_inter = v - D;
  
  const Vec3& hit_point = Vec3( ray.from() + ray.dir() * t_inter );
  const Vec3& normal = (hit_point - _l).dir();
    
  hit = Ray( hit_point, hit_point - _l ).transform( transform() );

  return true;
}
