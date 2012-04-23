#ifndef __PARSE_H__
#define __PARSE_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h> 
#include <list>
#include "vecmath.h"
#include "tokens.h"

#include "scene.h"

namespace SceneParser {
  extern void Parse(FILE* infile, Scene& s);
};

#endif /* __PARSE_H__ */
