#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "headers.h"

class Light {

 public:
 Light( const Vec3& loc, const Color& col ) : _loc(loc), _c(col) { }

  const Color& color(void) const { return _c;   }
  const Vec3&  loc  (void) const { return _loc; }

 private:
  Vec3 _loc;
  Color _c;
};

#endif
