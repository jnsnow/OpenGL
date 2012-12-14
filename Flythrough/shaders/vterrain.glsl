// attributes
attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec2 vTex;

// sent to the fshader
varying vec4 color;
varying vec4 fPosition;
varying vec2 outtexture;

// The below samplers are not needed in this shader.. tested and confirmed.
//uniform sampler2D gSampler0;
//uniform sampler2D gSampler1;
//uniform sampler2D gSampler2;

// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 CTM; // CTM is either PRT or TRP, depending.

// Lighting, dammit.
uniform int numLights ;
uniform vec3 LightPositionArray[8] ;

// fshader lighting vars
varying vec3 cameraVector;
varying vec3 fragmentNormal;
varying vec3 lightVector[8];


void main() {

     gl_Position = CTM * vPosition;
     color = vColor;
     outtexture = vTex;
     fPosition = vPosition;

     int i;

     for ( i = 0 ; i < numLights && i < 8 ; i ++ ) {
          lightVector[i] = LightPositionArray[i] - vPosition.xyz ;
     }

     //fragmentNormal = vNormal ;

     cameraVector = (R * vec4(0.0,0.0,1.0,1.0)).xyz;
     fragmentNormal = vec3(0.0,1.0,0.0) ;


}
