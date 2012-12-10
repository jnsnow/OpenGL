/* oh god please no */
/* no               */
/* nooooooooooo     */

#include "globals.h"
#include "Object.hpp"
#include "Timer.hpp"
#include "vec.hpp"
#include <cmath>
using namespace Angel;
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

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

void createPoint( Object *obj, point4 const &the_point, 
		  color4 const &the_color, vec3 const &the_normal ) {

  obj->points.push_back( the_point );
  obj->colors.push_back( the_color );
  obj->normals.push_back( the_normal );

}

/** Functions relating to the Sierpinski Pyramid **/

void triangle( Object *obj, const point4& a, const point4& b, 
	       const point4& c, const int color ) {

  static vec4  base_colors[] = {
    vec4( 1.0, 0.0, 0.0, 1.0 ), /* R */
    vec4( 0.0, 1.0, 0.0, 1.0 ), /* G */
    vec4( 0.0, 0.0, 1.0, 1.0 ), /* B */
    vec4( 1.0, 1.0, 0.0, 1.0 ), /* Y */
    vec4( 0.4, 0.1, 0.8, 1.0 )  /* P */
  };

  // Initialize temporary vectors along the quad's edge to
  //   compute its face normal

  vec3 u = normalize(vec3(b.x-a.x, b.y-a.y, b.z-a.z)) ;
  vec3 v = normalize(vec3(c.x-b.x, c.y-b.y, c.z-b.z)) ;

  vec3 normal = normalize( cross(u, v) );

  createPoint( obj, a, base_colors[ color ], normal );
  createPoint( obj, b, base_colors[ color ], normal );
  createPoint( obj, c, base_colors[ color ], normal );
  
}

void tetra( Object *obj,
	    const point4& a, const point4& b, 
	    const point4& c, const point4& d ) {

  triangle( obj, a, b, c, 0 );
  triangle( obj, a, c, d, 1 );
  triangle( obj, a, d, b, 2 );  // old   triangle( a, d, b, 2 );
  triangle( obj, d, c, b, 3 );

}

void Sierpinski_Pyramid( Object *obj,
			 const point4& a,
			 const point4& b,
			 const point4& c,
			 const point4& d,
			 int count ) {
  if ( count > 0 ) {
    point4 v0 = ( a + b ) / 2.0;
    point4 v1 = ( a + c ) / 2.0;
    point4 v2 = ( a + d ) / 2.0;
    point4 v3 = ( b + c ) / 2.0;
    point4 v4 = ( c + d ) / 2.0;
    point4 v5 = ( b + d ) / 2.0;
    Sierpinski_Pyramid( obj, a, v0, v1, v2, count - 1 );
    Sierpinski_Pyramid( obj, v0,  b, v3, v5, count - 1 );
    Sierpinski_Pyramid( obj, v1, v3,  c, v4, count - 1 );
    Sierpinski_Pyramid( obj, v2, v5, v4,  d, count - 1 );
  } else {
    tetra( obj, a, b, c, d );   // draw tetrahedron at end of recursion
  }
}

//--------------------------------------------------------------------

void quad( Object *obj, 
	   const point4 &a, const point4 &b,
	   const point4 &c, const point4 &d,
	   const color4 &A, const color4 &B,
	   const color4 &C, const color4 &D ) {


  // Initialize temporary vectors along the quad's edge to                                                                                 
  //   compute its face normal                                                                                                             
  vec4 u = b - a;
  vec4 v = c - b;

  vec3 normal = normalize( cross(u, v) );

  createPoint( obj, a, A, normal );
  createPoint( obj, b, B, normal );
  createPoint( obj, c, C, normal );

  createPoint( obj, a, A, normal );
  createPoint( obj, c, C, normal );
  createPoint( obj, d, D, normal );

}

//--------------------------------------------------------------------

#define QUAD(A,B,C,D)							\
  quad( obj, vertices[A], vertices[B], vertices[C], vertices[D],	\
	colors[A], colors[B], colors[C], colors[D] );

