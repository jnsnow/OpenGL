/* oh god please no */
/* no               */
/* nooooooooooo     */

#include "vec.hpp"
using namespace Angel;

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#ifndef __CUBE__ // Globals for Pyramid Model
const int NumTimesToSubdivide = 4;
const int NumTetrahedrons = 4*4*4*4;//intPow(4, NumTimesToSubdivide);
const int NumTriangles = 4*NumTetrahedrons;
const int NumVertices = 3*NumTriangles+6;
#endif
#ifdef __CUBE__ // Globals for Cube Model 
const int NumVertices = 36;
#endif

point4 points[NumVertices];
color4 colors[NumVertices];
vec3  normals[NumVertices];

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


using std::vector       ;
using std::fstream      ;
using std::ifstream     ;
using std::istringstream;
using std::string       ;
using std::ios          ;
using std::cerr         ;
using std::endl         ;


/*
#include <vector>
#include <ifstream>
#include <stream>
#include <string>*/


void createPoint( point4 const &the_point, color4 const &the_color, vec3 const &the_normal ) {
  static int Index = 0;
  points[Index]  = the_point ;
  colors[Index]  = the_color ;
  normals[Index] = the_normal;//vec3( 0.0, 1.0, 0.0);//the_normal;
  Index++;
}

//--------------------------------------------------------------------

void triangle( const point4& a, const point4& b, 
	       const point4& c, const int color ) {

  static vec4  base_colors[] = {
    vec4( 1.0, 0.0, 0.0, 1.0 ), /* R */
    vec4( 0.0, 1.0, 0.0, 1.0 ), /* G */
    vec4( 0.0, 0.0, 1.0, 1.0 ), /* B */
    vec4( 1.0, 1.0, 0.0, 1.0 ),  /* Y */
    vec4( 0.4, 0.1, 0.8, 1.0 )  /* P */
  };

  // Initialize temporary vectors along the quad's edge to
  //   compute its face normal


  vec3 u = normalize(vec3(b.x-a.x, b.y-a.y, b.z-a.z)) ;
  vec3 v = normalize(vec3(c.x-b.x, c.y-b.y, c.z-b.z)) ;

  vec3 normal = normalize( cross(u, v) );

  createPoint( a, base_colors[ color ], normal );
  createPoint( b, base_colors[ color ], normal );
  createPoint( c, base_colors[ color ], normal );
  
}

//--------------------------------------------------------------------

void tetra( const point4& a, const point4& b, 
	    const point4& c, const point4& d ) {

  triangle( a, b, c, 0 );
  triangle( a, c, d, 1 );
  triangle( a, d, b, 2 );  // old   triangle( a, d, b, 2 );
  triangle( d, c, b, 3 );

}

//--------------------------------------------------------------------

void divide_tetra( const point4& a, const point4& b,
		   const point4& c, const point4& d, int count ) {


  if ( count > 0 ) {
    point4 v0 = ( a + b ) / 2.0;
    point4 v1 = ( a + c ) / 2.0;
    point4 v2 = ( a + d ) / 2.0;
    point4 v3 = ( b + c ) / 2.0;
    point4 v4 = ( c + d ) / 2.0;
    point4 v5 = ( b + d ) / 2.0;
    divide_tetra(  a, v0, v1, v2, count - 1 );
    divide_tetra( v0,  b, v3, v5, count - 1 );
    divide_tetra( v1, v3,  c, v4, count - 1 ); // \/ \/ bug fixed
    divide_tetra( v2, v5, v4,  d, count - 1 ); // divide_tetra( v2, v4, v5,  d, count - 1 );

 
  } else {
    tetra( a, b, c, d );   // draw tetrahedron at end of recursion
  }

}

void divide_tetra_alt( const point4& a, const point4& b,
		       const point4& c, const point4& d, int count ) {
  point4 a2 = a/2.0;

  if ( count > 0 ) {
    point4 v0 = ( a2 + b ) / 2.0;
    point4 v1 = ( a2 + c ) / 2.0;
    point4 v2 = ( a2 + d ) / 2.0;
    point4 v3 = ( b + c ) / 2.0;
    point4 v4 = ( c + d ) / 2.0;
    point4 v5 = ( b + d ) / 2.0;
    divide_tetra_alt(  a2, v0, v1, v2, count - 1 );
    divide_tetra_alt( v0,  b, v3, v5, count - 1 );
    divide_tetra_alt( v1, v3,  c, v4, count - 1 ); // \/ \/ bug fixed
    divide_tetra_alt( v2, v5, v4,  d, count - 1 ); // divide_tetra( v2, v4, v5,  d, count - 1 );


  } else {
    tetra( a2, b, c, d );   // draw tetrahedron at end of recursion
  }

}

//--------------------------------------------------------------------

void quad( int a, int b, int c, int d ) {

  static const point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
  };

  static const color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
  };


  // Initialize temporary vectors along the quad's edge to                                                                                 
  //   compute its face normal                                                                                                             
  vec4 u = vertices[b] - vertices[a];
  vec4 v = vertices[c] - vertices[b];

  vec3 normal = normalize( cross(u, v) );

  createPoint( vertices[a], vertex_colors[a], normal );
  createPoint( vertices[b], vertex_colors[b], normal );
  createPoint( vertices[c], vertex_colors[c], normal );

  createPoint( vertices[a], vertex_colors[a], normal ); // if we did this right, we don't
  createPoint( vertices[c], vertex_colors[c], normal ); // need to recompute the normal 
  createPoint( vertices[d], vertex_colors[d], normal ); // because it is on the same plane, 
                                                // and therefore equivalent.

}

