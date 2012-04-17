#ifndef __SCENE_H__
#define __SCENE_H__

#include "headers.h"

#include "camera.h"
#include "light.h"
#include "shape.h"

#include <set>

class Scene {
  typedef std::set<Light*> LightSet;
  typedef std::set<Shape*> ShapeSet;
  
 public:
  Scene();
  ~Scene();

  void add_shape( Shape* );
  void add_light( Light* );
  void set_cam( const Camera& cam );
  
 private:
  LightSet _lights;
  ShapeSet _shapes;
  Camera   _cam;
};

#endif
