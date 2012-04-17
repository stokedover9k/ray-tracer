#include "shape.h"

Finish::Finish( float ambient_, 
		       float diffuse_,
		       float phong_,
		       float phong_size_,
		       float metallic_,
		       float reflection_ )
  : ambient(ambient_), diffuse(diffuse_), phong(phong_), phong_size(phong_size_), 
    metallic(metallic_), reflection(reflection_) 
{  }

