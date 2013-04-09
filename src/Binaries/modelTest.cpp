/**
 * @file ds.cpp
 * @author John Huston
 * @authors John Huston, Greg Giannone
 * @date 2013-02-20
 * @brief Dual-shader demo.
 * @details Work in progress!
 * Based loosely on Ed Angel's tutorials.
 **/

/* Engine Classes */
#include "Engine.hpp"
/* Utilities and Common */
#include "model.hpp"
#include "InitShader.hpp"
#include "glut_callbacks.h"
#include "ObjLoader.hpp"

const char *modelname;

/**
 * Initialization: load and compile shaders, initialize camera(s), load models.
 */
void init() {
  
  GLuint shader[2];
  Cameras *camList = Engine::instance()->cams();
  Scene *rootScene = Engine::instance()->rootScene();
  
  shader[0] = Angel::InitShader( "shaders/vterrain.glsl", "shaders/ftex.glsl" );
  
  camList->addCamera( "Camera1" );
  camList->next();
  
  // Setting a default and adding objects without:
  rootScene->shader( shader[0] );

  Object *model = rootScene->addObject( "Arbitrary Model" );
  ObjLoader::loadModelFromFile( model, modelname );

  vec4 min = model->getMin();
  vec4 max = model->getMax();
  fprintf( stderr, "Min: (%f,%f,%f)\nMax: (%f,%f,%f)\n", 
	   min.x, min.y, min.z,
	   max.x, max.y, max.z );
  model->_trans._offset.set( 0, -min.y, 0 );
  model->propagate();
  model->buffer();

  Object *floor = rootScene->addObject( "floor" );
  quad( floor, 
	vec4( -10, 0, 10, 1.0 ), vec4( -10, 0, -10, 1.0),
	 vec4( 10, 0, -10, 1.0 ),vec4( 10, 0, 10, 1.0 ),
	vec4( 0.4, 0.4, 0.4, 0.9 ) );
  floor->buffer();

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0, 0, 0, 1.0 );
}

/**
 * Cleans up our scene graph.
 */
void cleanup( void ) {
  //Engine::instance()->rootScene()->DestroyObject();
}

//--------------------------------------------------------------------

/**
 * This is a dual-shader demo! It looks very simple,
 * but it illustrates quickly and effectively how to use two shaders.
 *
 * @param argc Not used.
 * @param argv Not used.
 * @return EXIT_SUCCESS.
 *
 */
int main( int argc, char **argv ) {
  
  if (argc > 1) modelname = argv[1];
  else modelname = "../models/candle.obj";

  Engine::init( &argc, argv, "Model Demonstration" );
  init();

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
