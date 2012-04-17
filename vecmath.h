#ifndef __VECMATH_H__
#define __VECMATH_H__

#include <stdio.h>
#include <math.h>
#include <assert.h>

/* TODO these is just some vector/matrix operations; 
you need a more complete set (addition, multiplication by 
scalar, dot and cross products, matrix - vector product) 
*/

/* windows do not have a constant for Pi */

#ifdef _WIN32 
#define M_PI 3.14159265358979323846
#endif

struct Vector { 
  double x,y,z;
};

/* POV Ray transformation matrices have last 4 entries always 
   equal to 0 0 0 1. We still store these elements, 
   so that standard matrix operations can be performed using
   standard formulas.
*/

struct Matrix4d { 
  double val[16];
};

extern void SetVect(struct Vector* v, double x, double y, double z);
extern void PrintVect(struct Vector v);

extern void SetIdentity4d(struct Matrix4d* m );
/* compute res = mat1 * mat2 */
extern void MultMatrix4d(struct Matrix4d* res, 
                         struct Matrix4d* mat1, 
                         struct Matrix4d* mat2);

/* res = inverse(src), returns 0 if src singular */
extern int InverseMatrix4d(struct Matrix4d* res, struct Matrix4d* src );
/* res = scaleMat*res */
extern void ScaleMatrix4d(    struct Matrix4d* res, struct Vector* scale);
/* res = transMat*res */
extern void TranslateMatrix4d(struct Matrix4d* res, struct Vector* trans);
/* res = rotateMat*res; rotation matrix is computed using POV ray rules*/
extern void RotateMatrix4d(   struct Matrix4d* res, struct Vector* rotate);
extern void PrintMatrix4d(struct Matrix4d m );


#endif /* __VECMATH_H__ */







