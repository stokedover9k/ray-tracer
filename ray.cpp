#include "ray.h"
Ray::Ray( const Vec3& o, const Vec3& d, const Color& c ) : _o(o), _d(d), _c(c)
{  }

Ray Ray::transform( const Matrix& t ) const {
  Vec3 o  = t * Vec4(_o, 1);
  Vec3 d = t * Vec4(_d, 0);
  return Ray( o, d.dir(), _c );
}

Ray Ray::reflection( const Ray& N ) const {
  return Ray( N.origin(),
	      (dir() - 2 * N.dir() * ( dir().dot(N.dir()) )).dir() );
}
