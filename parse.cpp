#include "parse.h"

/* Functions in this file implement parsing operations for 
   each object type. The main function to be called is Parse().
   Each parsing function assigns values retrieved from the file 
   to a set of local variables that can be used to create 
   objects. Parsing stops at the first error and the program 
   exits through function Error() defined in tokens.c, after
   printing the current line number and an error message. 
   (A real parser would have better error recovery, but this 
   simple approach greatly simplifies the code and is sufficient
   for our purposes).

   Although this code runs, it requires additions to make it fully 
   functional. Search for comments starting with TODO to find 
   places requiring modification.
*/

namespace SceneParser {

  struct Color { 
    double r,g,b,f;
  };


  struct Pigment { 
    struct Color color;
  };

  struct Finish { 
    double ambient;
    double diffuse;
    double phong;
    double phong_size;
    double reflection;
    int metallic;
  };

  struct Interior { 
    double ior;
  };

  struct ModifierStruct { 
    struct Matrix4d transform;
    struct Pigment  pigment;
    struct Finish   finish;
    struct Interior interior;
  };

  /* a collection of functions for syntax verification */

  void ParseLeftAngle() { 
    GetToken();
    if(Token.id != T_LEFT_ANGLE ) Error("Expected <");
  }

  void ParseRightAngle() { 
    GetToken();
    if(Token.id != T_RIGHT_ANGLE ) Error("Expected >");  
  }

  double ParseDouble() { 
    GetToken();
    if(Token.id != T_DOUBLE ) Error("Expected a number");
    return Token.double_value;
  }

  void ParseLeftCurly() { 
    GetToken();
    if(Token.id != T_LEFT_CURLY ) Error("Expected {");
  }

  void ParseRightCurly() { 
    GetToken();
    if(Token.id != T_RIGHT_CURLY ) Error("Expected }");
  }

  void ParseComma() { 
    GetToken();
    if(Token.id != T_COMMA ) Error("Expected ,");
  }

  void ParseVector(struct Vector* v) { 
    ParseLeftAngle();
    v->x = ParseDouble();
    ParseComma();
    v->y = ParseDouble();
    ParseComma();
    v->z = ParseDouble();
    ParseRightAngle();
  }

  void ParseRGBFColor(struct Color* c) { 
    ParseLeftAngle();
    c->r = ParseDouble();
    ParseComma();
    c->g = ParseDouble();
    ParseComma();
    c->b = ParseDouble();
    ParseComma();
    c->f = ParseDouble();
    ParseRightAngle();
  }

  void ParseRGBColor(struct Color* c) { 
    ParseLeftAngle();
    c->r = ParseDouble();
    ParseComma();
    c->g = ParseDouble();
    ParseComma();
    c->b = ParseDouble();
    c->f = 0.0;
    ParseRightAngle();
  }

  void ParseColor(struct Color* c) { 
    GetToken();
    if(Token.id == T_RGB) 
      ParseRGBColor(c);
    else if ( Token.id == T_RGBF )
      ParseRGBFColor(c);
    else Error("Expected rgb or rgbf");
  }

  void PrintColor(struct Color* c) { 
    printf("rgbf <%.3g,%.3g,%.3g,%.3g>", c->r, c->g, c->b, c->f);
  }


  void ParseMatrix( struct Matrix4d* m) { 
    SetIdentity4d(m);
    ParseLeftAngle();
    m->val[4*0] = ParseDouble(); ParseComma(); 
    m->val[4*1] = ParseDouble(); ParseComma();
    m->val[4*2] = ParseDouble(); ParseComma();

    m->val[4*0+1] = ParseDouble(); ParseComma(); 
    m->val[4*1+1] = ParseDouble(); ParseComma();
    m->val[4*2+1] = ParseDouble(); ParseComma();

    m->val[4*0+2] = ParseDouble(); ParseComma(); 
    m->val[4*1+2] = ParseDouble(); ParseComma();
    m->val[4*2+2] = ParseDouble(); ParseComma();

    m->val[4*0+3] = ParseDouble(); ParseComma(); 
    m->val[4*1+3] = ParseDouble(); ParseComma();
    m->val[4*2+3] = ParseDouble(); 
    ParseRightAngle();
  }


  void ParseTransform(struct Matrix4d* transf) { 
    /* if there is nothing to parse, this is not our problem: 
       this should be handled by the caller */
    struct Vector v;
    struct Matrix4d m;
    while(1) { 
      GetToken();
      switch( Token.id ) { 
      case T_SCALE: 
	ParseVector(&v);
	ScaleMatrix4d(transf, &v);
	break;
      case T_ROTATE:
	ParseVector(&v);
	RotateMatrix4d(transf, &v);
	break;
      case T_TRANSLATE:
	ParseVector(&v);
	TranslateMatrix4d(transf, &v);
	break;
	/* once we run into an unknown token, we assume there are no 
	   more  transforms to parse and we return to caller */
      case T_MATRIX:
	ParseMatrix(&m);
	MultMatrix4d(transf, &m, transf); 
	break;
      default: UngetToken(); return; 
      }    
    }
  }

