#ifndef __SCENE_H__
#define __SCENE_H__

#include "headers.h"

#include "camera.h"
#include "light.h"
#include "shape.h"
#include "sphere.h"
#include "box.h"
#include "cylinder.h"

#include <set>
#include <list>
#include <cmath>

class Scene {
  typedef std::set<Light*> LightSet;
  typedef std::set<Shape*> ShapeSet;
  
 public:
  Scene(unsigned int N, unsigned int M);
  ~Scene();

  void add_shape( Shape* );
  void add_light( Light* );
  void set_cam( const Camera& cam );

  Color trace_ray( const Ray&, size_t depth=0 );

  Ray pixel_ray( unsigned int i, unsigned int j );
  
 private:
  LightSet _lights;
  ShapeSet _shapes;
  Camera   _cam;

  unsigned int _N, _M;
};

#endif
