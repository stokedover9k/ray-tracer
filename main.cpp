#include <stdio.h>
#include "tokens.h"
#include "parse.h"

#include "headers.h"
#include "scene.h"
#include "tga.h"

#define SCREEN_W 600
#define SCREEN_H 450

using std::cout;
using std::cerr;
using std::endl;

///////////////////////////////////////////////////////////////////////////////

namespace ARGS {
  enum { PROG_NAME = 0,
	 SCENE_DESCR_FILE,
	 SCR_RES_W,
	 SCR_RES_H,
  };
};

//-----------------------------------------------------------------------------

namespace Params {
  unsigned int scr_width  = SCREEN_W;
  unsigned int scr_height = SCREEN_H;
};

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator << (std::ostream& s, const Ray& r) {
  s << "R{" << r.origin() << " -> " << r.dir() << " (" << r.color() << ")}";
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

int main(int argc, char* argv[]) 
{
  if( argc <= ARGS::SCENE_DESCR_FILE ) {
    cerr << "ERROR: missing scene description file.\nExiting (0)." << endl;
    exit(0);
  }

  if( argc >= ARGS::SCR_RES_W + 1 ) {
    Params::scr_width = atoi(argv[ARGS::SCR_RES_W]);
    
    if( argc >= ARGS::SCR_RES_H + 1 )   
      Params::scr_height = atoi(argv[ARGS::SCR_RES_H]);
    else
      Params::scr_height = Params::scr_width * 3 / 4;
  }

  Scene scene(Params::scr_width, Params::scr_height);
  GLubyte *pixels = (GLubyte *) malloc (Params::scr_width * Params::scr_height * 
					3 * sizeof(GLubyte));

  /*******************************************/
  /**************** Parse Scene **************/
  /*******************************************/
  try 
    {
      FILE*  infile;
      infile = fopen(argv[ARGS::SCENE_DESCR_FILE],"r");
      if( infile ) 
	SceneParser::Parse(infile, scene);
      fclose( infile );
    }
  catch (...) 
    {
      cerr << "ERROR: while parsing and creating scene.\nExiting (1)" << endl;
      exit(1);
    }

  /*******************************************/
  /**************** Ray Tracing **************/
  /*******************************************/
  try
    {
      GLubyte *pxl_ptr = pixels;  //tmp pointer to store pixel values

      for( int j=0; j<Params::scr_height; j++ ) {
	for( int i=0; i<Params::scr_width; i++ ) {
	  
	  const Color& color = scene.trace_ray( scene.pixel_ray(i, j), 0 );
	  pxl_ptr = storePixel(pxl_ptr, color);

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
      FILE *fp = fopen( "out.tga", "wb" );
      writeTGA( &img, fp );
    }
  catch (const char* e)
    {
      cerr << "ERROR: " << e << "\nExiting (3)" << endl;
      exit(3);
    }

  return 0;
}

