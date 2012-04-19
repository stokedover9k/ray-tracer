#include "ray.h"
Ray::Ray( const Vec3& o, const Vec3& d, const Color& c ) : _o(o), _d(d), _c(c)
{  }

Ray Ray::transform( const Matrix& t ) const {
  Vec3 o  = t * Vec4(_o, 1);
  Vec3 to = t * Vec4(_o + _d, 1);
  return Ray( o, (to - o).dir(), _c );
}
