#include "camera.h"

Camera::Camera( const Vec3& l, const Vec3& at, const Vec3& up, const Vec3& r,
		float ang) :
  _loc(l), _look_at(at), _up(up), _right(r), _angle(ang)
{ 
  _adjust_up_dir(); 
}

void Camera::_adjust_up_dir(void) {
  const Vec3& view_dir = _look_at - _loc;
  _up = _up - _up.dot(view_dir) / view_dir.dot() * view_dir;
}

float Camera::a_ratio(void) const {
  return _up.l2() / _right.l2();
}

Vec3 Camera::view_dir(void) const {
  return (_look_at - _loc).dir();
}
