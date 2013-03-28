/**
 @file terrain.cpp
 @authors John Huston, Nicholas StPierre, Chris Compton
 @date 2013-02-23
 @brief This is a trimmed version of our Fall 2012 project.
 @details This is a tech demo for terrain generation using an udpated
 engine derived from Ed Angel's code from his book.
 **/

// System Headers
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>
// Multi-platform support and OpenGL headers
#include "platform.h"
#include "globals.h"
// Ed Angel's Math Classes
#include "vec.hpp"
#include "mat.hpp"
// Utilities and Classes
#include "Engine.hpp"
#include "Camera.hpp"
#include "Cameras.hpp"
#include "Screen.hpp"
#include "Object.hpp"
#include "Timer.hpp"
#include "Scene.hpp"
// Utilities
#include "model.hpp"
#include "InitShader.hpp"
#include "glut_callbacks.h"
#include "ObjLoader.hpp"

// Type Aliases
using Angel::vec3;
using Angel::vec4;

// Wii Connectivity 
#ifdef WII
#include <wiicpp.h>  // WiiC++ API
#include "WiiUtil.h" // Wii Controller Handler Util
CWii Wii;
bool usingWii = false;
#endif

// Textures
// Obtained from www.goodtextures.com
const char* terrainTex[] = { "../Textures/GoodTextures_0013423.jpg", // Dirt/Mud
    "../Textures/GoodTextures_0013779.jpg",  // Sand
    "../Textures/GrassGreenTexture0002.jpg", // Grass (who'da thunk?)
    "../Textures/GoodTextures_0013418.jpg",  // Rock
    "../Textures/GoodTextures_0013291.jpg"   // Snow
    };

/**
 randomize_terrain is called to regenerate the terrain in this application.
 It assumes the terrain is named "terrain" and is located as a direct
 child of the main scene graph.

 @return void.
 **/
void randomize_terrain() {
  
  // Final project demo used the equivalent of terrain_size = 10.
  const size_t terrain_size = 8;
  float H = fmod( (float) random(), 20 * terrain_size ) + 5 * terrain_size;
  
  srand( time( NULL ) );
  
  Object *Terrain = (*Engine::instance()->rootScene())["terrain"];
  double magnitude = landGen( Terrain, terrain_size, H );
  Terrain->buffer();
  GLint handle = glGetUniformLocation( Terrain->shader(), "terrainMag" );
  if ( handle != -1 ) glUniform1f( handle, magnitude );
  
}

/**
 init will initialize this particular flythrough,
 by creating and instantiating a shader, a camera,
 and a number of initial objects.

 @return void.
 **/
void init() {
  
  Scene *theScene = Engine::instance()->rootScene();
  Screen *myScreen = Engine::instance()->mainScreen();
  
  // Load the shaders.
  GLuint gShader = Angel::InitShader( "shaders/vterrain.glsl",
                                      "shaders/fterrain.glsl" );
  // Initialize engine setting: Enable "fixed yaw"... disable free-Y rotation.
  Engine::instance()->opt( "fixed_yaw", true );
  
  // Give the shader handle to the Scene Graph and the Camera List.
  theScene->shader( gShader );
  myScreen->_camList.shader( gShader );
  
  // Cameras must be added after setting a shader.
  myScreen->_camList.addCamera( "Camera1" );
  myScreen->_camList.next();
  
  /*
   NOTE:
   ==========================
   Please do not give the Terrain children.
   The terrain generation animation applies directly to the terrain,
   which of course propegates the animation to all child objects.

   You'll pancake any child objects.
   */

  // Let's create some objects.
  Object *terrain = theScene->addObject( "terrain" );
  glUseProgram( gShader );// Temporary hack until I refine the texturing management subsystem.
  terrain->texture( terrainTex );
  terrain->drawMode( GL_TRIANGLE_STRIP );
  randomize_terrain();// This call depends upon "terrain" existing within theScene.
  
  Object *pyramid = theScene->addObject( "pyramid" );
  
  /*
   Object *box   = theScene.addObject( "box" ) ;
   loadModelFromFile( box, "../models/box-a.obj");
   box->_trans.offset.SetY(100.0);
   box->_trans.offset.SetX(100.0);
   box->_buffer();
   */

  sierpinskiPyramid( pyramid, vec4( 0, 1, 0, 1 ), vec4( -1, -0.999, 1, 1 ),
                     vec4( 1, -0.999, 1, 1 ), vec4( 0, -0.999, -1, 1 ), 4 );
  pyramid->buffer();
  
  Object *cube_base = theScene->addObject( "basecube" );
  colorCube( cube_base, 1.0 );
  cube_base->buffer();
  
  Object *moon_cube = pyramid->addObject( "moon" );
  colorCube( moon_cube, 0.5 );
  moon_cube->buffer();
  
  // These models came from VALVE,
  // From their game "Team Fortress 2."
  // The model processing was done in Blender.
  Object *heavy = theScene->addObject( "heavy" );
  ObjLoader::loadModelFromFile( heavy, "../models/heavyT.obj" );
  heavy->buffer();
  heavy->_trans.scale.Set( 0.10 );
  heavy->_trans.offset.Set( 0, 2, 0 );
  
  // Valve's TF2 Medic
  Object *medic = heavy->addObject( "medic" );
  ObjLoader::loadModelFromFile( medic, "../models/medicT.obj" );
  medic->_trans.offset.Set( 0, 20, 0 );
  medic->buffer();
  
  // Valve's TF2 Spy
  Object *spy = medic->addObject( "spy" );
  ObjLoader::loadModelFromFile( spy, "../models/spyT.obj" );
  spy->_trans.offset.Set( 0, 20, 0 );
  spy->buffer();
  
  Object *ball = theScene->addObject( "ball" );
  sphere( ball );
  ball->_trans.scale.Set( 1000 );
  ball->buffer();
  
  /*
   Object *sun = theScene.addObject ("sun");
   //loadModelFromFile( sun, "../models/heavyT.obj" );
   sphere(sun);
   sun->_trans.offset.SetX(500.0);
   sun->_trans.scale.Set(16.0) ;
   sun->_buffer();
   */

  /*
   Object *actualMoon = theScene.addObject ("actualMoon");
   //loadModelFromFile( actualMoon, "../models/spyT.obj" );
   sphere(actualMoon);
   actualMoon->_trans.offset.SetX(-500.0);
   actualMoon->_trans.scale.Set(12.0);
   actualMoon->_buffer();
   */

  // The water gets generated last -- In order for our fake transparency to work.
  Object *agua = theScene->addObject( "agua" );
  makeAgua( terrain, agua );
  agua->buffer();
  agua->drawMode( GL_TRIANGLES );
  
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.3, 0.5, 0.9, 1.0 );
  
  //Attach a model to the Camera.
  Object *cam = myScreen->_camList.active();
  ObjLoader::loadModelFromFile( cam, "../models/rainbow_dashT.obj" );
  // http://kp-shadowsquirrel.deviantart.com/		
  //   art/Pony-Model-Download-Center-215266264
  cam->buffer();
  cam->drawMode( GL_TRIANGLES );
  cam->_trans.scale.Set( 0.05 );
  cam->_trans.PreRotation.RotateY( 180 );
  cam->propagate();
  
  // Add the propagate method to the Scene Graph directly, instead of this:
  // Note: Terrain doesn't/shouldn't have children ...
  terrain->propagate();
  
}