void cube( Object *obj, const GLfloat &size, const color4 colors[8] ) {

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

void colorcube( Object *obj, GLfloat size ) {

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

  cube( obj, size, vertex_colors );

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



float rand_float( void ) {
  return rand() / (float)RAND_MAX;
}

double jitter( double H ) {
  return (-H) + rand() * (H - (-H)) / RAND_MAX;
}

void landGen( Object *obj, int N, float H ) {

  Timer Tick;
  const int S = pow(2,N) + 1;
  std::vector< point4 > &vec = obj->points;
  std::vector< point4 > &col = obj->colors;
  std::vector< unsigned int > &drawIndex = obj->indices;

  if (DEBUG) printf( "\nEntering landGen()...\n");
  // the range (-h -> h) for the average offset
  // This determines the jaggedness of the peaks and valleys.
  // A smaller initial value will create smaller peaks and shallow valleys for
  // a look more similar to rolling hill tops.
  double CH = 0.5;
  double magnitude = (H*(2 - pow(2,-(N))));
  if (DEBUG) fprintf( stderr, "landGen theoretical magnitude: %f\n", magnitude );

  /* Initialize all points in the vector to have their X,Z (and w) coordinates. */
  if (vec.size()) vec.clear();
  vec.reserve( S * S );
  for ( int i = 0; i < S; ++i )
    for ( int j = 0; j < S; ++j ) 
      vec.push_back( vec4( j - (S/2), 0, i - (S/2), 1 ) );

  /* Initialize our color vectors. */
  if (col.size()) col.clear();
  col.reserve( S*S );
  for ( int i = 0; i < S; ++i )
    for ( int j = 0; j < S; ++j )
      col.push_back( vec4( 0.5, 0.5, 0.5, 1.0 ) );

  /* Simulate a 2D array in this 1D array. Use these Macros to help. */
#define OffsetAt(X,Z) ((X)*S+(Z))
#define VertexAt(X,Z) (vec.at(OffsetAt(X,Z)))
#define HeightAt(X,Z) (VertexAt(X,Z).y)
#define ColorAt(X,Z) (col.at(OffsetAt(X,Z)))

  // Initialize the corners of the grid
  HeightAt( 0, 0 )     = 0;
  HeightAt( S-1, 0 )   = 0;
  HeightAt( 0, S-1 )   = 0;
  HeightAt( S-1, S-1 ) = 0;

  // Populate the (x, y, z) values of vec4 according to the
  // Diamond-Square algorithm
  // sideLength is the distance of a single square side or
  // distance of diagonal in diamond.
  if (DEBUG) printf("\nEntering for( sideLength...) ...\n");
  for (int sideLength = S-1; sideLength >= 2; sideLength /= 2, H /= 2.0, CH /= 2.0) { 
    int halfSide = sideLength / 2;
    // generate new square values
    for ( int x = 0 ; x < S-1 ; x += sideLength ) {
      for ( int z = 0 ; z < S-1 ; z += sideLength) { 
	double avg =						\
	  (HeightAt( x, z ) + HeightAt( x + sideLength, z ) +
	   HeightAt( x, z + sideLength ) + HeightAt( x + sideLength, z + sideLength ))/4.0;
	
	vec4 color_avg =				\
	  (ColorAt(x,z) + ColorAt(x+sideLength,z) +
	   ColorAt(x,z+sideLength) + ColorAt(x+sideLength,z+sideLength))/4.0;
	vec4 color_jitter = vec4( jitter(CH), jitter(CH), jitter(CH), 0 );
	
	HeightAt( x + halfSide, z + halfSide ) = avg + jitter(H);
	ColorAt( x + halfSide, z + halfSide ) = color_avg + color_jitter;
      } // for z
    } // for x
   
    // Generate the diamond values
    // Since diamonds are staggered, we only move x by half side
    // NOTE: If the data shouldn't wrap the x < SIZE and y < SIZE
    for ( int x = 0 ; x < S-1 ; x += halfSide ) {
      for (int z = ( x+halfSide ) % sideLength ; z < S-1 ; z += sideLength ) {
 
        // x,z is center of diamond
        // We must use mod and add SIZE for subtraction
        // so that we can wrap around the array to find the corners

	double avg = 
	  (HeightAt( (x-halfSide + S) % S, z ) +
	   HeightAt( x + halfSide % S, z ) +
	   HeightAt( x, z + halfSide % S ) +
	   HeightAt( x, (z - halfSide + S) % S )) / 4.0;

	vec4 color_avg = 
	  (ColorAt( (x-halfSide + S) % S, z ) +
	   ColorAt( x + halfSide % S, z ) +
	   ColorAt( x, z + halfSide % S ) +
	   ColorAt( x, (z - halfSide + S) % S )) / 4.0;
	vec4 color_jitter = vec4( jitter(CH), jitter(CH), jitter(CH), 0 );
 
	HeightAt( x, z ) = avg + jitter(H);
	ColorAt( x, z ) = color_avg + color_jitter;

	// Wrapping:
	if (x == 0) {
	  HeightAt( S-1, z ) = HeightAt( x, z );
	  ColorAt( S-1, z ) = ColorAt( x, z );
	}
	if (z == 0) {
	  HeightAt( x, S-1 ) = HeightAt( x, z );
	  ColorAt( x, S-1 ) = ColorAt( x, z );
	}
      } // for z
    } // for x
  } // for sideLength


  //vec.reserve(2*S*S - 2*S);
  // Convert 2D array into 1D array to pass to shaders
  // Size of 1D array is 2*SIZE^2 - 2SIZE for
  // the sake of proper parsing by glTriangleStrips
  for ( int i = 0, j = 0 ; i + 1 < S ; i++ ) {
    for ( j = 0 ; j < S ; j++ ) {
      drawIndex.push_back(OffsetAt(i,j));
      drawIndex.push_back(OffsetAt(i+1,j));
    }

    /* If we're out of rows to serialize, give up. */
    if (++i == S) break;

    for ( ; j > 0 ; j-- ){
      drawIndex.push_back(OffsetAt(i,j-1));
      drawIndex.push_back(OffsetAt(i+1,j-1));
    }
  }

  for ( int i = 0; i < S*S; i++ ) {
    col.push_back(vec4(rand_float(),rand_float(),rand_float(),1.0));
  }

  Tick.Tock();
  if (DEBUG)
    fprintf( stderr, "Landgen took %lu usec, %f msec, %f sec to generate %d vertices.\n", 
	     Tick.Delta(), Tick.Delta()/1000.0, Tick.Delta()/1000000.0, S*S );

  return;
}
