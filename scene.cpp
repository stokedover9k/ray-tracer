#include "scene.h"

//
// constructor
//
Scene::Scene()
  : _N(0), _M(0), _global_amb(0,0,0), _R_stk()
{  }

//
// destructor
//
Scene::~Scene() 
{
  for( ShapeSet::iterator i = _shapes.begin(); i != _shapes.end(); i++ )
    delete (*i);
}

//
// trace ray
//
bool Scene::trace_ray( Ray& ray, size_t depth, const Shape* in_shape)
{
  if( depth == 0 ) return false;
  //---------------------------------------------------------------------------
  // find nearest intersection

  // for each visible light, compute compute RGB
  
  // trace reflected
  // trace refracted

  // compute resulting RGB val with lighting model
  //---------------------------------------------------------------------------
  
  // find nearest intersection
  float c = 0;
  Ray ip;
  const Shape* shape = NULL;
  double nearest_dist;

  shape = intersect_all( ray, ip );
  if( !shape )   return false;

  ip.from() += ZERO * ip.dir();  // move intersection in front of the surface

  // calculate color
  LightSet visible;
  for( LightSet::const_iterator litr = _lights.begin(); 
       litr != _lights.end(); litr++ )
    {
      Ray tmp_normal;
      const Light& light = *litr;
      if(!intersect_all(Ray(ip.from(), (light.loc()-ip.from()).dir()), tmp_normal)
	 || (tmp_normal.from()-ip.from()).l2() > (light.loc()-ip.from()).l2() ) 
	{
	  visible.push_back( *litr );
	}
    }

  // calculate reflection
  Ray Refl( ray.reflection(ip) );
  Refl.color() = Color(0,0,0);
  trace_ray( Refl, depth-1, in_shape);

  // calculate refraction
  Ray Refr;
  if( shape->pigment().w() > 0 )   // if hit shape is transparent
    {
      const Shape* enter_shape = NULL;
      const Shape* exit_shape  = NULL;

      if( _R_stk.erase(shape) )
	exit_shape  = shape;
      else {
	enter_shape = shape;
	_R_stk.insert(shape);
      }

      // if no TOTAL INTERNAL REFRACTION
      if ( refraction( ray.dir(), ip.dir(), Refr.dir(), 
		       in_shape ? in_shape->ior() : DEF_IOR, 
		       _R_stk.size() > 0 ? (*_R_stk.rbegin())->ior() : DEF_IOR ) )
	{
	  // move ray position inside the object (past the surface)
	  Refr.from() = ip.from() - ZERO * ip.dir() + ZERO * Refr.dir();
	  trace_ray(Refr, depth-1, _R_stk.size() > 0 ? *_R_stk.rbegin() : NULL);
	}

      if( exit_shape  ) _R_stk.insert(exit_shape);
      if( enter_shape ) _R_stk.erase(enter_shape);
    }
    
  ray.color() = calculate_lighting(ray.dir(), ip, Refl, Refr, shape, visible);
  return true;
}

//
// intersect all objects
//
const Shape* Scene::intersect_all( const Ray& inc, Ray& nearest_ip ) const {
  const Shape* nearest_s = NULL;
  double nearest_dist;

  Ray ip;    // tmp intersection point
  for( ShapeSet::const_iterator i = _shapes.begin(); i != _shapes.end(); i++ )
    {
      const Shape* s = *i;
  
      if( s->intersect( inc, ip ) ) {
	// check for object "in the same place" or really close
	if( inc.dir().dot( ip.from() - inc.from()) < ZERO )  continue;
	double ip_dist = (ip.from() - inc.from()).l2();
	if( nearest_s == NULL || ip_dist < nearest_dist - ZERO ) {
	  nearest_s = s;
	  nearest_ip = ip;
	  nearest_dist = ip_dist;
	}
      }
    }
  
  nearest_ip.from();
  return nearest_s;
}

