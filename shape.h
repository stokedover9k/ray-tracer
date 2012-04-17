#ifndef __SHAPE_H__
#define __SHAPE_H__

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
	 const Finish& f          = Finish() );

  virtual bool intersect( const Ray& inc,
			  Ray& returned,
			  Vec3& hit_point,
			  Vec3& normal,
			  size_t trace_depth ) const = 0;
  
 private:
  Matrix  _t;
  Pigment _p;
  Finish  _f;
  
};

#endif
