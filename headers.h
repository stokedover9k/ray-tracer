#ifndef __HEADERS_H__
#define __HEADERS_H__

#include "cvec2t.h"
#include "cvec3t.h"
#include "cvec4t.h"

#include "hmatrix.h"

#define ZERO 0.00001

typedef CVec3T<double> Vec3;
typedef CVec4T<double> Vec4;

typedef CVec3T<float> Color;
typedef CVec4T<float> Pigment;

typedef HMatrix<double> Matrix;

#endif
