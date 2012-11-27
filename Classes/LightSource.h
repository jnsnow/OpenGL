/*****************************************
 * Lighting Class for OpenGL

 * Nick St.Pierre

 * November 20, 2012

  ****************************************/
#ifndef _LIGHTSOURCE
#define _LIGHTSOURCE

using namespace std;

#include <vector>

class LightSource
{

 private:
   point4   position( 0.0, 0.0, 0.0, 0.0 ) ;
   vec3    direction( 0.0, 0.0, 0.0 ) ;
   color4 light_ambient( 0.0, 0.0, 0.0, 1.0 );
   color4 light_diffuse( 0.0, 0.0, 0.0, 1.0 );
   color4 light_specular( 0.0, 0.0, 0.0, 1.0 );
   const unsigned int id;

 public:

   LightSource();
   LightSource(point4);
   LightSource(point4, color4, color4, color4);
   LightSource(point4, vec3, color4, color4, color4);
   LightSource(point4, vec3, color4, color4, color4, unsigned int);

   LightSource(const LightSource& orig);

   virtual ~LightSource();


   // Set/Get Member pairs
   void SetLight_specular(color4 light_specular);
   color4 GetLight_specular() const;

   void SetLight_diffuse(color4 light_diffuse);
   color4 GetLight_diffuse() const;

   void SetLight_ambient(color4 light_ambient);
   color4 GetLight_ambient() const;

   void SetDirection(vec3 direction);
   vec3 GetDirection() const;

   void SetPosition(vec4 z);
   vec4 GetPosition() const;

   void SetId(vec4 z);
   unsigned int GetId() const;


   // Effect Fxns/Modifiers
   void DoEvents(void);
   void setEffect(int);


   // Predicate Functions
   bool isDistant(); // check the homogenous coordinate. if zero, true.


};

// SINGLETON? YEAH DO IT.
class Lights
{

 private:

  vector<LightSource> theLights;



 public:

  void removeLightSource();
  void addLightSource();
  void init_lights(GLuint program);

};


#endif
