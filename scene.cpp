#include "scene.h"

Color Scene::trace_ray( const Ray& ray, size_t depth )
{
  //---------------------------------------------------------------------------
  // find nearest intersection

  // for each visible light, compute compute RGB
  
  // trace reflected
  // trace refracted

  // compute resulting RGB val with lighting model
  //---------------------------------------------------------------------------
  
  // find nearest intersection
  float c = 0;
  Ray nearest_ip;
  //Ray nearest_ray;
  const Shape* nearest_s = NULL;
  double nearest_dist;

  for( ShapeSet::iterator i = _shapes.begin(); i != _shapes.end(); i++ )
    {
      const Shape* s = *i;
      Ray ip;                           // intersection point
  
      if( s->intersect( ray, ip ) ) {
	double ip_dist = (ip.from() - ray.from()).l2();
	if( nearest_s == NULL || ip_dist < nearest_dist ) 
	  {
	    nearest_s = s;
	    nearest_ip = ip;
	    nearest_dist = ip_dist;
	    //c = - ip.dir().dot(ray.dir());
	  }
      }
    }

  // calculate color
  if( nearest_s )
    {
      Color color = calculate_lighting(ray.dir(), nearest_ip, 
				       ray.reflection(nearest_ip).dir(),
				       nearest_s);
      return color;
    }
  
  return Color( 0, -c, 0 );
}

Color Scene::calculate_lighting( const Vec3& V, const Ray& N, const Vec3& R,
				 const Shape* shape_p ) const {
  const Finish&  fin = (shape_p)->finish();
  const Pigment& pgm = (shape_p)->pigment();
  
  Color amb(0,0,0);
  for( size_t C = 0; C < 3; C++ ) {
    amb(C) = (1 - pgm.w()) * fin.ambient * pgm(C) * _global_amb(C);
  }
  //std::cout << amb << std::endl;

  Color col_i(0,0,0);
  for( LightSet::const_iterator l_itr = _lights.begin(); 
       l_itr != _lights.end(); l_itr++ ) {
    const Light& light = **l_itr;
  
    const Vec3& L = (light.loc() - N.from()).dir();
    double LdotN = L.dot(N.dir());
    double LdotR = L.dot(-R.dir());

    for( size_t C=0; C<3; C++ ) {
      col_i(C) 
	+= (
	    (1 - pgm.w()) 
	    *
	    light.color()(C) 
	    * 
	    (fin.diffuse 
	     * pgm(C) 
	     * (LdotN > ZERO ? LdotN : 0)
	     + fin.phong 
	     * ( fin.metallic > ZERO ? pgm(C) : light.color()(C) )
	     * pow( (LdotR > ZERO ? LdotR : 0) , fin.phong_size ) 
	     )
	    );
    }
  }
  
  // calculate using reflected and refracted rays' colors
  Color refl_refr(0,0,0);  // FIXXX - calculation missing

  //return (amb + col_i + refl_refr).max(Color(1,1,1));
  return (amb + col_i ).min(Color(1,1,1));
}

Scene::Scene(unsigned int N, unsigned int M) : _N(N), _M(M), _global_amb(0,0,0)
{  }

Scene::~Scene() 
{
  for( LightSet::iterator i = _lights.begin(); i != _lights.end(); i++ )
    delete (*i);
  for( ShapeSet::iterator i = _shapes.begin(); i != _shapes.end(); i++ )
    delete (*i);
}

void Scene::add_shape( Shape* s )  { _shapes.insert(s); }
void Scene::add_light( Light* l )  { _lights.insert(l); }
void Scene::set_cam( const Camera& cam )  { _cam = cam; }

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
