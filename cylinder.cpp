#include "cylinder.h"

Cylinder::Cylinder( const Vec3& p1, const Vec3& p2, double r, const Shape& s )
  : Shape(s)
{ 
  const Matrix& TM = Matrix().setTranslation(p1);
  Vec3 p2_trans = p2 - p1;

  Vec3 rot_axis = cross(p2_trans, Vec3(0,0,1));
  double rot_angle = acos( p2_trans.dir().dot(Vec3(0,0,1)) );
  Matrix RM = Matrix();
  if( rot_angle > ZERO ) { RM.setRotation(-rot_angle, rot_axis); }
  const Matrix& SM = Matrix().setScale( Vec4(r,r, (p2-p1).l2(), 1) );

  transform()     = transform() * TM * RM * SM;
  inv_transform() = transform().inverse();
}

bool Cylinder::intersect_unit_circle( const Ray& inc, Ray& hit )
{
  double Z = inc.from().z();
  double z = inc.dir().z();

  Vec3 v = inc.from() - inc.dir() * Z / z;
  v.z() = 0;

  if( v.l2() > 1 ) return false;
  hit.from() = v;
  return true;
}

bool Cylinder::_intersect( const Ray& inc, Ray& hit ) const 
{
  bool inter1 = false;
  bool inter2 = false;
  bool inter3 = false;

  const Vec3& axis = Vec3(0,0,1);

  const Ray& r1 = inc;
  const Ray& r2 = Ray(r1.from() - axis, r1.dir());
  Ray h1, h2, h3;      //hits
  double t1 = DBL_MAX;
  double t2 = DBL_MAX;
  double t3 = DBL_MAX;


  //----- intersect tube -------//

  const Vec3& p = r1.from();
  const Vec3& v = r1.dir();
  const Vec3& pa = Vec3(0,0,0);
  const Vec3& va = Vec3(axis);
  const Vec3& pd = p - pa;
  
  double A = (v - v.dot(va)*va).dot();
  double B = 2 * ((v - v.dot(va) * va).dot(pd - pd.dot(va)*va));
  double C = (pd - pd.dot(va)*va).dot() - 1;
  double D = B*B - 4*A*C;

  if( D > 0 ) {
    double ttmp = (-B - sqrt(D)) / 2.0 / A;
    h3.from() = p + ttmp * v;
        
    if( h3.from().z() > 0 && h3.from().z() < 1 ) {
      t3 = ttmp;
      h3.dir() = Vec3(h3.from().x(), h3.from().y(), 0);
      if( t3 > 0 )   inter3 = true;
    }
  }

  //------ check if caps are closer ------//

  if( intersect_unit_circle( r1, h1 ) ) {
    h1.dir() = -axis;

    t1 = (h1.from() - r1.from()).l2();
    inter1 = true;
  }

  if( intersect_unit_circle( r2, h2 ) ) {
    h2.dir() = axis;
    h2.from() += axis;
    t2 = (h2.from() - r2.from()).l2();

    inter2 = true;
  }

  double min = DBL_MAX-1;

  if( inter1 ) {
    if( t1 < min ) {
      min = t1;
      hit = h1;
    }
  }
  if( inter2 ) {
    if( t2 < min ) {
      min = t2;
      hit = h2;
    }
  }
  if( inter3 ) {
    if( t3 < min ) {
      min = t3;
      hit = h3;
    }
  }

  return inter1 || inter2 || inter3;
}

