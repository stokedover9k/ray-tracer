#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "headers.h"
#include "ray.h"

class Camera {
 public:
  Camera( const Vec3& loc     = Vec3(0, 0, 0), 
	  const Vec3& look_at = Vec3(0, 0,-1),
	  const Vec3& up      = Vec3(0, 1, 0), 
	  const Vec3& right   = Vec3(1.33333, 0, 0),
	  float angle         = 45.0 );

  // getters & setters

  const Vec3& loc     (void) const   { return _loc;     }
  const Vec3& look_at (void) const   { return _look_at; }
  const Vec3& up      (void) const   { return _up;      }
  const Vec3& right   (void) const   { return _right;   }
  float       angle   (void) const   { return _angle;   }
  
  Vec3&       loc     (void)         { return _loc;     }
  Vec3&       look_at (void)         { return _look_at; }
  Vec3&       up      (void)         { return _up;      }
  Vec3&       right   (void)         { return _right;   }
  float&      angle   (void)         { return _angle;   }

 private:
  Vec3 _loc, _look_at, _up, _right;
  float _angle;

  void _adjust_up_dir(void);
};

#endif
