#include "scene.h"

bool Scene::trace_ray( Ray& ray, float ior, size_t depth, const Shape* in_shape)
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

  // calculate color
  if( shape ) {
    LightSet visible;
    for( LightSet::const_iterator litr = _lights.begin(); 
	 litr != _lights.end(); litr++ )
      {
	Ray tmp_normal;
	const Light& light = *litr;
	if( ! intersect_all( Ray(ip.from(), (light.loc() - ip.from()).dir()), 
			     tmp_normal ) ) 
	  {
	    visible.push_back( *litr );
	  }
      }

    // calculate reflection
    Ray Refl( ray.reflection(ip) );
    Refl.color() = Color(0,0,0);
    trace_ray( Refl, ior, depth-1, in_shape);

    // calculate refraction
    Ray Refr;
    const double outer_ior = 1.0;  // or is it ???????
    const double post_ior = shape == in_shape ? outer_ior : shape->ior();
    if( shape->pigment().w() > 0 &&
	refraction( ray.dir(), ip.dir(), Refr.dir(), ior, post_ior ) )
      {
	Refr.from() = ip.from() + ZERO * ray.dir() + ZERO * Refr.dir();
	trace_ray(Refr, post_ior, depth-1);
      }
    
    ray.color() = calculate_lighting(ray.dir(), ip, Refl, Refr, shape, visible);
    return true;
  }
  
  return false;
}

const Shape* Scene::intersect_all( const Ray& inc, Ray& nearest_ip ) const {
  const Shape* nearest_s = NULL;
  double nearest_dist;

  for( ShapeSet::iterator i = _shapes.begin(); i != _shapes.end(); i++ )
    {
      const Shape* s = *i;
      Ray ip;               // intersection point
  
      if( s->intersect( inc, ip ) ) {
	if( inc.dir().dot( ip.from() - inc.from()) < ZERO )  continue;
	double ip_dist = (ip.from() - inc.from()).l2();
	if( nearest_s == NULL || ip_dist < nearest_dist - ZERO ) {
	  nearest_s = s;
	  nearest_ip = ip;
	  nearest_dist = ip_dist;
	}
      }
    }
  
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

Scene::Scene(unsigned int N, unsigned int M) : _N(N), _M(M), _global_amb(0,0,0)
{  }

Scene::~Scene() 
{
  for( ShapeSet::iterator i = _shapes.begin(); i != _shapes.end(); i++ )
    delete (*i);
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
