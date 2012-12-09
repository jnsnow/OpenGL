#include <string>
#include <map>
#include <list>

#include "platform.h" // for GLuint
#include "Object.hpp"

class Scene {

 public:
  Scene( );
  ~Scene();

  void SetShader( GLuint gShader );

  Object *AddObject( std::string objName );
  void DelObject( std::string objName );
  void DelObject( void );
  void PopObject( void );

  Object *Next( void );
  Object *Prev( void );
  Object *Active( void );
  
  void Draw( void );

 private:
  std::map< std::string, Object* > map;
  std::list< Object* > list;
  std::list< Object* >::iterator currentObj;
  GLuint gShader;

  /* Copies are forbidden right now. */
  Scene( Scene &copy );
  Scene &operator=( const Scene &copy );

  /* Internal use */
  void DeleteObject( Object *obj );

};
