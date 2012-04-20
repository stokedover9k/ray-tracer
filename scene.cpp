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
  
  //Shape& s = **_shapes.begin();     // FIXXX this
  
  float c = 0;
  Ray nearest_ip;
  const Shape* nearest_s = NULL;
  double nearest_dist;

  for( ShapeSet::iterator i = _shapes.begin(); i != _shapes.end(); i++ )
    {
      const Shape* s = *i;
      Ray ip;                           // intersection point
  
      if( s->intersect( ray, ip ) ) {
	double ip_dist = (ip.origin() - ray.origin()).l2();
	if( nearest_s == NULL || ip_dist < nearest_dist ) 
	  {
	    nearest_s = s;
	    nearest_ip = ip;
	    nearest_dist = ip_dist;
	    c = - ip.dir().dot(ray.dir());
	  }
      }
    }

  if( c >= 0 )
    return Color( c, 0, 0 );
  return Color( 0, c, 0 );
}

Scene::Scene(unsigned int N, unsigned int M) : _N(N), _M(M)
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