Color Scene::calculate_lighting( const Vec3& V, const Ray& N, 
				 const Ray& Refl, const Ray& Refr,
				 const Shape* shape_p, 
				 const LightSet& visible ) const {

  const Finish&  fin = (shape_p)->finish();
  const Pigment& pgm = (shape_p)->pigment();

  Color amb(0,0,0);
  Color trans(0,0,0);
  for( size_t C = 0; C < 3; C++ ) {
    amb(C) = (1 - pgm.w()) * fin.ambient * pgm(C) * _global_amb(C);
    trans(C) += pgm.w() * pgm(C) * Refr.color()(C);
  }

  Color col_i(0,0,0);
  for( LightSet::const_iterator l_itr = visible.begin(); 
       l_itr != visible.end(); l_itr++ ) {
    const Light& light = *l_itr;
  
    const Vec3& L = (light.loc() - N.from()).dir();
    double scale = 1.0 - pgm.w();
    double LdotN = L.dot(N.dir());   if( LdotN < ZERO ) LdotN = 0;
    double LdotR = L.dot(Refl.dir().dir());
    double phong = LdotR > ZERO ? pow( LdotR, fin.phong_size ) : 0;
    const Color& Cspec = fin.metallic > ZERO ? pgm : light.color();

    for( size_t C=0; C<3; C++ ) {
      col_i(C) += (scale * light.color()(C) * (fin.diffuse * pgm(C) * LdotN 
					       + fin.phong * Cspec(C) * phong) );
    }
  }
  
  // calculate using reflected and refracted rays' colors
  Color refl_refr(0,0,0);  
  refl_refr += Refl.color() * fin.reflection;
  refl_refr += trans;

  return (amb + col_i + refl_refr ).min(Color(1,1,1));
}

void Scene::add_shape( Shape* s )  { _shapes.insert(s); }
void Scene::add_light( const Light& l )  { _lights.push_back(l); }
void Scene::set_cam( const Camera& cam )  { _cam = cam; }

void Scene::set_pxl_width ( unsigned int w )  { _N = w; }
void Scene::set_pxl_height( unsigned int h )  { _M = h; }


void Scene::set_global( SceneEnum param, float val ) {
  switch( param )
    {
    case GLOBAL_AMBIENT_R:   _global_amb.x() = val;  break;
    case GLOBAL_AMBIENT_G:   _global_amb.y() = val;  break;
    case GLOBAL_AMBIENT_B:   _global_amb.z() = val;  break;
    default:
      throw "Scene::set_global(Scene::SceneEnum, float): invalid field requested";
    };
}

float Scene::get_global( SceneEnum param ) const {
  switch( param )
    {
    case GLOBAL_AMBIENT_R:  return _global_amb.x();
    case GLOBAL_AMBIENT_G:  return _global_amb.y();
    case GLOBAL_AMBIENT_B:  return _global_amb.z();
    default:
      throw "Scene::get_global(Scene::SceneEnum): invalid field requested";
    };
}


Ray Scene::pixel_ray( unsigned int i, unsigned int j ) 
{
  double n = 1.0;
  float a = _cam.angle();

  double w = n * tan( a/2 );
  double h = _cam.a_ratio() * w;

  double w_disp = (0.5 + i) * 2*w/_N - w;
  double h_disp = (0.5 + j) * 2*h/_M - h;

  Vec3 img_center( _cam.loc() + n * _cam.view_dir() );
  Vec3 pxl_center( h_disp * _cam.up() 
		   + w_disp * cross(_cam.view_dir(), _cam.up()).dir() 
		   + _cam.view_dir() * n );

  return Ray( _cam.loc(), (pxl_center.dir()) );
}

bool Scene::refraction( const Vec3& V, const Vec3& N, 
			Vec3& T, double ior1, double ior2 ) const {
  const double n = ior1 / ior2;
  const double vdotn = -V.dot(N);
  const double D = n * n * (1.0 - vdotn*vdotn);
  if( D > 1.0 ) return false;

  T = (n * V + (n * vdotn - sqrt( 1.0 - D)) * N).dir();
  return true;
}

