///////////////////////////////////////////////////////////////////////////////

uniform bool      fIsTextured;
uniform sampler2D gSampler0; // Dirt
uniform sampler2D gSampler1; // Sand
uniform sampler2D gSampler2; // Grss
uniform sampler2D gSampler3; // Rock
uniform sampler2D gSampler4; // Snow
uniform vec3      lightColor[8];
uniform vec3      LightDirectionArray[8];
uniform vec3      LightPositionArray[8];
uniform int       numLights ;
uniform float     terrainMag;
//uniform vec3 LightAmbientArray[] ;
//uniform vec3 LightDiffuseArray[] ;
//uniform vec3 LightSpecularArray[];

varying vec3 cameraVector;    
varying vec4 color;           // in from vShader
varying vec4 fPosition;       // in from vShader
varying vec3 fragmentNormal;
varying vec3 lightVector[8];
varying vec2 outtexture;      // in from vshader

const float MAX_DIST = 2.5;
const float MAX_DIST_SQUARED = MAX_DIST * MAX_DIST;

float dirtUpper;
float grassUpper;
float grassLower;
float rockUpper;
float rockLower;
float sandUpper;
float sandLower;
float snowLower;

vec4 textureGradient( sampler2D a, sampler2D b, float upper, float lower )
{
    float diff = upper - lower ;
    return ((texture2D( a, outtexture) * (fPosition.y - lower)) +
            (texture2D( b, outtexture) * (upper - fPosition.y))) / diff ;
}

void setTextureLimits( float mag ){

    snowLower  =  0.2     * mag;
    rockUpper  =  0.1675  * mag;
    rockLower  =  0.15    * mag;
    grassUpper =  0.08    * mag;
    grassLower =  0.0375  * mag;
    sandUpper  =  0.00625 * mag;
    sandLower  = -0.0125  * mag;
    dirtUpper  = -0.0375  * mag;
}

void main() 
{

  if (fIsTextured) {

     setTextureLimits( terrainMag );    

    // Snow!
    if ( fPosition.y > snowLower )
        gl_FragColor = texture2D( gSampler4, outtexture ); 
    // Snow->Rock
    else if ((fPosition.y > rockUpper) && (fPosition.y < snowLower) )
	gl_FragColor = textureGradient( gSampler4, gSampler3, snowLower, rockUpper);
    // Rock
    else if ((fPosition.y > rockLower) && (fPosition.y <= rockUpper)) 
        gl_FragColor = texture2D( gSampler3, outtexture );
    // Rock->Grass
    else if ((fPosition.y > grassUpper) && (fPosition.y < rockLower))
        gl_FragColor = textureGradient( gSampler3, gSampler2, rockLower, grassUpper );
    // Grass
    else if (( fPosition.y > grassLower ) && (fPosition.y <= grassUpper ))
        gl_FragColor = texture2D( gSampler2, outtexture );  
    // Grass->Sand
    else if ((fPosition.y > sandUpper) && (fPosition.y < grassLower))
        gl_FragColor = textureGradient( gSampler2, gSampler1, grassLower, sandUpper );
    // Sand
    else if ((fPosition.y > sandLower ) && (fPosition.y <= sandUpper ))
        gl_FragColor = texture2D( gSampler1, outtexture );  
    // Sand->Dirt
    else if ((fPosition.y > dirtUpper) && (fPosition.y < sandLower))
        gl_FragColor = textureGradient( gSampler1, gSampler0, sandLower, dirtUpper );
    // Dirt
    else if ( fPosition.y <= dirtUpper )
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