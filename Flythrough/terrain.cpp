/**
   @file terrain.cpp
   @authors John Huston, Nicholas StPierre, Chris Compton
   @date 2012-12-06
   @brief This is a derivative of our main project file, fly.cpp.
   @details This is a tech demo for terrain generation using an udpated
   engine derived from fly.cpp, which was mostly based on Ed Angel's code
   from his book.
**/

#include "globals.h"
/* System Headers */
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>
/* Multi-platform support and OpenGL headers. */
#include "platform.h"
/* Ed Angel's Math Classes */
#include "vec.hpp"
#include "mat.hpp"
/* Utilities and Classes */
#include "model.hpp"
#include "Camera.hpp"
#include "InitShader.hpp"
#include "Cameras.hpp"
#include "Screen.hpp"
#include "Object.hpp"
#include "Timer.hpp"
#include "Scene.hpp"

#include "LightSource.hpp"
#include "Lights.hpp"

#include "OpenGL.h"

// Type Aliases
using Angel::vec3;
using Angel::vec4;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

// Wii Connectivity 
#ifdef WII
#include <wiicpp.h>  // WiiC++ API
#include "WiiUtil.h" // Wii Controller Handler Util
CWii Wii;
bool usingWii = false;
#endif
////

Screen myScreen( 800, 600 );
Scene theScene;
GLuint gShader;
bool fixed_yaw = true;

// #lights
Lights lights(false); // Bool controls the lighting mode. False indicates the simpler, faster one.

// Textures
// Obtained from www.goodtextures.com
const char* terrainTex[] = {
  "../Textures/GoodTextures_0013423.jpg",  // Dirt/Mud
  "../Textures/GoodTextures_0013779.jpg",  // Sand
  "../Textures/GrassGreenTexture0002.jpg", // Grass (who'da thunk?)
  "../Textures/GoodTextures_0013418.jpg",  // Rock
  "../Textures/GoodTextures_0013291.jpg"   // Snow
};

void randomize_terrain() {
  
  Object *Terrain = theScene["terrain"];
  // 8, 40
  double magnitude = landGen( Terrain, 8, 40.0 );
  Terrain->Buffer();

  GLint handle = glGetUniformLocation( gShader, "terrainMag" );
  if (handle != -1) glUniform1f( handle, magnitude );


}


