#ifndef __MODEL_H
#define __MODEL_H

// Needed for model loader
#include <vector>
using std::vector;

//#include <fstream>
//#include <sstream>
//#include <string>


#include "vec.hpp"
using Angel::vec4;
using Angel::vec3;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;




extern point4  points[];
extern color4  colors[];
extern vec3  normals[];


void createPoint( point4 const &the_point, color4 const &the_color, vec3 const &the_normal );

void triangle( const point4& a, const point4& b, 
	       const point4& c, const int color );

void tetra( const point4& a, const point4& b, 
	    const point4& c, const point4& d );

void divide_tetra( const point4& a, const point4& b,
		   const point4& c, const point4& d, int count );

// This version, which is broken at the moment,
// produces a warped tetra, which is neato!
void divide_tetra_alt( const point4& a, const point4& b,
		       const point4& c, const point4& d, int count );

void quad( int a, int b, int c, int d );

void quad( const point4 &a, const point4 &b,
	   const point4 &c, const point4 &d,
	   const color4 &A, const color4 &B,
	   const color4 &C, const color4 &D );

void cube( const GLfloat &size, const color4 colors[8] );

void colorcube( void );

void colorcube( GLfloat size );

// Model loader

void load_obj( const char* filename, vector<vec4> &vertices,
	       vector<vec3> &normals, vector<GLushort> &v_elements,
	       vector<GLushort> &n_elements);

#endif
