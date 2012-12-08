#include <vector>
#include "vec.hpp"
#include "Object.hpp"

Object::Object( void ) {

  draw_mode = GL_LINE_STRIP;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 5, buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer[VERTICES] );
  glBindBuffer( GL_ARRAY_BUFFER, buffer[NORMALS] );
  glBindBuffer( GL_ARRAY_BUFFER, buffer[COLORS] );
  glBindBuffer( GL_ARRAY_BUFFER, buffer[TEXCOORDS] );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer[INDICES] );

  /* Unset the VAO context. */
  //glBindVertexArray( 0 );
}

Object::~Object( void ) {
  /* Noooothing? */
}

void Object::Draw( void ) {

  glBindVertexArray( vao );
  if (indices.size() > 0)
    glDrawElements( draw_mode, indices.size(), GL_UNSIGNED_INT, 0 );
  else
    glDrawArrays( draw_mode, 0, points.size() );
  glBindVertexArray(0);
  
}
