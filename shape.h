#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <float.h>

#include "headers.h"
#include "ray.h"

#define FINISH_DEFAULT_AMBIENT      0.1
#define FINISH_DEFAULT_DIFFUSE      0.6
#define FINISH_DEFAULT_PHONG        0.0
#define FINISH_DEFAULT_PHONG_SIZE  40
#define FINISH_DEFAULT_METALLIC     0
#define FINISH_DEFAULT_REFLECTION   0.0

class Finish {
 public:
  Finish( float ambient_    = FINISH_DEFAULT_AMBIENT,
	  float diffuse_    = FINISH_DEFAULT_DIFFUSE,
	  float phong_      = FINISH_DEFAULT_PHONG,
	  float phong_size_ = FINISH_DEFAULT_PHONG_SIZE,
	  float metallic_   = FINISH_DEFAULT_METALLIC,
	  float reflection_ = FINISH_DEFAULT_REFLECTION );

  float ambient;
  float diffuse;
  float phong;
  float phong_size;
  float metallic;
  float reflection;
};

//-----------------------------------------------------------------------------

class Shape {
  
 public:

  Shape( const Matrix&  transform = Matrix(),
	 const Pigment& p         = Pigment(0,0,0,0),
	 const Finish&  f         = Finish(),
	 float ior                = 1.0 );

  // @param ip: origin at the intersection and direction as the normal
  virtual bool intersect( const Ray& inc, Ray& ip ) const;
  
  //Ray reflection_ray( const Ray& inc, const Ray ip ) const;

  Matrix&       transform    (void)        { return _t; }
  const Matrix& transform    (void) const  { return _t; }
  Matrix&       inv_transform(void)        { return _inv_t; }
  const Matrix& inv_transform(void) const  { return _inv_t; }

  const Finish&  finish (void) const { return _f; }
  const Pigment& pigment(void) const { return _p; }

 protected:
  virtual bool _intersect( const Ray& inc, Ray& ip ) const;

 private:
  Matrix  _t, _inv_t;
  Pigment _p;
  Finish  _f;
  float _ior;
  
};

#endif
