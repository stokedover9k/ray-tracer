#include "cylinder.h"

Cylinder::Cylinder( const Vec3& p1, const Vec3& p2, double r, const Shape& s )
  : Shape(s), _p1(p1), _p2(p2), _r(r)
{ 
  const Matrix& TM = Matrix().setTranslation(p1);
  Vec3 p2_trans = p2 - p1;

  Vec3 rot_axis = cross(p2_trans, Vec3(0,0,1));
  double rot_angle = acos( p2_trans.dir().dot(Vec3(0,0,1)) );
  const Matrix& RM = Matrix().setRotation(-rot_angle, rot_axis);
  const Matrix& SM = Matrix().setScale( Vec4(r,r, (p2-p1).l2(), 1) );

  transform()     = transform() * TM * RM * SM;
  inv_transform() = transform().inverse();

  _p1 = Vec3(0,0,0);
  _p2 = Vec3(0,0,1);
  _r = 1;

  /*
  using std::cout;  using std::endl;

  Vec3 a(1,1,1);  cout << "a: " << a << endl;
  Vec3 b(2,3,1);  cout << "b: " << b << endl;

  Matrix Trans = Matrix();
  Matrix Rot   = Matrix();
  Matrix Scale = Matrix();
  
  // translate ------------------
  Trans.setTranslation(a);   
  cout << Trans << endl;

  // rotate ----------------------
  Vec3 b_trans = Trans.inverse() * Vec4(b,1);
  cout << "b trans is equal to: " << b-a << endl;

  Vec3 rot_ax = cross( b_trans, Vec3(0,0,1) );
  double rot_ag = acos( b_trans.dir().dot(Vec3(0,0,1)) );
  Rot.setRotation(-rot_ag, rot_ax);

  cout << "rot_ax: " << rot_ax << endl;
  cout << "rot_ag: " << rot_ag << endl;

  // scale ------------------
  Vec4 scale_factor( 1,1, (b-a).l2(),1 );
  Scale.setScale( scale_factor );

  const Shape& shape( Trans * Rot * Scale );
  cout << "transform: " << shape.transform() << endl;
  cout << "inv trans: " << shape.inv_transform() << endl;
  cout << "check:     " << shape.transform() * shape.inv_transform() << endl;

  cout << endl;
  cout << "origin: " << shape.transform() * Vec4(0,0,0,1) << endl;
  cout << "z-1:    " << shape.transform() * Vec4(0,0,1,1) << endl;
  cout << "a:      " << shape.inv_transform() * Vec4(a,1) << endl;
  cout << "b:      " << shape.inv_transform() * Vec4(b,1) << endl;

  cout << "c: " << shape.inv_transform() * Vec4(0,0,1,1) << endl;
  //*/

  /*
  cout << "p1    : " << inv_transform() * Vec4(-2,-4,-5,1) << endl;
  cout << "p2    : " << inv_transform() * Vec4(-2,-1,-5,1) << std::endl;
  cout << "radius: " << transform() * Vec4((Vec3(0,1,0) * r),1) << std::endl;
  cout << "radius: " << transform() * Vec4((Vec3(1,0,0) * r),1) << std::endl;
  cout << "origin: " << transform() * Vec4(0,0,0,1) << endl;
  cout << "z=1   : " << transform() * Vec4(0,0,1,1) << endl;
  //*/
}

bool intersect_unit_circle( const Ray& inc, Ray& hit )
{
  double Z = inc.origin().z();
  double z = inc.dir().z();

  Vec3 v = inc.dir() * inc.origin().z() / inc.dir().z() - inc.origin();
  v.z() = 0;

  if( v.l2() > 1+ZERO ) return false;
  
  hit.origin() = v;
  return true;
}

bool Cylinder::intersect( const Ray& inc, Ray& hit ) const 
{
  bool inter1 = false;
  bool inter2 = false;
  bool inter3 = false;

  const Ray& r1 = inc.transform( inv_transform() );
  Ray r2(r1);     r2.origin() += Vec3(0,0,-1);
  Ray r3(r1);
  Ray  h1, h2, h3, h4;   //hits

  if( intersect_unit_circle( r1, h1 ) ) {
    h1.dir() = Vec3(0,0,-1);
    inter1 = true;
  }

  if( intersect_unit_circle( r2, h2 ) ) {
    h2.dir() = Vec3(0,0,1);
    h2.origin() += Vec3(0,0,1);
    r2.origin() += Vec3(0,0,1);
    inter2 = true;
  }

  //----- intersect tube -------//

  const Vec3& p = r1.origin();
  const Vec3& v = r1.dir();
  const Vec3 pa(0,0,0);
  const Vec3 va(0,0,1);

  Vec3 pd = p - pa;
  
  double A = (v - v.dot(va) * va).dot();
  double B = 2 * ((v - v.dot(va) * va).dot(pd - pd.dot(va) * va));
  double C = (pd - pd.dot(va) * va).dot() - 1;
  double D = B*B - 4*A*C;
  double t;
  if( D > 0 ) {
    double ttmp1 = (-B + sqrt( D ) ) / 2.0 / A;
    double ttmp2 = (-B - sqrt( D ) ) / 2.0 / A;
    h3.origin() = r1.origin() + ttmp1 * r1.dir();
    h4.origin() = r1.origin() + ttmp2 * r1.dir();
    
    if( h3.origin().z() > 0 && h3.origin().z() < 1 ) {
      //t = ttmp1;
      //hit.origin() = h3.origin();
      //hit.dir() = Vec3(h3.origin().x(), h3.origin().y(), 0);
      //inter3 = true;
    }
    if( h4.origin().z() > 0 && h4.origin().z() < 1 ) {
      if( !inter3 || 
	  (h4.origin() - r1.origin()).l2() < (hit.origin() - r1.origin()).l2() ) {
	t = ttmp2;
	hit.origin() = h4.origin();
	hit.dir() = Vec3(h4.origin().x(), h4.origin().y(), 0);
	inter3 = true;
      }
    }
  }

  //------ check if caps are closer ------//

  double t1 = (h1.origin() - r1.origin()).l2();
  if( inter1 ) {
    if( !inter3 || t1 < t ) {
      hit = h1;
      t = t1;
    }
  }
  if( inter2 ) {
    if( ( !inter1 && !inter2 ) || (h2.origin() - r2.origin()).l2() < t ) {
      hit.origin() = h2.origin();
      hit.dir()    = h2.dir();
    }
  }

  hit = hit.transform( transform() );

  return inter1 || inter2 || inter3;
}

