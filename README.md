HSC
===

- This is our Graphics I final project for Prof. Grinstein,
  at UMass Lowell, Fall 2012.

- This project utilizes a lot of code snippets, libraries and ideas from
  a variety of authors. They are credited in-line where appropriate, but here's a list:
  
  - Etay Mieri, olgdev.atspace.co.uk:
    	 Invaluable tutorials on OpenGL.  

  - Daniel Beard, Paul Boxley and Paul Martz:
    	   Information and code for generating random terrain.
	   The code we eventually implemented is heavily modified
	   from Daniel Beard's implementation, using Boxley and Martz's 
	   writeups as guidelines.

  - Ed Angel:
      Author of "Interactive Computer Graphics: A Top-Down Approach,"
      Provided the basis of our executable's main skeleton, 
      and 99.9% of our vec and mat classes, from his book's code.

  - Gabriele Randelli:
      Lead developer of the WiiC/PP library, which itself has been
      developed out of the (now defunct) WiiUse library, authored by
      Michael Laforest.


- To build the project, pushd Flythrough && make;
  To run, pushd Flythrough && ./fly;

- Controls (as of Mon 03 Dec 2012)
  w,a,s,d: Forward, back, strafe left and right (As in most commercial FPS games)
  mouse: Look around.
  click and move mouse: Rotate left or right.
  
  1-7, 0: Control rotating light distance,
  m: change light mode
  n: change light position

  z,x,c,v,b: Various perspective modes.

  +,-: Add or remove cameras.

- wiic/pp library utilizes the LGPL license,
  The source code is included in our source tree here for convenience,
  with minor alterations.