void init() {
  srand(time(NULL));
  
  // Load shaders and use the resulting shader program. 
  gShader = Angel::InitShader( "shaders/vterrain.glsl", "shaders/fterrain.glsl" );
  theScene.SetShader( gShader );
  myScreen.camList.SetShader( gShader );
  myScreen.camList.AddCamera( "Camera1" );
  myScreen.camList.Next();

  // initialize lights

  lights.init_lights(gShader);

  /*
    SUPER IMPORTANT NOTE DAMMIT
    ==========================
    Please do not give the Terrain children. 
    The animation is currently scaling the Y values of the 
    terrain to grow/shrink it.
    Any and all Children will inherit this.
    I've turned rainbow dash into a pancake over and over....
    Poor little pony....
  */

  /* Create some Objects ... */
  Object *terrain   = theScene.AddObject( "terrain" ) ;
  terrain->Texture( terrainTex );
  terrain->Mode( GL_TRIANGLE_STRIP );
  randomize_terrain();
  
  Object *pyramid   = theScene.AddObject( "pyramid" ) ;
  Sierpinski_Pyramid( pyramid,
		      vec4(  0,      1,  0, 1 ),
		      vec4( -1, -0.999,  1, 1 ),
		      vec4(  1, -0.999,  1, 1 ),
		      vec4(  0, -0.999, -1, 1 ),
		      4 );
  pyramid->Buffer();

  Object *cube_base = theScene.AddObject( "basecube" );
  colorcube( cube_base, 1.0 );
  cube_base->Buffer();
  
  Object *moon_cube = pyramid->AddObject( "moon" )    ;
  colorcube( moon_cube, 0.5 );
  moon_cube->Buffer();

  // These models came from VALVE,
  // From their game "Team Fortress 2."
  // The model processing was done in Blender.
  Object *heavy     = theScene.AddObject( "heavy" ) ;
  loadModelFromFile( heavy, "../models/heavyT.obj" );
  heavy->Buffer();
  heavy->trans.scale.Set( 0.10 );
  heavy->trans.offset.Set( 0, 2, 0 );

  // Valve's TF2 Medic
  Object *medic     = heavy->AddObject( "medic" );
  loadModelFromFile( medic, "../models/medicT.obj" );
  medic->trans.offset.Set( 0, 20, 0 );
  medic->Buffer();

  // Valve's TF2 Spy
  Object *spy       = medic->AddObject( "spy" );
  loadModelFromFile( spy, "../models/spyT.obj" );
  spy->trans.offset.Set( 0, 20, 0 );
  spy->Buffer();

  Object *ball = theScene.AddObject( "ball" );
  sphere( ball );
  ball->trans.scale.Set( 1000 );
  ball->Buffer();

  Object *sun = theScene.AddObject ("sun");
  //loadModelFromFile( sun, "../models/heavyT.obj" );
  sphere(sun);
  sun->trans.offset.Set(80.0);
  sun->trans.scale.Set(10.0) ;
  sun->Buffer();

  Object *actualMoon = theScene.AddObject ("actualMoon");
  //loadModelFromFile( actualMoon, "../models/spyT.obj" );
  sphere(actualMoon);
  actualMoon->trans.offset.Set(-80.0);
  actualMoon->trans.scale.Set(8.0);
  actualMoon->Buffer();


  //  for ( int z = 0 ; z < 2 ; z++ ){

    lights.addLightSource( LightSource( point4(0.0, 30.0, 0.0, 1.0), 
					color4(1.0, 1.0, 1.0, 1.0)));

    lights.addLightSource( LightSource( point4(0.0, -1.0, 0.0, 1.0), 
					color4(1.0, 1.0, 1.0, 1.0)));

    lights.addLightSource( LightSource( point4(10.0, 10.0, 10.0, 1.0), 
					color4(1.0, 1.0, 1.0, 1.0)));

    //    lights.addLightSource( LightSource( point4(0.0, 0.0, 0.0, 1.0), 
    //				color4(1.0, 1.0, 1.0, 1.0)));
    

  /*
  Object *pony      = terrain->AddObject( "pony" ) ;  
  // http://kp-shadowsquirrel.deviantart.com/		
  //   art/Pony-Model-Download-Center-215266264
  loadModelFromFile( pony, "../models/rainbow_dashT.obj" );
  pony->trans.offset.Set( 2, 2, 2 );
  pony->trans.scale.Set( 0.2 );
  pony->Buffer();
  */
  
  // The water gets generated last.
  Object *agua      = terrain->AddObject( "agua" )    ;
  makeAgua( terrain, agua ) ;
  agua->Buffer();
  agua->Mode( GL_TRIANGLES );

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.3, 0.5, 0.9, 1.0 );

 
  /*
  //Attach a model to the Camera.
  Object *cam = myScreen.camList.Active();
  loadModelFromFile( cam, "../models/rainbow_dashT.obj" );
  cam->Buffer();
  cam->Mode( GL_TRIANGLES );
  cam->trans.scale.Set( 0.05 );
  //cam->trans.PreOffset.Set( 0, 0, 0.2 );
  //cam->trans.rotation.RotateY( 180 );
  //cam->Propegate(); 
  */

  terrain->Propegate(); // UH BUT UH UH UH TERRAIN SHOULD NOT HAVE KIDS IT ISNT GOOD WITH KIDS

}

float timeOfDay = 0.0 ; // If we are going for day and night

void cleanup( void ) {

  theScene.DestroyObject();

}

//--------------------------------------------------------------------

/** A function that takes no arguments.
    Is responsible for drawing a SINGLE VIEWPORT. **/
void displayViewport( void ) {  
  theScene.Draw(); /* Draw free-floating objects */
  myScreen.camList.Draw(); /* Draw camera-attached objects */
}

void display( void ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  lights.sendAll();
  // Tell camList to draw using our displayViewport rendering function.
  myScreen.camList.View( displayViewport );
  glutSwapBuffers();
}