/**
 cleanup is a routine to call at exit time that will free up the
 resources the application is using.
 While not critical, it does aid in using debuggers to not have
 any memory leaks at exit time.
 
 @return void.
 **/
void cleanup( void ) {
  
  //Engine::instance()->rootScene()->DestroyObject();
  
}

/** 
 displayViewport is responsible for drawing a single viewport.

 @return void.
 **/
void displayViewport( void ) {
  
  // draw free-floating objects
  Engine::instance()->rootScene()->draw();
  // draw camera-attached objects
  Engine::instance()->cams()->draw();
  
}

/**
 display is responsible for drawing an entire screen.

 @return void.
 **/
void display( void ) {
  // Clear the screen and begin rendering.
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // Tell camList to draw using our displayViewport rendering function.
  Engine::instance()->cams()->view( displayViewport );
  
  // Utilize multi-buffering.
  glutSwapBuffers();
}

// This global switch controls if we generate terrain or not.
bool switchingTerrain = false;

/**
 TerrainGenerationAnimation is an animation callback that will:
 (A) If triggered, begin to shrink the object into a flat plane,
 (B) Order a re-generation of the terrain data
 (C) will grow the object back into its new, final shape.

 @param obj A reference to an object's transformation state.

 @return void.
 **/
void TerrainGenerationAnimation( TransCache &obj ) {
  
  typedef enum {
    SHRINKING, TRANSITIONING, GROWING, DONE
  } AnimationState;
  static AnimationState terrainState = DONE;
  static float CurrentScale = 0.0;
  
  if ( switchingTerrain ) {
    terrainState = SHRINKING;
    switchingTerrain = false;
    CurrentScale = 0.0;
  }
  
  switch ( terrainState ) {
  
  case SHRINKING:
    obj.scale.Set( 1.0,
                   ((1.0 + cos( CurrentScale * DEGREES_TO_RADIANS )) / 2.0),
                   1.0 );
    
    CurrentScale += 1.0 * Tick.Scale();
    
    if ( CurrentScale >= 180.0 ) {
      terrainState = TRANSITIONING;
    }
    break;
    
  case TRANSITIONING:
    // Multithread this for graphics 2 project
    randomize_terrain();
    terrainState = GROWING;
    break;
    
  case GROWING:
    obj.scale.Set( 1.0,
                   ((1.0 + cos( CurrentScale * DEGREES_TO_RADIANS )) / 2.0),
                   1.0 );
    
    CurrentScale += 1.0 * Tick.Scale();
    
    if ( CurrentScale >= 360.0 ) terrainState = DONE;
    break;
    
  case DONE:
    return;
    
  default:
    return;
  }
}

