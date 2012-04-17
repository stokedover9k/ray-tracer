#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

/* A token is one of special symbols
{ } < > , (comma) 
a real number or a reserved word 
 */

enum TokenIDs { 
  T_DOUBLE,
  T_COMMA,

  T_LEFT_ANGLE,
  T_RIGHT_ANGLE,
  T_LEFT_CURLY,
  T_RIGHT_CURLY,

  T_ROTATE,
  T_TRANSLATE,
  T_SCALE,
  T_MATRIX,

  T_POLYGON,
  T_SPHERE,
  T_BOX,
  T_CYLINDER,
  T_CONE,
  T_QUADRIC,

  T_CAMERA,
  T_LOCATION,
  T_RIGHT,
  T_UP,
  
  T_LOOK_AT,
  T_ANGLE,

  T_GLOBAL_SETTINGS,
  T_AMBIENT_LIGHT,
  T_LIGHT_SOURCE,
  T_FINISH,
  T_PIGMENT,
  T_COLOR,
  T_RGB,
  T_RGBF,
  T_REFLECTION,
  T_AMBIENT,
  T_DIFFUSE,
  T_PHONG,
  T_METALLIC,
  T_PHONG_SIZE,
  T_INTERIOR,
  T_IOR,

  T_NULL,
  T_EOF,

  T_LAST
};

struct Token_Struct { 
  enum TokenIDs id; 
  double double_value; /* has meaning only if id = T_DOUBLE */
  int unget_flag;
  FILE* infile;
  int lineNumber;
};

extern const char* TokenNames[];
extern struct Token_Struct Token;

extern void Error(const char* error_msg);

extern void InitializeToken(FILE* infile);

extern void GetToken();
extern void UngetToken();


#endif /* __TOKENS_H__ */