void keylift( unsigned char key, int x, int y ) {

  if (myScreen.camList.NumCameras() < 1) return;
  
  Camera &cam = *(myScreen.camList.Active());

  switch( key ) {
  case 'w':
    cam.Stop( Camera::Forward );
    break;
  case 's':
    cam.Stop( Camera::Backward );
    break;
  case 'a':
    cam.Stop( Camera::Left );
    break;
  case 'd':
    cam.Stop( Camera::Right );
    break;
  case 'q':
    cam.Stop( Camera::Up );
    break;
  case 'e':
    cam.Stop( Camera::Down );
    break;
  }
}



typedef enum {

  SHRINKING,
  TRANSITIONING,
  GROWING,
  DONE

} TSTATE ;

TSTATE terrainState = SHRINKING ;

// These belong to MakeFlatToRegular()
bool switchingTerrain = false ;
float CurrentScale = 0.0;

void MakeFlatToRegular( TransCache &obj ) {

  if ( switchingTerrain ) {

    terrainState = SHRINKING ;
    switchingTerrain = false ;
    CurrentScale = 0.0       ;
  }

  //float f ;
  switch ( terrainState ) {

  case SHRINKING:

    //    f = CurrentScale / 180 ;
    obj.scale.Set(1.0,
		  ((1.0+cos(CurrentScale*DegreesToRadians))/2.0),
		  1.0);
    
    CurrentScale += 1.0 * Tick.Scale() ;

    if ( CurrentScale >= 180.0 ) {
      terrainState = TRANSITIONING ;
    }

    break;


  case TRANSITIONING:

    // Multithread this for graphics 2 project
    randomize_terrain();
    //landGen( theScene["terrain"], 8, 40.0 );
    //theScene["terrain"]->Buffer();
    terrainState = GROWING ;
    // CurrentScale = 0.0 ;
    break;

  case GROWING:


    //f = CurrentScale/360.0;

    obj.scale.Set(1.0,
		  ((1.0+cos(CurrentScale*DegreesToRadians))/2.0),
		  1.0);

    CurrentScale += 1.0 * Tick.Scale() ;

    if ( CurrentScale >= 360.0 ) terrainState = DONE ;
    break;


  case DONE:
    return;


  default: 
    return;

  }

    // bottom

}

void keyboard( unsigned char key, int x, int y ) {

  switch( key ) {

  case 033: // Escape Key	  
    cleanup();
    glutLeaveMainLoop();
    break;

  case ';': // Print Info
    fprintf( stderr, "Active Object: %s\n",
	     theScene.Active()->Name().c_str() );
    break;

  case 'l':
    switchingTerrain = true ; 
    // GLOBAL State variable used to control the terrain animation
    //randomize_terrain();
    break;
    
  case '+':
    std::stringstream camName;
    camName << "AutoCamera" << myScreen.camList.NumCameras() + 1;
    myScreen.camList.AddCamera( camName.str() );
    break;
  }

  if (myScreen.camList.NumCameras() < 1) return;

  /* A shorthand variable with local scope that refers to "The Active Camera." */
  Camera &cam = *(myScreen.camList.Active());
  
  switch( key ) {
  case '-':
    myScreen.camList.PopCamera();
    break;
  case ';':
    fprintf( stderr, "Camera Position: (%f,%f,%f)\n", cam.X(), cam.Y(), cam.Z() );
    break;
    
  case 'w':
    cam.Move( Camera::Forward );
    break;
  case 's':
    cam.Move( Camera::Backward );
    break;
  case 'a':
    cam.Move( Camera::Left );
    break;
  case 'd':
    cam.Move( Camera::Right );
    break;
  case 'q':
    cam.Move( Camera::Up );
    break;
  case 'e':
    cam.Move( Camera::Down );
    break;
    
    //Perspectives
  case 'z': cam.changePerspective( Camera::PERSPECTIVE ); break;
  case 'x': cam.changePerspective( Camera::ORTHO ); break;
  case 'c': cam.changePerspective( Camera::ORTHO2D ); break;
  case 'v': cam.changePerspective( Camera::FRUSTUM ); break;
  case 'b': cam.changePerspective( Camera::IDENTITY ); break;

 // case 't': lights.addLightSource( randomLight() /*! not defined !*/ ) ; break;

  }
}

