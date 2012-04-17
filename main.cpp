#include <stdio.h>
#include "tokens.h"
#include "parse.h"

#include "headers.h"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char* argv[]) { 
  Scene scene;

  try 
    {
      FILE*  infile;
      if( argc <= 1) return 0;
      infile = fopen(argv[1],"r");
      if( infile ) 
	SceneParser::Parse(infile, scene);
      fclose( infile );
    }
  catch (...) 
    {
      cerr << "ERROR: while parsing and creating scene.\nExiting (1)" << endl;
      exit(1);
    }

  try
    {
      
    }
  catch (const char* e)
    {
      cerr << "ERROR: " << e << "\nExiting (2)" << endl;
      exit(2);
    }

  return 0;
}

