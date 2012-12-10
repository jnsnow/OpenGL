// GENERIC VSHADER
// Adadapted from this magnificent web site
// http://ogldev.atspace.co.uk/www/tutorial20/tutorial20.html

// attributes
attribute vec4 vPosition ;
attribute vec4 vNormal   ;

// sent to the fshader
varying vec4 color;
varying vec3 cameraVector;

// position/movement
uniform mat4 P   ;
uniform mat4 R   ;
uniform mat4 T   ;
uniform mat4 CTM ; // CTM is either PRT or TRP, depending.

// Lighting 
uniform int numLights ;
uniform vec3 LightPositionArray[] ;


varying vec3 LightVector[numLights];


void main() {

     int i;

     for ( i = 0 ; i < numLights ; i ++ ) {

     // BWAH BULLET

          // http://3.bp.blogspot.com/-Sr2O0lM68ZM/TV5aVB1jF7I/AAAAAAAAAvw/e1OO6avilKU/s1600/swallowtail_caterpillar_big.jpg
	  
	  LightVector[i] = LightPositionArray[i] - vPosition.xyz ;

     }

     cameraVector = (R * vec4(0.0,0.0,1.0,1.0)).xyz;
     gl_Position = CTM * vPosition ;

}