/**
 wiilook is an analog of mouselook, for wii remote controls.
 It takes a reference to a Camera, and two vec3s,
 and uses the information to adjust the Camera's rotation.

 @param WiiCamera The camera to adjust the rotation of.
 @param NewTheta  The X,Y,Z angles of the Wii Remote.
 @param MovementRates The X, Y, Z angular velocities of the Wii Remote.

 @return Void.
 **/
void wiilook( Camera &WiiCamera, const Angel::vec3 &NewTheta,
              const Angel::vec3 &MovementRates ) {
  
  static Angel::vec3 OldTheta; /* Defaults to 0,0,0 */
  // Rotation Order: Y-X-Z looks the best, I think.
  //WiiCamera.yaw( NewTheta.y - OldTheta.y );
  //WiiCamera.pitch( OldTheta.x - NewTheta.x );
  //WiiCamera.roll( NewTheta.z - OldTheta.z );
  
  float yaw = -MovementRates.y / 20;
  float pitch = -MovementRates.x / 20;
  float roll = MovementRates.z / 20;
  
  if ( abs( yaw ) >= 0.1 )
    WiiCamera.yaw( -MovementRates.y / 20,
                   Engine::instance()->opt( "fixed_yaw" ) );
  if ( abs( pitch ) >= 0.1 ) WiiCamera.pitch( -MovementRates.x / 20 );
  if ( abs( roll ) >= 0.1 ) WiiCamera.roll( MovementRates.z / 20 );
  
  OldTheta = NewTheta;
  
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
  if ( 0 ) fprintf( stderr, "Timescale: %f\n", timeScale );
  
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

/// hackity hack hack hackey doo!
float heightScale = 0.0;
float ticker = 0.0;

void idle( void ) {
  
  Scene &theScene = (*Engine::instance()->rootScene());
  
  Tick.Tock();
  
  if ( DEBUG_MOTION )
    fprintf( stderr, "Time since last idle: %lu\n", Tick.Delta() );
  
  Object &Terrain = *(theScene["terrain"]);
  Object &Pyramid = *(theScene["pyramid"]);
  Pyramid.animation( animationTest );
  Pyramid["moon"]->animation( simpleRotateAnim );
  
  Object &Heavy = *(theScene["heavy"]);
  Object &Medic = *(Heavy["medic"]);
  Object &Spy = *(Medic["spy"]);
  Heavy.animation( simpleRotateAnim );
  Medic.animation( simpleRotateY );
  Spy.animation( simpleRotateY );
  
  if ( Engine::instance()->opt( "terrain_regen" ) ) {
    fprintf( stderr, "terrain_regen on, turning on switchingTerrain bool\n" );
    switchingTerrain = true;
    Engine::instance()->opt( "terrain_regen", false );
  }
  
  Terrain.animation( TerrainGenerationAnimation );
  
#ifdef WII
  if (usingWii) {
    static const unsigned NumPolls = 20;
    Camera *camptr = dynamic_cast< Camera* >( myScreen._camList["AutoCamera2"] );
    Angel::vec3 theta_diff;
    Angel::vec3 accel_mag;

    // Take many samples for two reasons:
    // (1) Without this, we can't poll often enough and Wii Input "lags".
    // (2) Average/Sample to "smooth" the data.
    for (size_t i = 0; i < NumPolls; ++i) {
      pollWii( Wii );
      if (PollResults.Reset_Camera && camptr != NULL) camptr->resetRotation();
      theta_diff += PollResults.wr_thetas;
      accel_mag += PollResults.bb_magnitudes;
    }

    if (camptr) {
      camptr->Accel( (accel_mag / NumPolls) * 2.0 );
      wiilook( *camptr, theta_diff / NumPolls, PollResults.wr_rates );
    }

  }
#endif
  
  // Move all camera(s).
  Engine::instance()->cams()->idleMotion();
  glutPostRedisplay();
  
}

//--------------------------------------------------------------------

void menufunc( int value ) {
  
  Engine *EN = Engine::instance();
  Scene *theScene = EN->rootScene();
  
  switch ( value ) {
  case 0:
    landGen( (*theScene)["terrain"], 12, 40.0 );
    (*theScene)["terrain"]->buffer();
    break;
  case 1:
    EN->flip( "fixed_yaw" );
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
  Angel::InitInitShader( argv[0] );
  
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  //glutInitWindowSize( 0, 0 );
  glutCreateWindow( "Terrain" );
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
  
  if ( DEBUG ) {
    fprintf( stderr, "GL_VENDOR: %s\n", glGetString( GL_VENDOR ) );
    fprintf( stderr, "GL_RENDERER: %s\n", glGetString( GL_RENDERER ) );
    fprintf( stderr, "GL_VERSION: %s\n", glGetString( GL_VERSION ) );
    fprintf( stderr, "GL_SHADING_LANGUAGE_VERSION: %s\n",
             glGetString( GL_SHADING_LANGUAGE_VERSION ) );
    fprintf( stderr, "GL_EXTENSIONS: %s\n", glGetString( GL_EXTENSIONS ) );
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
