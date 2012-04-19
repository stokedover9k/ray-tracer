#include "cylinder.h"

Cylinder::Cylinder( const Vec3& p1, const Vec3& p2, double r, const Shape& s )
  : Shape(s), _p1(p1), _p2(p2), _r(r)
{ 
  using std::cout;  using std::endl;

  Vec3 a(1,1,1);  cout << "a: " << a << endl;
  Vec3 b(2,3,1);  cout << "b: " << b << endl;

  Matrix Trans = Matrix();
  Matrix Rot   = Matrix();
  Matrix Scale = Matrix();
  
  /***** translate */
  Trans.setTranslation(a);   
  cout << Trans << endl;

  /***** rotate */
  Vec3 b_trans = Trans.inverse() * Vec4(b,1);
  //cout << "b trans: " << b_trans << endl;

  Vec3 rot_ax = cross( b_trans, Vec3(0,0,1) );
  double rot_ag = acos( (b_trans.dir()).dot(Vec3(0,0,1)) );
  Rot.setRotation(-rot_ag, rot_ax);

  cout << "rot_ax: " << rot_ax << endl;
  cout << "rot_ag: " << rot_ag << endl;

  /***** scale */
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

  throw "exit: cylinder - matrix test code";



  std::cout << p2 << " neg: " << -p2 << std::endl;
  const Matrix& TM = Matrix().setTranslation(p1);

  _p2 = _p2 - p1;
  _p2 = _p2.dir();

  Vec3 rotAxis = cross(_p2, Vec3(0,0,1));
  double rotAngle = acos( _p2.dot(Vec3(0,0,1)) );
  const Matrix& RM = Matrix().setRotation(-rotAngle, rotAxis);
  const Matrix& SM = Matrix().setScale( Vec3(r, r, (p2-p1).l2()) );

  const Shape& tmp_s( transform() * TM * RM * SM );
  //*/
  //const Shape& tmp_s( transform() * TM );
  transform()     = tmp_s.    transform();
  inv_transform() = tmp_s.inv_transform();

  _p1 = Vec3(0,0,0);
  _p2 = Vec3(0,0,1);
  _r = 1;

  std::cout << inv_transform() * Vec4(_p1,1) << " --- " 
	    << inv_transform() * Vec4(0,0,0,1) << std::endl;
  std::cout << "radius: " << transform() * Vec4((Vec3(0,1,0) * r),1) << std::endl;
  std::cout << "radius: " << transform() * Vec4((Vec3(1,0,0) * r),1) << std::endl;

  throw "Exiting...";
}

bool Cylinder::intersect( const Ray& inc, Ray& hit ) const 
{
  return false;
}

