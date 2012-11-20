/* what the fuck is happening to me i am a frog */
#ifndef __MODEL_H
#define __MODEL_H

#include "Angel.h"
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;
/* do we really need these here?
extern point4 points[];
extern color4 colors[];
extern vec3 normals[];
*/
void createPoint( point4 const &the_point, color4 const &the_color, vec3 const &the_normal );

void triangle( const point4& a, const point4& b, 
	       const point4& c, const int color );

void tetra( const point4& a, const point4& b, 
	    const point4& c, const point4& d );

void divide_tetra( const point4& a, const point4& b,
		   const point4& c, const point4& d, int count );

void quad( int a, int b, int c, int d );

void quad( const point4 &a, const point4 &b,
	   const point4 &c, const point4 &d,
	   const color4 &A, const color4 &B,
	   const color4 &C, const color4 &D );

void cube( const GLfloat &size, const color4 colors[8] );

void colorcube( void );

void colorcube( GLfloat size );

#endif
