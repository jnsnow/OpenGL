#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include <vector>
#include <string>
#include "platform.h"
#include "vec.hpp"
#include "mat.hpp"
#include "Scene.hpp"

class Object : public Scene {

  enum bufferType { 
    VERTICES,
    NORMALS,
    INDICES,
    COLORS,
    TEXCOORDS };

public:
  Object( const std::string &name, GLuint gShader );
  ~Object( void );
  void Draw( void );
  void Buffer( void );
  void Mode( GLenum new_node );
  void Texture( const char* filename );
  const std::string &Name( void ) const;

  /* Bad. Bad! Protect these. ...Later? :( */
  std::vector<Angel::vec4> points;
  std::vector<Angel::vec3> normals;
  std::vector<unsigned int> indices;
  std::vector<Angel::vec4> colors;
  std::vector<Angel::vec2> texcoords;

protected:
  std::string name; /* name is used as an identifying handle for the object. */
  GLuint vao;  /* Vertex Array Object handle identifying our buffers */
  GLuint buffer[5]; /* Our buffer handles. */
  GLenum draw_mode; /* How should we draw? GL_TRIANGLES? GL_LINE_LOOP? etc. */

};

#endif
