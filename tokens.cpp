#include "tokens.h"

/* Tokeninzing part of the parser; the parsing routines call 
GetToken() and UngetToken() to retrieve tokens from the input stream.
GetToken reads the input stream of characters stripping the comments 
until a complete token is produced. Then the token is placed into the
global variable TOken, from which it can ber retrieved by the calling
routines. 
*/


/* table of token names  for debugging and diagnostics */

const char* TokenNames[] = { 
  "DOUBLE",
  "COMMA",
  "LEFT_ANGLE",
  "RIGHT_ANGLE",
  "LEFT_CURLY",
  "RIGHT_CURLY",
  "ROTATE",
  "TRANSLATE",
  "SCALE",
  "MATRIX",
  "POLYGON",
  "SPHERE",
  "BOX",
  "CYLINDER",
  "CONE",
  "QUADRIC",
  "CAMERA",
  "LOCATION",
  "RIGHT",
  "UP",
  "LOOK_AT",
  "ANGLE",
  "GLOBAL_SETTINGS",
  "AMBIENT_LIGHT",
  "LIGHT_SOURCE",
  "FINISH",
  "PIGMENT",
  "COLOR",
  "RGB",
  "RGBF",
  "REFLECTION",
  "AMBIENT",
  "DIFFUSE",
  "PHONG",
  "METALLIC",
  "PHONG_SIZE",
  "INTERIOR",
  "IOR",         
  "NULL",
  "EOF"
};





struct Token_Struct Token;

struct ReservedWord_Struct { 
  enum TokenIDs id; 
  const char* string;
};

struct ReservedWord_Struct ReservedWords[] = { 
  {  T_ROTATE,    "rotate" },
  {  T_TRANSLATE, "translate" },
  {  T_SCALE,     "scale" },
  {  T_MATRIX,    "matrix" },
  {  T_POLYGON,   "polygon" },
  {  T_SPHERE,    "sphere" },
  {  T_BOX,       "box" },
  {  T_CYLINDER,  "cylinder" },
  {  T_CONE,      "cone" },
  {  T_QUADRIC,   "quadric" },
  {  T_CAMERA,    "camera" },
  {  T_LOCATION,  "location" },
  {  T_RIGHT,     "right" },
  {  T_UP,        "up" },
  {  T_LOOK_AT,   "look_at" },
  {  T_ANGLE,     "angle" },

  {  T_GLOBAL_SETTINGS, "global_settings"},
  {  T_AMBIENT_LIGHT, "ambient_light"},
  {  T_LIGHT_SOURCE, "light_source"},
  {  T_FINISH, "finish"},
  {  T_PIGMENT, "pigment"},
  {  T_RGB, "rgb"},
  {  T_COLOR, "color"},
  {  T_RGBF, "rgbf"},
  {  T_REFLECTION, "reflection"},
  {  T_AMBIENT, "ambient"},
  {  T_DIFFUSE, "diffuse"},
  {  T_PHONG, "phong"},
  {  T_METALLIC, "metallic"},
  {  T_PHONG_SIZE, "phong_size"},
  {  T_INTERIOR, "interior"},
  {  T_IOR, "ior"},
  {  T_LAST, ""}        
};


#define CR '\010'
#define LF '\0'

/* 
Error handling: print a message and terminate the program.
exit(0) should be replaced by stack unwinding 
(e.g. using setjmp/longjmp or C++ exceptions) if 
return to the calling program rather than simple 
exit is required.
*/


void Error(const char* str ) { 
  printf("Line %d: %s\n", Token.lineNumber, str); 
  exit(0);
}

/* should be called before GetToken() */

void InitializeToken(FILE* infile) { 
  Token.unget_flag = 0;
  Token.id = T_NULL;
  Token.infile = infile;
  Token.lineNumber = 1;
}


static void SkipSpaces() { 
  int c;
  while(1) { 
    c = getc(Token.infile);
    if( c == '\n') Token.lineNumber++;
    if (c == EOF ) return;
    if( c ==  '/') {
      /* we use slash only as a part of the 
         comment begin seqence; if something other than another 
         slash follows it, it is an error */
      if( getc(Token.infile) == '/') {
        /* skip everything till the end of the line */
        while( c != '\n' && c != '\r' && c != EOF ) {
          c = getc(Token.infile);
        }
        Token.lineNumber++;
      } else Error("Missing secnd slash in comment");
    }
    if( !isspace(c))
      break;
  } 
  ungetc(c,Token.infile);
}


#define MAX_STRING_LENGTH 128

void ReadDouble() { 
  /* this is cheating -- we'd better parse the number definition 
     ourselves, to make sure it conforms to a known standard and 
     to do error hanndling properly,
     but for our purposes  this is good enough */
  int res;
  res = fscanf( Token.infile, "%le", &Token.double_value);
  if( res == 1 ) Token.id = T_DOUBLE; 
  else Error("Could not read a number");
}

#define MAX_STR_LENGTH 128

/* this is a very stupid way of looking up things in a table, 
   but because our table is so small, it works ok */

static enum TokenIDs FindReserved(char* str) { 
  int c;
  struct ReservedWord_Struct* ptr; 
  ptr = ReservedWords;
  while( ptr->id != T_LAST ) {
    if( !strcmp(str, ptr->string) ) return ptr->id;
    ptr++;
  }
  return T_NULL;
}

static void ReadName() { 
  char str[MAX_STR_LENGTH];
  int str_index;
  int c;

  str_index = 0;
  while (1) {
    c = getc(Token.infile);
    if (c == EOF) Error("Could not read a name");
    if (isalpha(c) || isdigit(c) || c == '_') {
      /* if the name is too long, ignore extra characters */
      if( str_index < MAX_STR_LENGTH-1)
        str[str_index++] = c;
      } else {
      ungetc(c,Token.infile);
      break;
    }
  }
  str[str_index++] = '\0';
  Token.id = FindReserved(str);
  if( Token.id == T_NULL) { 
    fprintf(stderr, "%s: ", str); 
    Error("Unknown reserved word");
  }
  return;
}

/* sets the global struct Token to the
  next input token, if there is one. 
  if there is no legal token in the input, 
  returns 0, otherwise returns 1.
*/

void GetToken() { 
  int c;
  if( Token.unget_flag) { 
    Token.unget_flag = FALSE;
    return;
  }
  
  SkipSpaces();
  c = getc(Token.infile);
  
  if( c == EOF ) { 
    Token.id = T_EOF;
    return;
  }
  
  if(isalpha(c)) { 
    ungetc(c,Token.infile); 
    ReadName();
  } else if( isdigit(c) || c == '.' || c == '-' || c == '+' ) { 
    ungetc(c,Token.infile); 
    ReadDouble();
  } else { 
    switch(c) { 
    case ',':   
      Token.id = T_COMMA;
      break;    
    case '{': 
      Token.id = T_LEFT_CURLY;
      break;
    case '}': 
      Token.id = T_RIGHT_CURLY;
      break;
    case '<': 
      Token.id = T_LEFT_ANGLE;
      break;
    case '>': 
      Token.id = T_RIGHT_ANGLE;
      break;
    default:
      Error("Unknown token");
    }
  }
}

/* Assumes that GetToken() was called at least once. 
Cannot be called two times without a GetToken() between 
the calls */

void UngetToken() { 
  assert(!Token.unget_flag);
  Token.unget_flag = TRUE;
}

