// attributes
attribute vec4 vPosition;
// sent to the fshader
varying vec4 color;

// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 CTM; // CTM is either PRT or TRP, depending.

varying float color_scale ;

void main() {


     float distanceX;
     float distanceY;
     float distanceZ;
     float distance;

     gl_Position = CTM *  vPosition;
     color_scale = 0.25 * (vPosition.y+1.0);     

     //color = vec4( 0,0,0,1);

     distanceX =(T[0][3] < 0.0) ? T[0][3] + vPosition.x :  T[0][3] - vPosition.x ;
     distanceY =(T[1][3] < 0.0) ? T[1][3] + vPosition.y :  T[1][3] - vPosition.y ;
     distanceZ =(T[2][3] < 0.0) ? T[2][3] + vPosition.z :  T[2][3] - vPosition.z ;

     distance  = (distanceX*distanceX) + 
//     	       	 (distanceY*distanceY) ;
		 (distanceZ*distanceZ) ;

     distance  = sqrt(distance);

     // this transforms it into a falloff
     //distance = 1.0 - distance;


     distance = 10.0 / distance ;

     //color = color - or *?  vec4(distance, distance, distance, 0.0);
     color =  vec4(distance, distance, distance, 0.0);


     color = color  * color_scale * vec4( 0.625, 0.375, 0.01, 1.0 ) ;
//     color = color  * color_scale * vec4( 1.0, 1.0, 0.0, 1.0 ) ;



     if( color.x < 0.0 ) color.x = 0.0;
     if( color.y < 0.0 ) color.y = 0.0;
     if( color.z < 0.0 ) color.z = 0.0;


}


// NOTHING t00 crazy
// just lighting falloff
