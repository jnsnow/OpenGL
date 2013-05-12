/**
 @file Particle.hpp
 @author Nick Ver Voort, nicholas_vervoort@student.uml.edu
 @since 23 Feb 2013
 @authors Nick Ver Voort, Chris Compton, Nicholas St.Pierre
 nicholas_vervoort@student.uml.edu, christopher_compton@student.uml.edu, nstpierr@cs.uml.edu
 @brief The Particle class
 **/

#ifndef __PARTICLE_H
#define __PARTICLE_H

#include <string>
#include "mat.hpp"
#include "vec.hpp"
#include "Object.hpp"

using std::string;


class Particle {
  
public:

  Particle( Angel::vec4 position, float lifetime );
  Particle( Angel::vec4 position, float scale, float lifetime );
  ~Particle( void );

  void changeVel( Angel::vec3 newVel ); // ???

  // setters
  void  setAlpha( float newAlpha );
  void  setColor( Angel::vec4 newColor );
  void  setLifetime( float newLifetime );
  void  setMaxLifetime( float newMaxLife );
  void  setPos( Angel::vec4 newPos );
  void  setScale( Angel::vec3 newScale );
  void  setVel( Angel::vec3 newVel );
  void  setParticleRespawnFlag( bool );

  // getters
  Angel::vec4  getPosition( void );
  float getMaxLifetime(void);
  float getLifetime( void );
  float getAlpha( void ) ;
  Angel::vec4 getColor( void );
  bool getRespawnFlag( void );
  //void setSpin( float newSpin );

  void updateSelf( void );

private:
  Angel::vec4   _pos;
  Angel::vec3   _scale;
  Angel::vec3   _vel;
  Angel::vec4   _color;
  float  _lifetime;
  float  _maxLifetime;
  bool   _respawnFlag;
  //  float spin;

};

#endif
