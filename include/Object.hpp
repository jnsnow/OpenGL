#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include <vector>
#include <string>
#include "platform.h"
#include "vec.hpp"
#include "mat.hpp"
#include "Scene.hpp"
#include "TransCache.hpp"

class Object : public Scene {

  enum bufferType { 
    VERTICES,
    NORMALS,
    INDICES,
    COLORS,
    TEXCOORDS };

public:

  typedef enum Uniforms {
    Begin,
    IsTextured = Begin,
    ObjectCTM,
    End
  } Uniform;

  Object( const std::string &name, GLuint gShader );
  virtual ~Object( void );
  void Draw( void );
  void Buffer( void );
  void Mode( GLenum new_node );
  void Texture( const char** filename );
  const std::string &Name( void ) const;
  virtual void Link( Object::Uniform which, const std::string &name );

  void Animation(void (*anim_func)( TransCache &arg ));

  /* Bad. Bad! Protect these. ...Later? :( */
  std::vector<Angel::vec4> points;
  std::vector<Angel::vec3> normals;
  std::vector<unsigned int> indices;
  std::vector<Angel::vec4> colors;
  std::vector<Angel::vec2> texcoords;


protected:
  /** name is used as an identifying handle for the object. **/
  std::string name;     

  /** Vertex Array Object handle identifying our buffers/object. **/
  GLuint vao;

  /** Handles to our five buffers **/
  GLuint buffer[5];
  
  /** Drawing mode for this object. GL_TRIANGLES, GL_LINE_LOOP, etc. **/
  GLenum draw_mode;

  /** Handles to Uniforms on the shader. **/
  GLint *handles;

  bool isTextured;

  /** Transformation State **/
  TransCache trans;

};

#endif
