///////////////////////////////////////////////////////////////////////////////

// sent to the fshader
varying vec4 color;
varying vec3 cameraVector;
varying vec3 fragmentNormal;
varying vec3 lightVector[8];

const float MAX_DIST = 2.5;
const float MAX_DIST_SQUARED = MAX_DIST * MAX_DIST;

uniform int numLights ;

  uniform vec3 lightColor[8];
//uniform vec3 LightAmbientArray[] ;
//uniform vec3 LightDiffuseArray[] ;
//uniform vec3 LightSpecularArray[];

uniform vec3 LightPositionArray[8];
uniform vec3 LightDirectionArray[8];

varying vec2 outtexture;
varying vec4 fPosition;

uniform sampler2D gSampler0;
uniform sampler2D gSampler1;
uniform sampler2D gSampler2;

uniform bool fIsTextured;

vec4 textureGradient( sampler2D a, sampler2D b, float upper, float lower )
{
    float diff = upper - lower ;
    return ((texture2D( a, outtexture) * (fPosition.y - lower)) +
            (texture2D( b, outtexture) * (upper - fPosition.y))) / diff ;
}

void main() 
{

  if (fIsTextured) {

    float gradupper1 =   7.0 ;
    float gradlower1 =   4.0 ;
    float gradupper2 =  -2.0 ;
    float gradlower2 =  -5.0 ;


    // Snow!
    if ( fPosition.y > gradupper1 )
        gl_FragColor = texture2D( gSampler2, outtexture ); 

    // Snow->Rock
    else if ((fPosition.y > gradlower1) && (fPosition.y < gradupper1) )
	gl_FragColor = textureGradient( gSampler2, gSampler1, gradupper1, gradlower1);

    // Rock
    else if ((fPosition.y > gradupper2) && (fPosition.y <= gradlower1)) 
        gl_FragColor = texture2D( gSampler1, outtexture );

    // Rock->Grass
    else if ((fPosition.y > gradlower2) && (fPosition.y < gradupper2))
        gl_FragColor = textureGradient( gSampler1, gSampler0, gradupper2, gradlower2 );

    // Grass
    else if ( fPosition.y <= gradlower2 )
        gl_FragColor = texture2D( gSampler0, outtexture );  

    }

    else gl_FragColor = color;




/*
 // else gl_FragColor = color4(0.0, 0.0, 0.0, 1.0) ;


    // deal with all the lights, right now.

     int i;
     vec3 diffuse, specular;
     vec3 normal, cameraDir;

     float dist, distFactor;

     vec3 lightDir, halfAngle, specularColor ;

     float specularDot ;
     float diffuseDot  ;

     // initialize diffuse/specular lighting
     diffuse  = vec3(0.0, 0.0, 0.0);
     specular = vec3(0.0, 0.0, 0.0);

     // normalize the fragment normal and camera direction
     normal = normalize(fragmentNormal);
     cameraDir = normalize(cameraVector);


     vec4 sample ;

     // loop through each light
     for ( i = 0; i < numLights && i < 8; ++i) {

       // calculate distance between 0.0 and 1.0
       dist = min(dot(lightVector[i], lightVector[i]), MAX_DIST_SQUARED) / MAX_DIST_SQUARED;
       distFactor = 1.0 - dist;

       // diffuse
       lightDir = normalize(lightVector[i]);
       diffuseDot = dot(normal, lightDir);
       diffuse += lightColor[i] * clamp(diffuseDot, 0.0, 1.0) * distFactor;

       // specular
       halfAngle = normalize(cameraDir + lightDir);
       specularColor = min(lightColor[i] + 0.5, 1.0);
       specularDot = dot(normal, halfAngle);
       specular += specularColor * pow(clamp(specularDot, 0.0, 1.0), 16.0) * distFactor;
     }

     sample = vec4(1.0, 1.0, 1.0, 1.0);
     gl_FragColor = gl_FragColor * vec4(clamp(sample.rgb * ( diffuse  ) + specular, 0.0, 1.0), sample.a) ;
     gl_FragColor = gl_FragColor * vec4(clamp(sample.rgb * ( diffuse  ) + specular, 0.0, 1.0), sample.a) ;
*/

}