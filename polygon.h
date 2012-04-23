#ifndef __POLYGON_H_
#define __POLYGON_H_

#include "shape.h"
#include <list>

class Polygon : public Shape {
  typedef std::list<Vec3> VtxList;
  
 public:
  Polygon( const VtxList& verts = VtxList(), const Shape& s = Shape() );

  Vec3 normal(void) const;

 protected:
  virtual bool _intersect( const Ray& inc, Ray& hit_point ) const;

 private:
  VtxList _verts;
};

#endif
