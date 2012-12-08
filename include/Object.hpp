#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include <vector>
#include "platform.h"
#include "vec.hpp"

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

private:
  
  GLuint vao;
  GLuint buffer[5];
  GLenum draw_mode;
 
protected:

  std::vector<Angel::vec4> points;
  std::vector<Angel::vec3> normals;
  std::vector<unsigned int> indices;
  std::vector<Angel::vec4> colors;
  std::vector<Angel::vec2> texcoords;
  
};

#endif
