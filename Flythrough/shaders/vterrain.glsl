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


void main() {

     gl_Position = CTM * vPosition;
     color = vColor;
     outtexture = vTex;
     fPosition = vPosition;

}
