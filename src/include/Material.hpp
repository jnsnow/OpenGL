#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "vec.hpp"

/**
 * This class contains the material information of an object. The elements
 * in this class are parsed from the .mtl file. The values stored are the
 * Ambient, specular, diffuse vectors, and the shininess and transparency
 * coefficients. Planned later is to hold the bump mappings.
 *
 * @author Michael Gorlin
 * @since 2013-04-16
 */
class Material {
public:
  Material();
  Material(Angel::vec3 ambient);
  Material(Angel::vec3 ambient, Angel::vec3 diffuse, Angel::vec3 specular, float shininess);
  Material(Angel::vec3 ambient, Angel::vec3 diffuse, Angel::vec3 specular, float shininess, float transparency);
  
	void setAmbient(Angel::vec3 ambient);
	void setSpecular(Angel::vec3 specular);
	void setDiffuse(Angel::vec3 diffuse);
	void setShininess(float shininess);
	void setTransparency(float transparency);
	void setRefract(float ior);
	
	const Angel::vec3 getAmbient() const;
	const Angel::vec3 getDiffuse() const;
	const Angel::vec3 getSpecular() const;

	float getShininess() const;
	float getTransparency() const;
	float getRefract() const;

	/**
	 * This function initializes the uniforms for the material in the shader. 
	 * Uniform the uniform names are:
	 *    vec3  mat_ambient
	 *    vec3  mat_diffuse
	 *    vec3  mat_specular
	 *    float mat_shininess
	 *    float mat_transparency
	 *
	 * @param shaderProgram The shader program that is going to be used to
	 * render the object.
	 */
	void initMaterial(GLuint shaderProgram);

private:
	Angel::vec3 ambient;
	Angel::vec3 specular;
	Angel::vec3 diffuse;
	
	float shininess;
	float transparency;
	float ior;

};

#endif
