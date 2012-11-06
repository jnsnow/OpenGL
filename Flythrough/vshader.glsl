attribute vec4 vPosition;
attribute vec4 vColor;
varying   vec4 color;

uniform vec4 glsl_trans;
uniform mat4 glsl_pers;
uniform mat4 glsl_camrot;
uniform mat4 glsl_ctm;

void main() {

   color = vColor;
   gl_Position = glsl_pers * glsl_ctm * vPosition;

}
