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
  Object( GLuint gShader );
  ~Object( void );
  void Draw( void );
  void Buffer( void );
  void Mode( GLenum new_node );

  /* Bad. Bad! Protect these. ...Later? :( */
  std::vector<Angel::vec4> points;
  std::vector<Angel::vec3> normals;
  std::vector<unsigned int> indices;
  std::vector<Angel::vec4> colors;
  std::vector<Angel::vec2> texcoords;

protected:  
  GLuint vao;
  GLuint buffer[5];
  GLenum draw_mode;
  GLuint gShader;

  /* Per-Object CTM */
  Angel::mat4 CTM;
  
  /* Hello, Children! */
  std::vector<Object> children;
};

#endif
