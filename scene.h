#ifndef __SCENE_H__
#define __SCENE_H__

#define DEF_IOR 1.0

#include "headers.h"

#include "camera.h"
#include "light.h"
#include "shape.h"
#include "sphere.h"
#include "box.h"
#include "cylinder.h"
#include "polygon.h"

#include <set>
#include <list>
#include <cmath>

class Scene {
  struct compare_ior {
    bool operator() (const Shape* a, const Shape* b) 
    { if( a->ior() == b->ior() ) return a < b;
      return a->ior() < b->ior(); }
  };

  typedef std::list<Light> LightSet;
  typedef std::set<Shape*> ShapeSet;
  typedef std::set<const Shape*, Scene::compare_ior> RefrStack;
  
 public:
  enum SceneEnum {
    GLOBAL_AMBIENT_R, GLOBAL_AMBIENT_G, GLOBAL_AMBIENT_B,
  };

  Scene(unsigned int N, unsigned int M);
  ~Scene();

  void add_shape( Shape* );
  void add_light( const Light& );
  void set_cam( const Camera& cam );
  void set_pxl_width ( unsigned int w );
  void set_pxl_height( unsigned int h );
  void set_global( SceneEnum param, float val );
  float get_global( SceneEnum param ) const;
  float aspect_ratio(void) const { return _cam.a_ratio(); }

  bool trace_ray( Ray&, size_t depth, 
		  const Shape* going_through = NULL);

  bool refraction( const Vec3& inc, const Vec3& normal, 
		   Vec3& refraction, double ior_from, double ior_to) const;

  Ray pixel_ray( unsigned int i, unsigned int j );

 protected:
  const Shape* intersect_all( const Ray& ray, Ray& intersection ) const;
  Color calculate_lighting( const Vec3& inc_dir,
			    const Ray& normal,
			    const Ray& reflected_ray,
			    const Ray& refracted_ray,
			    const Shape* shape_ptr,
			    const LightSet& visible_lights ) const;

 private:
  //
  // physically present in the scene
  //
  LightSet _lights;
  ShapeSet _shapes;
  Camera   _cam;

  //
  // structure to handle refraction indices of overlapping objects
  //
  RefrStack _R_stk;

  //
  // global parameters of the scene
  //
  Color _global_amb;        //ambient light

  //
  // parameters for the ray tracer
  //
  unsigned int _N, _M;      //pixel width and height

};

#endif