void keyboard_ctrl( int key, int x, int y ) {

  switch (key) {
    //Cycle between Active Objects ...
  case GLUT_KEY_LEFT:
    theScene.Prev();
    break;
  case GLUT_KEY_RIGHT:
    theScene.Next();
    break;

    //Change the Draw Mode ...
  case GLUT_KEY_F1:
    theScene.Active()->Mode( GL_POINTS );
    break;
  case GLUT_KEY_F2:
    theScene.Active()->Mode( GL_LINE_STRIP );
    break;
  case GLUT_KEY_F3:
    theScene.Active()->Mode( GL_TRIANGLE_STRIP );
    break;
  case GLUT_KEY_F4:
    theScene.Active()->Mode( GL_TRIANGLES );
    break;
  }

  // If there are no Cameras, don't muck around with this section.
  if (myScreen.camList.NumCameras() < 1) return;

  switch( key ) {
  case GLUT_KEY_PAGE_UP:
    myScreen.camList.Prev();
    break;

  case GLUT_KEY_PAGE_DOWN:
    myScreen.camList.Next();
    break;
  }
}

void mouse( int button, int state, int x, int y ) {

  if (myScreen.camList.NumCameras() < 1) return;

  if ( state == GLUT_DOWN ) {
    switch( button ) {
    case 3: myScreen.camList.Active()->dFOV( 1 ); break;
    case 4: myScreen.camList.Active()->dFOV( -1 ); break;
    }
  }

}


void mouseroll( int x, int y ) {

  if ((x != myScreen.MidpointX()) || (y != myScreen.MidpointY())) {
    if (myScreen.camList.NumCameras() > 0)
      myScreen.camList.Active()->roll( x - myScreen.MidpointX() );
    glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );
  }

}


void mouselook( int x, int y ) {

  if ((x != myScreen.MidpointX()) || (y != myScreen.MidpointY())) {
    const double dx = ((double)x - myScreen.MidpointX());
    const double dy = ((double)y - myScreen.MidpointY());
    
    if (myScreen.camList.NumCameras() > 0) {
      myScreen.camList.Active()->pitch( dy );
      myScreen.camList.Active()->yaw( dx, fixed_yaw );
    }    

    glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );
  }
  
}


void resizeEvent( int width, int height ) {

  /* Handles resizing the child cameras as well. */
  myScreen.Size( width, height );
  glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );

}


void simpleRotateY( TransCache &obj ) {
  obj.rotation.RotateY( Tick.Scale() * 1.5 ); 
}



void simpleRotateAnim( TransCache &obj ) {

  obj.rotation.RotateY( Tick.Scale() * 1.5 );
  obj.offset.Set( 1.5, 0, 0 );
  obj.orbit.RotateY( Tick.Scale() * -1.0 );

}
  

void animationTest( TransCache &obj ) {

  double timeScale = Tick.Scale();
  double theta = timeScale * 0.1;
  if (0) fprintf( stderr, "Timescale: %f\n", timeScale );

  //Object increasingly grows. 
  /* Note that, Scaling Adjustment, unlike Rotation and Translation,
     is a multiplicative adjustment, which means that
     we can't just multiply by our time scaling factor,
     we have to take pow( scaleFactor, timeScale ) instead.
     This is, of course, incredibly inefficient. */
  // obj.scale.Adjust( pow( 1.001, timeScale ) );

  //Object rotates in-place.
  obj.rotation.RotateX( theta );
  obj.rotation.RotateY( theta );
  obj.rotation.RotateZ( theta );

  obj.offset.Set( 5, 0, 0 );

  //Object increasingly moves away from origin, x += 0.01
  //obj.offset.Delta( timeScale * 0.01, 0, 0 );

  //Object orbits about the origin
  obj.orbit.RotateX( timeScale * 0.2 );
  obj.orbit.RotateY( timeScale * 0.2 );
  obj.orbit.RotateZ( timeScale * 0.2 );

  // Object moves its focal orbit-point, x = 5.
  //obj.displacement.Set( 5, 0, 0 );
  
}


// TOD --> T.ime O.f D.ay
void sunOrbit(TransCache &sun ) {
  static float TOD = 0.0 ;
  sun.orbit.RotateZ( TOD * 0.01 );
  TOD += 0.01;
  if ( TOD > 360.0 ) TOD = 0.0;
}

