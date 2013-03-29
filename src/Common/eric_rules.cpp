/*
 * eric_rules.cpp
 *
 *  Created on: Mar 29, 2013
 *      Author: Eric McCann
 */

#include <cstdio>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "platform.h"
#include "InitShader.hpp"
#include "eric_rules.hpp"


// http://stackoverflow.com/questions/236129/splitting-a-string-in-c
std::vector< std::string > &split( const std::string &s, char delim,
                                   std::vector< std::string > &elems ) {
  std::stringstream ss( s );
  std::string item;
  while ( std::getline( ss, item, delim ) ) {
    elems.push_back( item );
  }
  return elems;
}
std::vector< std::string > split( const std::string &s, char delim ) {
  std::vector< std::string > elems;
  return split( s, delim, elems );
}

namespace VooDoo
{
  /**
   * InitRelativePaths is a preparation step allowing files referred to relative to the executable to be referred to relative to the working dir AND the executable
   *
   * @param argc from main
   * @param argv from main
   *
   * @return NOTHING
   */
  void InitRelativePaths( int argc, char** argv ) {
    char *binloc = argv[0];

    std::vector< std::string > slapchop = split( std::string( binloc ), '/' );

    //slapchop[0..length-2] == path without executable name
    if ( slapchop.size() > 1 ) {
      std::stringstream cat;
      for ( int i = 0; i < slapchop.size() - 1; i++ )
        cat << slapchop[i] << "/";
      getRelativePath(cat.str().c_str());
    }
  }

  /**
   * Turns a binary-relative relative path into a working directory relative relative path.
   *
   * @praram the path
   *
   * @return the real path
   */
  const char *getRelativePath(const char *path )
  {
    static std::string dondeestalosshaders;
    if (dondeestalosshaders.size() == 0)
    {
      if (strlen(path) == 0)
      {
        char** fake = (char**)malloc(sizeof(char*));
        fake[0] = (char*)"";
        InitRelativePaths(-1, fake);
        free(fake);
      }
      else
      {
        dondeestalosshaders = std::string(path);
      }
    }
    std::stringstream wholepath;
    wholepath << dondeestalosshaders << path;
    return wholepath.str().c_str();
  }
}
