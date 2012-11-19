attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec3 vNormal;
varying   vec4 color;

// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 PRT;

// color/lighting
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;

varying vec4 finalLight;


void main() {


  // phong model for lighting.
  // ... I think.

    vec3 pos = (normalize(R * T * vPosition)).xyz;

    vec3 L = normalize( ( R * (T * LightPosition)).xyz - pos );
    vec3 E = normalize( -pos );
    vec3 H = normalize( L + E );


    // Transform vertex normal into eye coordinates
    vec3 N = normalize( R*T*vec4(vNormal, 0.0) ).xyz;


    // Compute terms in the illumination equation
    vec4 ambient = AmbientProduct;


    float Kd = max( dot(L, N), 0.0 );
    vec4  diffuse = Kd*DiffuseProduct;


    float Ks = pow( max(dot(N, H), 0.0), Shininess );
    vec4  specular = Ks * SpecularProduct;


    if( dot(L, N) < 0.0 ) {
    specular = vec4(0.0, 0.0, 0.0, 1.0);
    }


   // POSITION STUFF

   //gl_Position = P * R * T * vPosition;
   //gl_Position = vPosition * T * R * P;
   gl_Position = PRT * vPosition;


   // COLOR/LIGHTING

    vec3 lightsum = vec3( (ambient + diffuse + specular).xyz) ;


    // This right here is how we take the original color of the surface into consideration.
    // We should talk about a high level way to manage this. Lighting will require some organized code.
    finalLight = vec4( vColor.x * lightsum.x,
        	       vColor.y * lightsum.y,
		       vColor.z * lightsum.z,
		       1.0  ) ;

/*
    color = vec4( vColor.x * lightsum.x,
    	    	  vColor.y * lightsum.y,
		  vColor.z * lightsum.z,
		  1.0  ) ;
*/


   color = vColor;
   //color.a = 1.0;

}