  void ParsePigment(   struct Pigment* pigment ) { 
    ParseLeftCurly();
    while(1) { 
      GetToken();
      if( Token.id == T_COLOR)
	ParseColor(&(pigment->color));
      else if( Token.id == T_RIGHT_CURLY) return;
      else Error("error parsing pigment: unexpected token");
    }    
  }

  void PrintPigment( struct Pigment* pigment ) {
    printf("\tpigment { color ");
    PrintColor(&(pigment->color));
    printf("}");
  }

  void ParseFinish( struct Finish* finish ) { 
    finish->ambient = 0.0; 
    finish->diffuse = 0.0; 
    finish->phong = 0.0; 
    finish->phong_size  = 0.0; 
    finish->reflection= 0.0; 
    finish->metallic= 0.0; 

    ParseLeftCurly();
    while(1) { 
      GetToken();
      switch(Token.id) {
      case T_AMBIENT:
	finish->ambient = ParseDouble();
	break;
      case T_DIFFUSE:
	finish->diffuse = ParseDouble();
	break;
      case T_PHONG:
	finish->phong = ParseDouble();
	break;
      case T_PHONG_SIZE:
	finish->phong_size = ParseDouble();
	break;
      case T_REFLECTION:
	finish->reflection = ParseDouble();
	break;
      case T_METALLIC:
	finish->metallic = ( ParseDouble()!= 0.0 ? 1: 0);
	break;
      case T_RIGHT_CURLY: return;
      default: Error("Error parsing finish: unexpected token");
      }   
    } 
  }

  void PrintFinish( struct Finish* finish ) { 
    printf("\tfinish { ambient %.3g diffuse %.3g phong %.3g phong_size %.3g reflection %.3g metallic %d }\n", 
	   finish->ambient, finish->diffuse, 
	   finish->phong, finish->phong_size, 
	   finish->reflection, finish->metallic);
  }

  void ParseInterior( struct Interior* interior) { 
    ParseLeftCurly();
    while(1) {
      GetToken();
      if( Token.id == T_RIGHT_CURLY) return;
      else if (Token.id == T_IOR) { 
	interior->ior = ParseDouble();
      } else Error("Error parsing interior: unexpected token\n");
    }
  }


  void InitModifiers(struct ModifierStruct* modifiers) { 

    SetIdentity4d(&(modifiers->transform));

    modifiers->pigment.color.r = 0;
    modifiers->pigment.color.g = 0;
    modifiers->pigment.color.b = 0;
    modifiers->pigment.color.f = 0;

    modifiers->finish.ambient    = 0.1;
    modifiers->finish.diffuse    = 0.6;
    modifiers->finish.phong      = 0.0;
    modifiers->finish.phong_size = 40.0;
    modifiers->finish.reflection = 0;

    modifiers->interior.ior = 1.0; 
  }


  void ParseModifiers(struct ModifierStruct* modifiers) { 
    while(1) { 
      GetToken();
      switch(Token.id) { 
      case  T_SCALE:
      case T_ROTATE:
      case T_TRANSLATE:
      case T_MATRIX: 
	UngetToken();
	ParseTransform(&(modifiers->transform));
	break;
      case T_PIGMENT:
	ParsePigment(&(modifiers->pigment));
	break;
      case T_FINISH:
	ParseFinish(&(modifiers->finish));
	break;
      case T_INTERIOR:
	ParseInterior(&(modifiers->interior));
	break;      
      default: UngetToken(); return;
      }
    }
  }

  void PrintModifiers(struct ModifierStruct* modifiers) {
    printf("\tmatrix "); PrintMatrix4d(modifiers->transform);
    printf("\n"); 
    PrintPigment(&(modifiers->pigment));
    printf("\n"); 
    PrintFinish(&(modifiers->finish));
    printf("\tinterior { ior %.3g }\n", modifiers->interior.ior);
  }

  Shape MakeShape( struct ModifierStruct* modifiers ) {
    Color&    p = modifiers->pigment.color;
    Matrix4d& t = modifiers->transform;
    Finish&   f = modifiers->finish;

    return Shape( ::Matrix( t.val ).transpose(),
		  ::Pigment(p.r, p.g, p.b, p.f),
		  ::Finish( f.ambient, f.diffuse, f.phong, f.phong_size,
			    f.metallic, f.reflection ),
		  modifiers->interior.ior
		  );
  }


