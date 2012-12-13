#include <string>
#include <map>
#include <list>

#include "Scene.hpp"
#include "Object.hpp"

typedef std::pair< std::string, Object * > mapping;

Scene::Scene() {
  currentObj = list.begin();
}

Scene::~Scene() {
  /* Traverse the list and free all Objects. */
}

Object *Scene::AddObject( std::string objName ) {

  Object *obj = new Object( objName, gShader );
  list.push_back( obj );
  map.insert( mapping( objName, obj ) );
  return obj;

}

void Scene::SetShader( GLuint gShader ) {
  this->gShader = gShader;
}

GLuint Scene::GetShader( void ) {
  return gShader;
}

/**
   DeleteObject is the actual implementation function that will
   remove an Object from the Scene list and Scene map,
   then free the object.
   @param obj The pointer to the object to free.
**/
void Scene::DeleteObject( Object *obj ) {
  list.remove( obj );
  map.erase( obj->Name() );
  delete obj;
}

void Scene::DelObject( std::string objName ) {
  Object *obj = map[ objName ];
  DeleteObject( obj );
}

void Scene::DelObject( void ) {
  DeleteObject(*(list.begin()));
}

void Scene::PopObject( void ) {
  DeleteObject(*(--list.end()));
}

Object *Scene::Next( void ) {

  ++currentObj;
  if (currentObj == list.end())
    currentObj = list.begin();

  return *currentObj;

}

Object *Scene::Prev( void ) {

  if (currentObj == list.begin()) {
    currentObj = --list.end();
  } else {
    --currentObj;
  }

  return *currentObj;

}


Object *Scene::Active( void ) {
  return *currentObj;
}

void Scene::Draw( void ) {
  std::list< Object* >::iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    (*it)->Draw();
  }
}
