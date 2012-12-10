#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include <vector>
#include "platform.h"
#include "vec.hpp"
#include "mat.hpp"

class Object {

  enum bufferType { 
    VERTICES,
    NORMALS,
    INDICES,
    COLORS,
    TEXCOORDS };

public:
  Object( void );
  ~Object( void );
  void Draw( void );

protected:  
  GLuint vao;
  GLuint buffer[5];
  GLenum draw_mode;

  std::vector<Angel::vec4> points;
  std::vector<Angel::vec3> normals;
  std::vector<unsigned int> indices;
  std::vector<Angel::vec4> colors;
  std::vector<Angel::vec2> texcoords;

  /* Per-Object CTM */
  Angel::mat4 CTM;
  
  /* Hello, Children! */
  std::vector<Object> children;
};

#endif
