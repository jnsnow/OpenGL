/* 
 * File:   LightSource.cpp
 * Author: nico200
 * 
 * Created on November 20, 2012, 1:39 PM
 */

#include "LightSource.hpp"
#include "Angel.h"


LightSource::LightSource() {
}

LightSource::LightSource(const LightSource& orig) {
}

LightSource::~ LightSource() {
}

void LightSource::SetLight_specular(color4 light_specular) {
  this->light_specular = light_specular ;
}

color4 LightSource::GetLight_specular() const {
  return light_specular ;
}

void LightSource::SetLight_diffuse(color4 light_diffuse) {
  this->light_diffuse = light_diffuse ;
}

color4 LightSource::GetLight_diffuse() const {
  return light_diffuse ;
}

void LightSource::SetLight_ambient(color4 light_ambient) {
  this->light_ambient = light_ambient ;
}

color4 LightSource::GetLight_ambient() const {
  return light_ambient ;
}

void LightSource::SetDirection(vec3 direction) {
  this->direction = direction ;
}

vec3 LightSource::GetDirection() const {
  return direction ;
}

void LightSource::SetPosition(point4 z) {
  this->z = z ;
}

point4 LightSource::GetPoint() const {
  return z ;
}




void
init_lights(void)
{ // Initialize shader lighting parameters
  //    point4 light_position( 0.0, 0.0, -1.0, 0.0 );
  //    point4 light_position2( 0.0, 2.0, -1.0, 0.0 );
  color4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
  color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
  color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

  color4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
  color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 );
  color4 material_specular( 1.0, 0.8, 0.0, 1.0 );
  float  material_shininess = 100.0;

  color4 ambient_product = light_ambient * material_ambient;
  color4 diffuse_product = light_diffuse * material_diffuse;
  color4 specular_product = light_specular * material_specular;

  // This is all copy-pasted. And it is done poorly.

  glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
		1, ambient_product );
  glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product );
  glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product );

  glUniform4fv( glGetUniformLocation(program, "LightPosition"),
		1, light_position );

  glUniform4fv( glGetUniformLocation(program, "LightPosition2"),
		1, light_position2 );

  glUniform1f( glGetUniformLocation(program, "Shininess"),
	       material_shininess );
}
