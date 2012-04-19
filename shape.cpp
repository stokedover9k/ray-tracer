#include "shape.h"

Shape::Shape(const Matrix& t, const Pigment& p, const Finish& f, float ior ) :
  _t(t), _p(p), _f(f), _ior(ior)
{ 
  if( ! _inv_t.setInverse(_t) ) 
    { throw "Shape::Shape: transformation has no inverse"; }
}

Finish::Finish( float ambient_, 
		float diffuse_,
		float phong_,
		float phong_size_,
		float metallic_,
		float reflection_ )
  : ambient(ambient_), diffuse(diffuse_), phong(phong_), phong_size(phong_size_), 
    metallic(metallic_), reflection(reflection_) 
{  }

bool Shape::intersect( const Ray& inc, Ray& hit_point ) const 
{
  throw "Shape::intersect(): implementation missing.";
}

Ray Shape::reflection_ray( const Ray& inc, const Ray ip ) const 
{
  return Ray( ip.origin(), 
	      inc.dir() - 2 * ip.dir() * ( inc.dir().dot(ip.dir()) ) );
}
