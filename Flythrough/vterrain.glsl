// attributes
attribute vec4 vPosition;
// sent to the fshader
varying vec4 color;

// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 CTM; // CTM is either PRT or TRP, depending.

varying float color_scale;

void main() {
     gl_Position = CTM * vPosition;
     color_scale = 0.5 * vPosition.y;     
     color = color_scale * vec4( 0.65, 0.31, 0.0, 1.0 );
     //color = vec4( 0,0,0,1);
}
