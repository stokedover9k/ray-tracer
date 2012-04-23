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

bool Shape::intersect( const Ray& inc, Ray& ip ) const 
{
  if( this->_intersect( Ray(inc).transform( inv_transform() ), ip ) ) {
    ip = ip.transform( transform() );
    return true;
  }
  return false;
}

bool Shape::_intersect( const Ray& inc, Ray& ip ) const {
  throw "Shape::intersect(const Ray&, Ray&): implementation missing.";
}

