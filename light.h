#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "headers.h"

class Light {

 public:
 Light( const Vec3& loc, const Color& col ) : _loc(loc), _c(col) { }

 private:
  Vec3 _loc;
  Color _c;
};

#endif
