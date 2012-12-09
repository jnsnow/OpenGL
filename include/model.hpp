#ifndef __MODEL_H
#define __MODEL_H

#include "vec.hpp"
#include "Object.hpp"

using Angel::vec4;
using Angel::vec3;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

/* do we really need these here?

   Yes.
   --jh
*/
extern point4  points[];
extern color4  colors[];
extern vec3  normals[];

void createPoint( Object *obj, 
		  point4 const &the_point, color4 const &the_color, vec3 const &the_normal );

void triangle( Object *obj,
	       const point4& a, const point4& b, 
	       const point4& c, const int color );

void tetra( Object *obj,
	    const point4& a, const point4& b, 
	    const point4& c, const point4& d );

void Sierpinski_Pyramid( Object *obj,
			 const point4& a, const point4& b,
			 const point4& c, const point4& d, int count );


void quad( Object *obj,
	   const point4 &a, const point4 &b,
	   const point4 &c, const point4 &d,
	   const color4 &A, const color4 &B,
	   const color4 &C, const color4 &D );

void cube( Object *obj, const GLfloat &size, const color4 colors[8] );

void colorcube( Object *obj, GLfloat size );

void landGen( Object *obj );

#endif
