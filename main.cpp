#include <stdio.h>
#include "tokens.h"
#include "parse.h"

#include "headers.h"
#include "scene.h"
#include "tga.h"

#define SCREEN_W 600
#define MAX_RAY_DEPTH 1

const char* USAGE = 
  "Usage: tracer SCENE_FILE [PARAMS] \n"
  "\n"
  "List of parameters: \n"
  " param |   type    |    description                            | default\n"
  "  -w   |  integer  |  Width (in pixels) of the output image    |  600\n"
  "  -h   |  integer  |  Height (in pixels) of the output image   | ~~varied\n"
  "  -d   |  integer  |  Maximum ray reflection/refraction depth  |  1\n"
  "  -o   | file name |  Output image filename                    | \"out.tga\"\n"
  "\n"
  "WIDTH and HEIGHT of the output image:\n"
  "  If only one of the default values is overwritten, the other is adjusted\n"
  "  to match the aspect ratio specified by the input file.";

using std::cout;
using std::cerr;
using std::endl;

///////////////////////////////////////////////////////////////////////////////

namespace ARGS {
  enum { PROG_NAME = 0,
	 SCENE_DESCR_FILE,
	 OPTIONAL,
  };
};

//-----------------------------------------------------------------------------

namespace Params {
  unsigned int scr_width  = SCREEN_W;
  unsigned int scr_height = 1;
  size_t max_ray_depth = MAX_RAY_DEPTH;
  const char* input_file_name = NULL;
  const char* output_file_name = "out.tga";
};

void parse_args(int argc, char *argv[], const Scene& scene) {
  bool resized_w = false;
  bool resized_h = false;
  for( int argn = ARGS::OPTIONAL; argn < argc; argn++ ) 
    {
      if( strcmp(argv[argn], "-w") == 0 ) {
	if( ++argn == argc ) throw "pixel WIDTH indicated, but not provided.";
	Params::scr_width = atoi(argv[argn]);
	resized_w = true;
      } 

      else if( strcmp(argv[argn], "-h") == 0 ) {
	if( ++argn == argc ) throw "pixel HEIGHT indicated, but not provided.";
	Params::scr_height = atoi(argv[argn]);
	resized_h = true;
      }

      else if( strcmp(argv[argn], "-o") == 0 ) {
	if( ++argn == argc ) throw "OUTPUT FILE indicated, but not provided.";
	Params::output_file_name = argv[argn];
      }

      else if( strcmp(argv[argn], "-d") == 0 ) {
	if( ++argn == argc ) throw "MAX RAY DEPTH indicated, but not provided";
	Params::max_ray_depth = atoi(argv[argn]);
      }

      else {
	throw "unknown argument.";
      }
    }

  if( !resized_h ) 
    Params::scr_height = Params::scr_width * (scene.aspect_ratio()+ZERO);
  else if( !resized_w )
    Params::scr_width = Params::scr_height / (scene.aspect_ratio()-ZERO);

  if( Params::scr_width     == 0 ) throw "pixel WIDTH set to 0";
  if( Params::scr_height    == 0 ) throw "pixel HEIGHT set to 0";
  if( Params::max_ray_depth == 0 ) throw "max RAY DEPTH set to 0 (must be >0)";
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator << (std::ostream& s, const Ray& r) {
  s << "R{" << r.from() << " -> " << r.dir() << " (" << r.color() << ")}";
  return s;
}

// returns the pointer to the following pixel
GLubyte * storePixel(GLubyte *pxl_ptr, const Color& c) {
  GLubyte *i = pxl_ptr;
  *(i++) = GLubyte(c(2) * 255.0);
  *(i++) = GLubyte(c(1) * 255.0);
  *(i++) = GLubyte(c(0) * 255.0);
  return i;
}

///////////////////////////////////////////////////////////////////////////////
//----------------------------   MAIN   -------------------------------------//
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) 
{
  if( argc <= ARGS::SCENE_DESCR_FILE ) {
    cerr << "ERROR: missing scene description file.\n\n"
	 << USAGE << "\nExiting (0)." << endl;
    exit(0);
  }

  Scene scene;
  GLubyte *pixels;
  FILE *outfile;

  /*******************************************/
  /**************** Parse Scene **************/
  /*******************************************/
  try 
    {
      FILE* infile = fopen(argv[ARGS::SCENE_DESCR_FILE],"r");
      if( infile ) 
	SceneParser::Parse(infile, scene);
      else throw "failed to open the input file.";
      fclose( infile );
    }
  catch (const char* e) 
    {
      cerr << "ERROR: (while parsing and creating scene) " << e << "\n\n"
	   << USAGE << "\nExiting(1)" << endl;
      exit(1);
    }
  catch (...) 
    {
      cerr << "ERROR: (while parsing and creating scene).\nExiting (1)" << endl;
      exit(1);
    }

  /*******************************************/
  /**************** Parse ARGS ***************/
  /*******************************************/
  try
    {
      parse_args(argc, argv, scene);

      pixels = (GLubyte *)malloc(Params::scr_width * Params::scr_height 
				 * 3 * sizeof(GLubyte));
      if( pixels == NULL ) throw "Failed to allocate memory for the pixel map";

      outfile = fopen( Params::output_file_name, "wb" );
      if( outfile == NULL ) throw "Failed to open output file";

      scene.set_pxl_width ( Params::scr_width  );
      scene.set_pxl_height( Params::scr_height );
    }
  catch (const char* e)
    {
      cerr << "ERROR: (while argument parsing and setup) " << e << "\n\n"
	   << USAGE << "\nExiting (0)." << endl;
      exit(0);
    }

  /*******************************************/
  /**************** Ray Tracing **************/
  /*******************************************/
  try
    {
      GLubyte *pxl_ptr = pixels;  //tmp pointer to store pixel values

      for( int j=0; j<Params::scr_height; j++ ) {
	for( int i=0; i<Params::scr_width; i++ ) {
	  Ray r = scene.pixel_ray(i, j);
	  scene.trace_ray( r, Params::max_ray_depth );
	  pxl_ptr = storePixel(pxl_ptr, r.color());
	}
      }
    }
  catch (const char* e)
    {
      cerr << "ERROR: " << e << "\nExiting (2)" << endl;
      exit(2);
    }

  /*******************************************/
  /********** Output pixels to file **********/
  /*******************************************/
  try
    {
      gliGenericImage img;
      img.width  = Params::scr_width;
      img.height = Params::scr_height;
      img.components = 3;
      img.format = GL_RGB;
      img.pixels = pixels;
      img.cmap = NULL;
      writeTGA( &img, outfile );
    }
  catch (const char* e)
    {
      cerr << "ERROR: " << e << "\nExiting (3)" << endl;
      exit(3);
    }

  return 0;
}

