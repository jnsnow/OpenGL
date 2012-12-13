varying vec4 color;
varying vec2 outtexture;

varying vec4 fPosition;

uniform sampler2D gSampler0;
uniform sampler2D gSampler1;
uniform sampler2D gSampler2;

void main() 
{

    // Snow!
    if ( fPosition.y > 5.0 ){	
        gl_FragColor = texture2D( gSampler2, outtexture );
    } 
    // Snow/Rock
    else if ((fPosition.y > 4.0) && (fPosition.y < 5.0) ){
      	gl_FragColor =
	   ( ((texture2D( gSampler2, outtexture ) * (fPosition.y - 4.0)) + 
	      (texture2D( gSampler1, outtexture ) * (5.0 - fPosition.y))));
    } 
    // Rock
    else if ((fPosition.y > 0.0) && (fPosition.y <= 4.0)) {
        gl_FragColor = texture2D( gSampler1, outtexture );
    } 

    // Rock/Grass
    else if ((fPosition.y > -2.0) && (fPosition.y < 0.0)){
        gl_FragColor =
	   ( ((texture2D( gSampler1, outtexture ) * (fPosition.y + 2.0) / 2.0 ) + 
	      (texture2D( gSampler0, outtexture ) * ( -fPosition.y) / 2.0)));
    }
    // Grass
    else if ( fPosition.y <= 0.0 ){
        gl_FragColor = texture2D( gSampler0, outtexture );  
    }

    //    gl_FragColor = color;

}