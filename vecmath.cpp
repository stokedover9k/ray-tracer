#include "vecmath.h"

/* TODO these is just some vector/matrix operations; 
you need a more complete set (addition, multiplication by 
scalar, dot and cross products, matrix - vector product) 
*/

void SetVect(struct Vector* v, double x, double y, double z) { 
  v->x = x;
  v->y = y;
  v->z = z;
}

void SetIdentity4d(struct Matrix4d* m ) { 
  m->val[0] = 1.0;   m->val[1] = 0.0;   m->val[2] = 0.0;   m->val[3] = 0.0;
  m->val[4] = 0.0;   m->val[5] = 1.0;   m->val[6] = 0.0;   m->val[7] = 0.0;
  m->val[8] = 0.0;   m->val[9] = 0.0;   m->val[10] = 1.0;  m->val[11] = 0.0;
  m->val[12] = 0.0;  m->val[13] = 0.0;  m->val[14] = 0.0;  m->val[15] = 1.0;
}


void MultMatrix4d(struct Matrix4d* res, 
                  struct Matrix4d* mat1, 
                  struct Matrix4d* mat2) { 
  struct Matrix4d tmp;
  int i,j,k;

  for (i = 0 ; i < 4 ; i++) {
    for (j = 0 ; j < 4 ; j++) {
      tmp.val[4*j+i] = 0.0;
      for (k = 0 ; k < 4 ; k++){
        tmp.val[4*j+i] +=   mat1->val[4*j+k] * mat2->val[4*k+i];
      }
    }
  }

  for (i = 0 ; i < 4 ; i++) {
    for (j = 0 ; j < 4 ; j++) {
      res->val[4*i+j] = tmp.val[4*i+j];
    }
  }
}

void ScaleMatrix4d(struct Matrix4d* res, struct Vector* scale) {
  struct Matrix4d scale_mat;
  SetIdentity4d(&scale_mat);
  scale_mat.val[0] = scale->x;
  scale_mat.val[4+1] = scale->y;
  scale_mat.val[4*2+2] = scale->z;
  MultMatrix4d(res, &scale_mat, res);
}

void TranslateMatrix4d(struct Matrix4d* res, struct Vector* trans) { 
  struct Matrix4d trans_mat;
  SetIdentity4d(&trans_mat);
  trans_mat.val[0+3] =    trans->x;
  trans_mat.val[4+3] =    trans->y;
  trans_mat.val[4*2+3] =  trans->z;
  MultMatrix4d(res, &trans_mat, res);
}

/* set matrix res to the inverse of src; return 1 if the inverse 
exists, 0 otherwise */


int InverseMatrix4d(struct Matrix4d* res, struct Matrix4d* src ){
  struct Matrix4d a;
  int i, j, k;
  double v;
  double tmp; /* for swaps */
  
  for( i = 0; i < 16; i++ ) 
    a.val[i] = src->val[i]; 
  SetIdentity4d(res);
  
  for (i = 0; i < 4; i++) {
    v = a.val[4*i+i];	 /* Find the new pivot. */
    k = i;
    for (j = i + 1; j < 4; j++)
      if (fabs(a.val[4*j+i]) > fabs(v)) {
        /* Find maximum on col i, row i+1..4 */
        v = a.val[4*j+i];
        k = j;
      }
      j = k;
      
      if (i != j)
        for (k = 0; k < 4; k++) { /* swap  rows */
          tmp = a.val[4*i+k]; a.val[4*i+k] = a.val[4*j+k]; 
          a.val[4*j+k] = tmp;
          tmp = res->val[4*i+k]; res->val[4*i+k] = res->val[4*j+k];
          res->val[4*j+k] = tmp;
        }
        
        for (j = i + 1; j < 4; j++) {	 /* Eliminate col i from row i+1..4. */
          v = a.val[4*j+i] / a.val[4*i+i];
          for (k = 0; k < 4; k++) {
            a.val[4*j+k]    -= v * a.val[4*i+k];
            res->val[4*j+k] -= v * res->val[4*i+k];
          }
        }
  }
  
  for (i = 3; i >= 0; i--) {			       /* Back substitution. */
    if (a.val[4*i+i] == 0)
      return 0;					   /* The matrix is singular. */
    
    for (j = 0; j < i; j++) {	 /* Eliminate col i from row 1..i-1. */
      v = a.val[4*j+i] / a.val[4*i+i];
      for (k = 0; k < 4; k++) {
        res->val[4*j+k] -= v * res->val[4*i+k];
      }
    } 
  } 
  
  for (i = 0; i < 4; i++)		    /* Normalize the inverse matrix. */
    for (j = 0; j < 4; j++)
      res->val[4*i+j] /= a.val[4*i+i];
    return 1;
}

/* Unusual POV-Ray rotations: 
   a vector with 3 components specifies 3 rotations, around x, y and z
   axes resepctively; 
   create three matrices and apply them in the order x,y,z.
*/

void RotateMatrix4d(struct Matrix4d* res, struct Vector* rotate) { 
  struct Matrix4d rotate_mat_x, rotate_mat_y, rotate_mat_z;
  double angle_x = M_PI*rotate->x/180.0;
  double angle_y = M_PI*rotate->y/180.0;
  double angle_z = M_PI*rotate->z/180.0;
  
  SetIdentity4d(&rotate_mat_x);
  SetIdentity4d(&rotate_mat_y);
  SetIdentity4d(&rotate_mat_z);
  
  rotate_mat_x.val[4*1+1] =  cos(angle_x);
  rotate_mat_x.val[4*2+2] =  cos(angle_x);
  rotate_mat_x.val[4*2+1] =  sin(angle_x);
  rotate_mat_x.val[4*1+2] =  -sin(angle_x);

  rotate_mat_y.val[4*0+0] =  cos(angle_y);
  rotate_mat_y.val[4*2+2] =  cos(angle_y);
  rotate_mat_y.val[4*2+0] =  -sin(angle_y);
  rotate_mat_y.val[4*0+2] =  sin(angle_y);

  rotate_mat_z.val[4*0+0] =  cos(angle_z);
  rotate_mat_z.val[4*1+1] =  cos(angle_z);
  rotate_mat_z.val[4*1+0] =  sin(angle_z);
  rotate_mat_z.val[4*0+1] =  -sin(angle_z);

  MultMatrix4d(res,  &rotate_mat_x, res);
  MultMatrix4d(res,  &rotate_mat_y, res);  
  MultMatrix4d(res,  &rotate_mat_z, res);

}

 
/* 
  Printing is done in POV ray format so that the results can be easily fed 
  back into parser, if needed. The output is not the complete 4 by 4 matrix!
  POV ray language matrix statement requires only 12 entries, with the 
  last 4 entries assumed to be 0,0,0,1. 
  to make sure that we somehow did not get a matrix with illegal 
  entries in the last row, we assert that they are what we expect.   
  Moreover, the order of the entries is different from the usual vertex 
  notation: entries are printed column by column, i.e. as 
  a11,a21,a31, a12,a22,a32, a13,a23,a33, a14,a24,a34 
  
*/

void PrintVect(struct Vector v) { 
  printf( "<%.3g, %.3g, %.3g>", v.x,v.y,v.z); 
}

void PrintMatrix4d(struct Matrix4d m ) { 
  int i,j;
  printf("< %.3g",m.val[0]);
  for( j = 0; j < 4; j++) {
    for( i = ( (j==0)?1:0); i < 4; i++)
      printf(",%.3g ",m.val[4*i+j]);
  }
  printf(">");

}
