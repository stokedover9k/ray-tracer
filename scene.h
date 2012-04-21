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
  enum SceneEnum {
    GLOBAL_AMBIENT_R, GLOBAL_AMBIENT_G, GLOBAL_AMBIENT_B,
  };

  Scene(unsigned int N, unsigned int M);
  ~Scene();

  void add_shape( Shape* );
  void add_light( Light* );
  void set_cam( const Camera& cam );
  void set_global( SceneEnum param, float val );
  float get_global( SceneEnum param ) const;

  bool trace_ray( const Ray&, Color& color, size_t depth=0 );

  Ray pixel_ray( unsigned int i, unsigned int j );

 protected:
  const Shape* intersect_all( const Ray& ray, Ray& intersection ) const;
  Color calculate_lighting( const Vec3& inc_dir,
			    const Ray& normal,
			    const Vec3& reflect_dir,
			    const Shape* shape_ptr,
			    const LightSet& visible_lights ) const;

 private:
  LightSet _lights;
  ShapeSet _shapes;
  Camera   _cam;

  Color _global_amb; //ambient light in the scene

  unsigned int _N, _M;
};

#endif