void moonOrbit(TransCache &moon) {
  static float TOD = 0.0 ;
  moon.orbit.RotateZ( TOD * 0.01);
  TOD += 0.01;
  if ( TOD > 360.0 ) TOD = 0.0;
}


void idle( void ) {

  Tick.Tock();
  if (DEBUG_MOTION) 
    fprintf( stderr, "Time since last idle: %lu\n", Tick.Delta() );


  Object &Terrain = *(theScene["terrain"]);
  Object &Pyramid = *(theScene["pyramid"]);
  Pyramid.Animation( animationTest );
  Pyramid["moon"]->Animation( simpleRotateAnim );

  Object &Heavy = *(theScene["heavy"]);
  Object &Sun   = *(theScene["sun"]);
  Object &Moon  = *(theScene["actualMoon"]);
  Object &Medic = *(Heavy["medic"]);
  Object &Spy   = *(Medic["spy"]);
  Heavy.Animation( simpleRotateAnim );
  Medic.Animation( simpleRotateY );
  Spy.Animation( simpleRotateY );
  Terrain.Animation( MakeFlatToRegular );

  Sun.Animation(  sunOrbit  );
  Moon.Animation( moonOrbit );

  // Move light absolutely, ie to the point given
  lights.moveLight( 0, Sun.GetPosition() ) ;



#ifdef WII
  if (usingWii) {
    static const unsigned NumPolls = 20;
    Camera *camptr = dynamic_cast< Camera* >( myScreen.camList["AutoCamera2"] );
    for (size_t i = 0; i < NumPolls; ++i) {
      pollWii( Wii );
      //Returns and sets bb_magnitudes.
      if (camptr) camptr->Accel( bb_magnitudes / NumPolls );
    }
  }
#endif

  // Move all camera(s).
  myScreen.camList.IdleMotion();
  glutPostRedisplay();

}


//--------------------------------------------------------------------

void menufunc( int value ) {

  switch (value) {
  case 0:
    landGen( theScene["terrain"], 12, 40.0 );
    theScene["terrain"]->Buffer();
    break;
  case 1:
    if (fixed_yaw) fixed_yaw = false;
    else fixed_yaw = true;
    break;
  }
    
}

int main( int argc, char **argv ) {

#ifdef WII
  if (!(usingWii = initWii( Wii ))) {
    std::cerr << "Not using Wii controls for this runthrough.\n";
  }
#endif

  // OS X suppresses events after mouse warp.  This resets the suppression 
  // interval to 0 so that events will not be suppressed. This also found
  // at http://stackoverflow.com/questions/728049/
  // glutpassivemotionfunc-and-glutwarpmousepointer
#ifdef __APPLE__
  CGSetLocalEventsSuppressionInterval( 0.0 );
#endif

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( myScreen.Width(), myScreen.Height() );
  glutCreateWindow( "Gasket Flythrough" );
  glutFullScreen();
  glutSetCursor( GLUT_CURSOR_NONE );

  GLEW_INIT();
  init();

  /* Register our Callbacks */
  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard );
  glutKeyboardUpFunc( keylift );
  glutSpecialFunc( keyboard_ctrl );
  glutMouseFunc( mouse );
  glutMotionFunc( mouseroll );
  glutPassiveMotionFunc( mouselook );
  glutIdleFunc( idle );
  glutReshapeFunc( resizeEvent );

  if (DEBUG) {
    fprintf( stderr, "GL_VENDOR: %s\n", glGetString( GL_VENDOR ));
    fprintf( stderr, "GL_RENDERER: %s\n", glGetString( GL_RENDERER ));
    fprintf( stderr, "GL_VERSION: %s\n", glGetString( GL_VERSION ));
    fprintf( stderr, "GL_SHADING_LANGUAGE_VERSION: %s\n", 
	     glGetString( GL_SHADING_LANGUAGE_VERSION ));
  }

  int menu = glutCreateMenu( menufunc );
  glutSetMenu( menu );
  glutAddMenuEntry( "Randomize Terrain", 0 );
  glutAddMenuEntry( "Toggle Free Rotation", 1 );
  glutAttachMenu( GLUT_RIGHT_BUTTON );

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;

}
