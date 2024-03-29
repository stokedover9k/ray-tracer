#include "polygon.h"

Polygon::Polygon( const VtxList& verts, const Shape& s ) 
  : Shape(s), _verts(verts.begin(), verts.end())
{ 
  if( verts.size() < 3 ) throw "Polygon::Polygon(const std::list<Vec3>&, "
			   "const Shape&): less than 3 vertices given.";
  if( s.pigment().w() > 0 ) throw "Polygon::Polygon(const std::list<Vec3>&, "
			      "const Shape&): 2D object can't be transparent.";
}

bool Polygon::_intersect( const Ray& inc, Ray& ip ) const {
  VtxList::const_iterator v_itr = _verts.begin();
  Vec3 v1 = *_verts.rbegin();

  // project onto a plane
  size_t X, Y, Z;
  const Vec3& norm = normal();

  double t = - (inc.from() - v1).dot(norm) / inc.dir().dot(norm);
  const Vec3& hit = inc.from() + t * inc.dir();

  X = std::max(norm(0), -norm(0)) < std::max(norm(1), -norm(1)) ? 0 : 1;
  Z = X == 0 ? 1 : 0;
  Y = std::max(norm(Z), -norm(Z)) < std::max(norm(2), -norm(2)) ? Z : 2;

  do {
    const Vec3& v2 = *v_itr++;
    if( cross((v2 - v1), (hit - v1)).dot(norm) <= 0 ) { 
      return false;
    }
    v1 = v2;
  } while( v_itr != _verts.end() );

  ip = Ray(hit, norm.dot(inc.dir()) > 0 ? -norm : norm);
  return true;
}

Vec3 Polygon::normal(void) const {
  VtxList::const_iterator i = _verts.begin();
  const Vec3& a = *i++;
  const Vec3& b = *i++;
  const Vec3& c = *i++;

  return cross((c-b).dir(), (a-b).dir()).dir();
}
