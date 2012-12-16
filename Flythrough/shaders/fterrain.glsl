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

uniform sampler2D gSampler0; // Dirt
uniform sampler2D gSampler1; // Sand
uniform sampler2D gSampler2; // Grss
uniform sampler2D gSampler3; // Rock
uniform sampler2D gSampler4; // Snow

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
    float gradupper1 = 13.0 ; // Between here..
    float gradlower1 = 11.0 ; // and here is Snow->Rock
        // Rock is between ^^ THESE vv values
    float gradupper2 =  8.0 ; // Between here...
    float gradlower2 =  6.0 ; // and here is Rock->grass
        // Grass is between ^^ THESE vv values
    float gradupper3 =  3.0 ; // Between here...
    float gradlower3 =  0.5 ; // and here is Grass->Sand
        // Sand is between ^^ THESE vv values
    float gradupper4 = -1.0 ; // Between here...
    float gradlower4 = -3.0 ; // and here is Sand->dirt

    // Snow!
    if ( fPosition.y > gradupper1 )
        gl_FragColor = texture2D( gSampler4, outtexture ); 
    // Snow->Rock
    else if ((fPosition.y > gradlower1) && (fPosition.y < gradupper1) )
	gl_FragColor = textureGradient( gSampler4, gSampler3, gradupper1, gradlower1);
    // Rock
    else if ((fPosition.y > gradupper2) && (fPosition.y <= gradlower1)) 
        gl_FragColor = texture2D( gSampler3, outtexture );
    // Rock->Grass
    else if ((fPosition.y > gradlower2) && (fPosition.y < gradupper2))
        gl_FragColor = textureGradient( gSampler3, gSampler2, gradupper2, gradlower2 );
    // Grass
    else if (( fPosition.y > gradupper3 ) && (fPosition.y <= gradlower2 ))
        gl_FragColor = texture2D( gSampler2, outtexture );  
    // Grass->Sand
    else if ((fPosition.y > gradlower3) && (fPosition.y < gradupper3))
        gl_FragColor = textureGradient( gSampler2, gSampler1, gradupper3, gradlower3 );
    // Sand
    else if ((fPosition.y > gradupper4 ) && (fPosition.y <= gradlower3 ))
        gl_FragColor = texture2D( gSampler1, outtexture );  
    // Sand->Dirt
    else if ((fPosition.y > gradlower4) && (fPosition.y < gradupper4))
        gl_FragColor = textureGradient( gSampler1, gSampler0, gradupper4, gradlower4 );
    // Dirt
    else if ( fPosition.y <= gradlower4 )
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