void quad( const point4 &a, const point4 &b,
	   const point4 &c, const point4 &d,
	   const color4 &A, const color4 &B,
	   const color4 &C, const color4 &D ) {


  // Initialize temporary vectors along the quad's edge to                                                                                 
  //   compute its face normal                                                                                                             
  vec4 u = b - a;
  vec4 v = c - b;

  vec3 normal = normalize( cross(u, v) );

  createPoint( a, A, normal );
  createPoint( b, B, normal );
  createPoint( c, C, normal );

  createPoint( a, A, normal );
  createPoint( c, C, normal );
  createPoint( d, D, normal );

}

//--------------------------------------------------------------------

#define QUAD(A,B,C,D)						\
  quad( vertices[A], vertices[B], vertices[C], vertices[D],	\
	colors[A], colors[B], colors[C], colors[D] );

void cube( const GLfloat &size, const color4 colors[8] ) {

  const GLfloat lower = -(size/2);
  const GLfloat upper = (size/2);

  const point4 vertices[8] = {
    point4( lower, lower, upper, 1.0 ),
    point4( lower, upper, upper, 1.0 ),
    point4( upper, upper, upper, 1.0 ),
    point4( upper, lower, upper, 1.0 ),
    point4( lower, lower, lower, 1.0 ),
    point4( lower, upper, lower, 1.0 ),
    point4( upper, upper, lower, 1.0 ),
    point4( upper, lower, lower, 1.0 )
  };
  
  QUAD( 1, 0, 3, 2 );
  QUAD( 2, 3, 7, 6 );
  QUAD( 3, 0, 4, 7 );
  QUAD( 6, 5, 1, 2 );
  QUAD( 4, 5, 6, 7 );
  QUAD( 5, 4, 0, 1 );
  
}

// generate 12 triangles: 36 vertices and 36 colors
void colorcube() { 
  quad( 1, 0, 3, 2 );
  quad( 2, 3, 7, 6 );
  quad( 3, 0, 4, 7 );
  quad( 6, 5, 1, 2 );
  quad( 4, 5, 6, 7 );
  quad( 5, 4, 0, 1 );
}

void colorcube( GLfloat size ) {

  static const color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
  };

  cube( size, vertex_colors );

}

//--------------------------------------------------------------------

/**
 model loader:
 Shamelessly ripped straight from the Simiandroid's main source file!

  .obj model loader
  models must be triangulated, and have normals included to be
  correctly parsed
  vertices, normals, vertex elements, and normal elements will be returned to pointers to vectors
*/


/*
  Basic function to split strings over a delimiter
  used in model parsing
*/
vector<string> split(string str, char delim)
{
  vector<string> *elements = new vector<string>();

  // loop through and break string up by the deliminator
  while (str.find_first_of(delim) != string::npos)
    {
      // position of deliminator in current string
      int pos = str.find_first_of(delim);

      elements->push_back(str.substr(0, pos));

      str = str.substr(pos + 1);
    }

  // str will have the final element remaining in it
  elements->push_back(str);

  return *elements;
}

void load_obj(const char* filename, vector<vec4> &vertices,
	      vector<vec3> &normals, vector<GLushort> &v_elements,
	      vector<GLushort> &n_elements)
{
  // file input stream
  ifstream in(filename, ios::in);

  if (!in)
    {
      cerr << "Cannot open " << filename << endl;
      exit(1);
    }

  string line;

  while (getline(in, line))
    {
      // lines beginning with 'v ' have vertices
      if (line.substr(0, 2) == "v ")
	{
	  // read in vertex coords
	  istringstream s(line.substr(2));
	  vec4 v;
	  s >> v.x;
	  s >> v.y;
	  s >> v.z;
	  v.w = 1.0;
	    
	  vertices.push_back(v);
	}
      // lines beginning with 'f ' have elements
      else if (line.substr(0, 2) == "f ")
	{
	  istringstream s(line.substr(2));
	  string a, b, c;
	  s >> a;
	  s >> b;
	  s >> c;

	  // split elements by '/'
	  v_elements.push_back(atoi(split(a, '/')[0].c_str()));
	  v_elements.push_back(atoi(split(b, '/')[0].c_str()));
	  v_elements.push_back(atoi(split(c, '/')[0].c_str()));

	  n_elements.push_back(atoi(split(a, '/')[2].c_str()));
	  n_elements.push_back(atoi(split(b, '/')[2].c_str()));
	  n_elements.push_back(atoi(split(c, '/')[2].c_str()));
	}
      // lines beginning with 'vn ' will have normals
      else if (line.substr(0, 3) == "vn ") {
	istringstream s(line.substr(3));
	vec3 vn;
	s >> vn.x;
	s >> vn.y;
	s >> vn.z;
	  
	normals.push_back(vn);
      }
      else if (line[0] == '#') { /* ignore comments */ }
      else { /* blank/junk */ }
    }

}


