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
  const Color&   color (void) const   { return _c; }

  Vec3&          origin(void)         { return _o; }
  Vec3&          dir   (void)         { return _d; }
  Color&         color (void)         { return _c; }

  Ray transform( const Matrix& ) const;
  Ray reflection( const Ray& N ) const;
  
 private:
  Vec3 _o, _d;   // origin, direction
  Color _c;      // color
};

#endif
