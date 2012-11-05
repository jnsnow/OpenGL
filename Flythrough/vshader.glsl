attribute vec4 vPosition;
attribute vec4 vColor;
varying   vec4 color;

uniform vec4 glsl_trans;
uniform mat4 glsl_pers;
uniform mat4 glsl_camrot;

void main() {

   vec4 trans_vec;
   color = vColor;
   trans_vec = glsl_trans;
   trans_vec.w = 0.0;

   gl_Position = (((vPosition - trans_vec) * glsl_camrot) * glsl_pers);


}