  void ParseCamera(Scene& scene) { 
    /* these are variables where we store the information about the
       camera; they can be used in the end of the function to 
       assign fields in the camera object */

    struct Vector location, right, up, look_at; 
    double angle;
    struct Matrix4d  transform; 
    int done = FALSE;

    SetIdentity4d(&transform);

    /* default values */
    SetVect(&location, 0.0, 0.0,0.0);   bool loc_b   = false;  //set to "true"
    SetVect(&look_at, 0.0, 0.0,1.0);    bool look_b  = false;  //  if new value
    SetVect(&right, 1.0, 0.0, 0.0);     bool right_b = false;  //  (non-default)
    SetVect(&up, 0.0,1.0,0.0);          bool up_b    = false;  //  found
    angle = 60.0*M_PI/180.0;            bool angle_b = false;

    ParseLeftCurly();

    /* parse camera parameters */  
    while(!done) {     
      GetToken();
      switch(Token.id) { 
      case T_LOCATION:  ParseVector(&location);           loc_b   = true;  break;
      case T_RIGHT:     ParseVector(&right);              right_b = true;  break;
      case T_UP:        ParseVector(&up);                 up_b    = true;  break;
      case T_LOOK_AT:   ParseVector(&look_at);            look_b  = true;  break;
      case T_ANGLE:     angle = M_PI*ParseDouble()/180.0; angle_b = true;  break;
      default: done = TRUE; UngetToken(); break;
      }
    }

    ParseTransform(&transform);
    ParseRightCurly();

    Camera c;
    if( loc_b   )  c.loc()     = Vec3(location.x, location.y, location.z);
    if( right_b )  c.right()   = Vec3(right.x, right.y, right.z);
    if( up_b    )  c.up()      = Vec3(up.x, up.y, up.z);
    if( look_b  )  c.look_at() = Vec3(look_at.x, look_at.y, look_at.z);
    if( angle_b )  c.angle()   = angle*180.0/M_PI;
    scene.set_cam( c );
  
  }

  void ParsePolygon(Scene& scene) { 
    /* these three variables store information about the polygon */
    int num_vertices;
    struct Vector* vertices;
    int vert_cnt;
    int done = FALSE;
    int i;
    struct ModifierStruct modifiers;
    InitModifiers(&modifiers);


    num_vertices = 0; 
    vertices = 0;

    ParseLeftCurly();
    num_vertices = (int)ParseDouble();

    if( num_vertices < 3 ) Error("Polygon must have at least 3 vertices");

    vertices = (struct Vector*)malloc( sizeof(struct Vector)*num_vertices);
    ParseComma();

    std::list<Vec3> vertex_list = std::list<Vec3>();
    for( vert_cnt = 0; vert_cnt < num_vertices; vert_cnt++) { 
      ParseVector(&(vertices[vert_cnt]));
      Vec3 v(vertices[vert_cnt].x, vertices[vert_cnt].y, vertices[vert_cnt].z);
      vertex_list.push_back(v);
      if( vert_cnt < num_vertices-1 ) ParseComma();
    }
    ParseModifiers(&modifiers);
    ParseRightCurly();

    scene.add_shape( new Polygon( vertex_list, MakeShape( &modifiers ) ) );
  }

  void ParseSphere(Scene& scene) { 
    struct Vector center; 
    double radius; 
    struct ModifierStruct modifiers;

    InitModifiers(&modifiers);
    SetVect(&center, 0,0,0);
    radius = 1.0;

    ParseLeftCurly();
    ParseVector(&center);
    ParseComma();
    radius = ParseDouble();

    ParseModifiers(&modifiers);
    ParseRightCurly();

    scene.add_shape( new Sphere( Vec3(center.x, center.y, center.z),
				 radius,
				 MakeShape( &modifiers ) ) );
  }

  void ParseBox(Scene& scene) { 
    struct Vector corner1, corner2; 
    struct ModifierStruct modifiers;
    InitModifiers(&modifiers);


    SetVect(&corner1, 0,0,0);
    SetVect(&corner2, 0,0,0);

    ParseLeftCurly();
    ParseVector(&corner1);
    ParseComma();
    ParseVector(&corner2);
    ParseModifiers(&modifiers);
    ParseRightCurly();
 
    scene.add_shape( new Box( Vec3(corner1.x, corner1.y, corner1.z),
			      Vec3(corner2.x, corner2.y, corner2.z),
			      MakeShape( &modifiers ) ) );
  }

