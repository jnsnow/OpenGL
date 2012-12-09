#include <string>
#include <vector>
#include "platform.h"
#include "vec.hpp"
#include "Object.hpp"

Object::Object( const std::string &name, GLuint gShader ) {

  /* The constructor is going to initialize the VAO and a series of VBOs.
     The VAO is our general handle to this collection of VBOs.

     Each VBO contains some component data for how to render the vertex:
     Position, Color, Direction (Normal), Texture and Draw Order. */
    
  /* Associate this Object with the Shader. */
  this->gShader = gShader;
  this->name = name;
  GLuint glsl_uniform;

  /* Initialize our draw mode to GL_LINE_STRIP until informed otherwise. */
  draw_mode = GL_LINE_STRIP;

  /* Create our VAO, which is our handle to all the rest of the following information. */
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  /* Does this go here? I have no real idea. */
  glUseProgram( gShader );

  /* Create five VBOs: One each for Positions, Colors, Normals, Textures and Draw Order. */
  glGenBuffers( 5, buffer );

  /* Create the Vertex buffer and link it with the shader. */
  glBindBuffer( GL_ARRAY_BUFFER, buffer[VERTICES] );
  glsl_uniform = glGetAttribLocation( gShader, "vPosition" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 4, GL_FLOAT, GL_FALSE, 0, 0 );

  /* Create the Normal buffer and link it with the shader. */
  glBindBuffer( GL_ARRAY_BUFFER, buffer[NORMALS] );
  glsl_uniform = glGetAttribLocation( gShader, "vNormal" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  /* Create the Color buffer and link it with the shader. */
  glBindBuffer( GL_ARRAY_BUFFER, buffer[COLORS] );
  glsl_uniform = glGetAttribLocation( gShader, "vColor" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 4, GL_FLOAT, GL_FALSE, 0, 0 );

  /* Create the Texture Coordinate buffer and link it with the shader. */
  glBindBuffer( GL_ARRAY_BUFFER, buffer[TEXCOORDS] );
  glsl_uniform = glGetAttribLocation( gShader, "TexXY" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 2, GL_FLOAT, GL_FALSE, 0, 0 );

  /* Create the Drawing Order buffer, but we don't need to link it with the shader,
     because we won't be accessing this data directly. (I.e, the numbers here
     are not important once we are in the Vertex Shader. */
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer[INDICES] );

  /* Unset the VAO context. */
  glBindVertexArray( 0 );
}

Object::~Object( void ) {
  /* Noooothing? */
}

void Object::Buffer( void ) {

  glBindVertexArray( vao );
  
  glBindBuffer( GL_ARRAY_BUFFER, buffer[VERTICES] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec4) * points.size(),
		&(points[0]), GL_STATIC_DRAW );

  glBindBuffer( GL_ARRAY_BUFFER, buffer[NORMALS] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec3) * normals.size(),
		&(normals[0]), GL_STATIC_DRAW );

  glBindBuffer( GL_ARRAY_BUFFER, buffer[COLORS] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec4) * colors.size(),
		&(colors[0]), GL_STATIC_DRAW );

  glBindBuffer( GL_ARRAY_BUFFER, buffer[TEXCOORDS] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec2) * texcoords.size(),
		&(texcoords[0]), GL_STATIC_DRAW );
  
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer[INDICES] );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
		&(indices[0]), GL_STATIC_DRAW );

  glBindVertexArray( 0 );

}

void Object::Draw( void ) {

  glBindVertexArray( vao );
  if (indices.size() > 0)
    glDrawElements( draw_mode, indices.size(), GL_UNSIGNED_INT, 0 );
  else
    glDrawArrays( draw_mode, 0, points.size() );
  glBindVertexArray(0);
  
}

void Object::Mode( GLenum new_mode ) {

  this->draw_mode = new_mode;

}

const std::string &Object::Name( void ) const {
  return name;
}
