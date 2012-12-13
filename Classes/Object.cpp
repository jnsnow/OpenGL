#include <string>
#include <vector>
#include "platform.h"
#include "vec.hpp"
#include "Object.hpp"
#include <SOIL.h>
#include "globals.h"


Object::Object( const std::string &name, GLuint gShader ) {

  /* The constructor is going to initialize the VAO and a series of VBOs.
     The VAO is our general handle to this collection of VBOs.

     Each VBO contains some component data for how to render the vertex:
     Position, Color, Direction (Normal), Texture and Draw Order. */
    
  // Create room for our GLUniform handles
  handles = new GLuint [Object::End];

  // Associate this Object with the Shader.
  SetShader( gShader );
  this->name = name;
  /* Does this go here? I have no real idea. */
  glUseProgram( gShader );

  // Load 
  Link( Object::IsTextured, "fIsTextured" );
  Link( Object::ObjectCTM, "OCTM" );

  /* Initialize our draw mode to GL_LINE_STRIP until informed otherwise. */
  draw_mode = GL_LINE_STRIP;

  /* Create our VAO, which is our handle to all the rest of the following information. */
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  GLuint glsl_uniform;

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
  glsl_uniform = glGetAttribLocation( gShader, "vTex" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 2, GL_FLOAT, GL_FALSE, 0, 0 );

  if (DEBUG) 
    fprintf( stderr,
	     "buffhandles: %u %u %u %u %u\n",
	     buffer[VERTICES], buffer[NORMALS],
	     buffer[COLORS], buffer[TEXCOORDS],
	     buffer[INDICES] );
  
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

  /*
  if (texcoords.size() < points.size()) {
    if (DEBUG) 
      fprintf( stderr, "Resizing texcoords array prior to buffer.\n" );
    texcoords.resize( points.size(), Angel::vec2(0,0) );
  }
  */

  if (texcoords.size()) {
    glBindBuffer( GL_ARRAY_BUFFER, buffer[TEXCOORDS] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec2) * texcoords.size(),
		  &(texcoords[0]), GL_STATIC_DRAW );
  }
  
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer[INDICES] );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
		&(indices[0]), GL_STATIC_DRAW );

  glBindVertexArray( 0 );

}


void Object::Link( Object::Uniform which, const std::string &name ) {

  handles[which] = glGetUniformLocation( GetShader(), name.c_str() );

}


void Object::Texture( const char** filename ) {

  glBindVertexArray( vao );

  GLuint tex2dgrass = SOIL_load_OGL_texture( filename[0],
					     SOIL_LOAD_AUTO,
					     SOIL_CREATE_NEW_ID,
					     SOIL_FLAG_MIPMAPS | 
					     SOIL_FLAG_INVERT_Y | 
					     SOIL_FLAG_NTSC_SAFE_RGB | 
					     SOIL_FLAG_COMPRESS_TO_DXT );

  GLuint tex2drock = SOIL_load_OGL_texture( filename[1],
					    SOIL_LOAD_AUTO,
					    SOIL_CREATE_NEW_ID,
					    SOIL_FLAG_MIPMAPS | 
					    SOIL_FLAG_INVERT_Y | 
					    SOIL_FLAG_NTSC_SAFE_RGB | 
					    SOIL_FLAG_COMPRESS_TO_DXT );

  GLuint tex2dsnow = SOIL_load_OGL_texture( filename[2],
					    SOIL_LOAD_AUTO,
					    SOIL_CREATE_NEW_ID,
					    SOIL_FLAG_MIPMAPS | 
					    SOIL_FLAG_INVERT_Y | 
					    SOIL_FLAG_NTSC_SAFE_RGB | 
					    SOIL_FLAG_COMPRESS_TO_DXT );
  
  GLuint gSampler0 = glGetUniformLocation( GetShader(), "gSampler0" );
  glUniform1i( gSampler0, 0 );
  GLuint gSampler1 = glGetUniformLocation( GetShader(), "gSampler1" );
  glUniform1i( gSampler1, 1 );
  GLuint gSampler2 = glGetUniformLocation( GetShader(), "gSampler2" );
  glUniform1i( gSampler2, 2 );

  glActiveTexture( GL_TEXTURE0 );
  glBindTexture( GL_TEXTURE_2D, tex2dgrass );
  glEnable( GL_TEXTURE_2D );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  glActiveTexture( GL_TEXTURE1 );
  glBindTexture( GL_TEXTURE_2D, tex2drock );
  glEnable( GL_TEXTURE_2D );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  glActiveTexture( GL_TEXTURE2 );
  glBindTexture( GL_TEXTURE_2D, tex2dsnow );
  glEnable( GL_TEXTURE_2D );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  
  glBindVertexArray( 0 );

}

void Object::Draw( void ) {

  glBindVertexArray( vao );

  /* Inform the shader if it should texture this object or not. */
  glUniform1i( handles[Object::IsTextured],
	       (texcoords.size() > 0) ? 1 : 0 );

  if (indices.size() > 0)
    glDrawElements( draw_mode, indices.size(), GL_UNSIGNED_INT, 0 );
  else
    glDrawArrays( draw_mode, 0, points.size() );
  glBindVertexArray(0);

  // Draw all of our Children.
  // (With clothes on, pervert.)
  Scene::Draw();

}

void Object::Mode( GLenum new_mode ) {

  this->draw_mode = new_mode;

}

const std::string &Object::Name( void ) const {
  return name;
}
