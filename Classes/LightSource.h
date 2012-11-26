/*****************************************
 * Lighting Class for OpenGL

 * Nick St.Pierre

 * November 20, 2012

  ****************************************/
#ifndef _LIGHTSOURCE
#define _LIGHTSOURCE


class LightSource
{

 private:
   point4   position( 0.0, 0.0, 0.0, 0.0 ) ;
   vec3    direction( 0.0, 0.0, 0.0 ) ;
   color4 light_ambient( 0.0, 0.0, 0.0, 1.0 );
   color4 light_diffuse( 0.0, 0.0, 0.0, 1.0 );
   color4 light_specular( 0.0, 0.0, 0.0, 1.0 );


 public:

   LightSource();
   LightSource(const LightSource& orig);
   virtual ~LightSource();
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


};


class Lights
{

 private:

  






 public:

  void init_lights(GLuint program);

};




#endif
