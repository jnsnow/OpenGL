varying vec4 color;
varying vec2 outtexture;

varying vec4 fPosition;

uniform sampler2D gSampler0;
uniform sampler2D gSampler1;
uniform sampler2D gSampler2;


vec4 textureGradient( sampler2D a, sampler2D b, float upper, float lower )
{
    float diff = upper - lower ;
    return ((texture2D( a, outtexture) * (fPosition.y - lower)) +
            (texture2D( b, outtexture) * (upper - fPosition.y))) / diff ;

}

void main() 
{

    // Snow!
    if ( fPosition.y > 6.0 )
        gl_FragColor = texture2D( gSampler2, outtexture ); 

    // Snow->Rock
    else if ((fPosition.y > 4.0) && (fPosition.y < 6.0) )
	gl_FragColor = textureGradient( gSampler2, gSampler1, 6.0, 4.0);

    // Rock
    else if ((fPosition.y > 0.0) && (fPosition.y <= 4.0)) 
        gl_FragColor = texture2D( gSampler1, outtexture );

    // Rock->Grass
    else if ((fPosition.y > -2.0) && (fPosition.y < 0.0))
        gl_FragColor = textureGradient( gSampler1, gSampler0, 0.0, -2.0 );

    // Grass
    else if ( fPosition.y <= -2.0 )
        gl_FragColor = texture2D( gSampler0, outtexture );  

    //    gl_FragColor = color;

}