  void ParseCylinder(Scene& scene) { 
    struct Vector base_point , cap_point ;
    double radius; 
    struct ModifierStruct modifiers;
    InitModifiers(&modifiers);

    SetVect(&base_point, 0,0,0);
    SetVect(&cap_point, 0,0,0);
    radius = 0;

    ParseLeftCurly();
    ParseVector(&base_point);
    ParseComma();
    ParseVector(&cap_point);
    ParseComma();
    radius = ParseDouble();
    ParseModifiers(&modifiers);
    ParseRightCurly();
 
    scene.add_shape( new Cylinder( Vec3(base_point.x, base_point.y, base_point.z),
				   Vec3(cap_point.x, cap_point.y, cap_point.z),
				   radius,
				   MakeShape( &modifiers) ) );
  }

  void  ParseCone() { 
    struct Vector base_point, cap_point;
    double base_radius, cap_radius; 
    struct ModifierStruct modifiers;
    InitModifiers(&modifiers);

    SetVect(&base_point, 0,0,0);
    SetVect(&cap_point, 0,0,0);
    base_radius = 0;
    cap_radius = 0;

    ParseLeftCurly();
    ParseVector(&base_point);
    ParseComma();
    base_radius = ParseDouble();
    ParseComma();
    ParseVector(&cap_point);
    ParseComma();  
    cap_radius = ParseDouble();
    ParseModifiers(&modifiers);
    ParseRightCurly();

    /* TODO: assignment to the cone object fields should happen here;
       for now, we just print the values */

    printf("cone {\n\t");
    PrintVect(base_point); printf(", %.3g, ", base_radius); 
    PrintVect(cap_point); printf(", %.3g\n",  cap_radius); 
    PrintModifiers(&modifiers);
    printf("\n}\n");
  }

  void ParseQuadric() { 
    struct Vector ABC, DEF, GHI; 
    double J;
    struct ModifierStruct modifiers;
    InitModifiers(&modifiers);

    SetVect(&ABC, 0,0,0);
    SetVect(&DEF, 0,0,0);
    SetVect(&GHI, 0,0,0);
    J = 0;

    ParseLeftCurly();
    ParseVector(&ABC);
    ParseComma();
    ParseVector(&DEF);
    ParseComma();
    ParseVector(&GHI);
    ParseComma();
    J = ParseDouble();
    ParseModifiers(&modifiers);
    ParseRightCurly();
  
    printf("quadric {\n\t");
    PrintVect(ABC); printf(", "); 
    PrintVect(DEF); printf(", "); 
    PrintVect(GHI); printf(", %.3g\n", J);   
    PrintModifiers(&modifiers);
    printf("\n}\n");

  }

  void ParseLightSource(Scene& scene) { 
    struct Color c;
    struct Vector pos;
    c.r = 0; c.g = 0; c.b = 0; c.f =0; 
    pos.x = 0; pos.y = 0; pos.z = 0;
    ParseLeftCurly();
    ParseVector(&pos);
    GetToken();
    if(Token.id != T_COLOR) Error("Error parsing light source: missing color");
    ParseColor(&c);
    ParseRightCurly();
    
    scene.add_light( Light(Vec3(pos.x, pos.y, pos.z), 
			   ::Color((float)c.r, (float)c.g, (float)c.b)));
  } 

  void ParseGlobalSettings(Scene& scene) { 
    struct Color ambient;
    ambient.r = 0; ambient.g = 0; ambient.b = 0; ambient.f =0;     
    ParseLeftCurly();
    while(1) { 
      GetToken();
      if(Token.id == T_AMBIENT_LIGHT) {
        ParseLeftCurly();
        GetToken();
        if(Token.id != T_COLOR) 
          Error("Error parsing light source: missing color");
        ParseColor(&ambient);
        ParseRightCurly();
      } else if(Token.id == T_RIGHT_CURLY) { 
        break;
      } else         
        Error("error parsing default settings: unexpected token");
    }

    scene.set_global(Scene::GLOBAL_AMBIENT_R, ambient.r);
    scene.set_global(Scene::GLOBAL_AMBIENT_G, ambient.g);
    scene.set_global(Scene::GLOBAL_AMBIENT_B, ambient.b);
  }

  /* main parsing function calling functions to parse each object;  */

  void Parse(FILE* infile, Scene& s) { 
    InitializeToken(infile);
    GetToken();
    while(Token.id != T_EOF) { 
      switch(Token.id) { 
      case T_CAMERA:   ParseCamera(s);   break;
      case T_POLYGON:  ParsePolygon(s);  break;
      case T_SPHERE:   ParseSphere(s);   break;
      case T_BOX:      ParseBox(s);      break;
      case T_CYLINDER: ParseCylinder(s); break;
      case T_CONE:     ParseCone();     break;
      case T_QUADRIC:  ParseQuadric();  break;
      case T_LIGHT_SOURCE:  ParseLightSource(s);  break;
      case T_GLOBAL_SETTINGS: ParseGlobalSettings(s); break;
      default:         Error("Unknown statement");
      }
      GetToken();
    }
  }

};
