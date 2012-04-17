#ifndef __RAH_H__
#define __RAH_H__

#include "headers.h"

class Ray {

 public:
  Ray( const Vec3&   origin = Vec3(0,0,0),
       const Vec3&   dir    = Vec3(0,0,0),
       const Color&  color  = Color(0,0,0) );

  const Vec3&    origin(void) const   { return _o; }
  const Vec3&    dir   (void) const   { return _d; }
  const Pigment& color (void) const   { return _c; }

  Vec3&          origin(void)         { return _o; }
  Vec3&          dir   (void)         { return _d; }
  Pigment&       color (void)         { return _c; }
  
 private:
  Vec3 _o, _d;   // origin, direction
  Pigment _c;    // color
};

#endif
