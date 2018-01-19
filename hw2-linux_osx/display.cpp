/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
// OSX systems require different headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "Transform.h"
#include "Geometry.h"

using namespace std ; 
#include "variables.h"
#include "readfile.h"

// To be used as a matrix stack for the modelview.
std::vector <glm::mat4> modelviewStack;

// New helper transformation function to transform vector by modelview 
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.  
// Some of you may want to use the more modern routines in readfile.cpp 
// that can also be used.  
void transformvec (const GLfloat input[4], GLfloat output[4]) 
{
  glm::vec4 inputvec(input[0], input[1], input[2], input[3]);
  glm::vec4 outputvec = modelview * inputvec;
  output[0] = outputvec[0];
  output[1] = outputvec[1];
  output[2] = outputvec[2];
  output[3] = outputvec[3];
}

void pushMatrix(glm::mat4 mat) {
  modelviewStack.push_back(glm::mat4(mat));
}

void popMatrix(glm::mat4& mat) {
  if( modelviewStack.size()) {
    mat = glm::mat4(modelviewStack.back());
    modelviewStack.pop_back();
  } else {
    mat = glm::mat4(1.0f);
  }
}

void display() 
{
  glClearColor(0, 0, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  // Either use the built-in lookAt function or the lookAt implemented by the user.
  if (useGlu) {
    modelview = glm::lookAt(eye,center,up); 
  } else {
    modelview = Transform::lookAt(eye,center,up); 
  }

  // Lights are transformed by current modelview matrix. 
  // The shader can't do this globally. 
  // So we need to do so manually.  
  // glLoadMatrixf(&modelview[0][0]);
  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);
  
  if (numused) {
    glUniform1i(enablelighting,true);

    // YOUR CODE FOR HW 2 HERE.  
    // You need to pass the light positions and colors to the shader. 
    // glUniform4fv() and similar functions will be useful. See FAQ for help with these functions.
    // The lightransf[] array in variables.h and transformvec() might also be useful here.
    // Remember that light positions must be transformed by modelview.  

    for (int i = 0; i < numused; i++) {
      
			GLfloat orig[4] = { 
          lightposn[(i * 4)], 
          lightposn[(i * 4) + 1], 
          lightposn[(i * 4) + 2], 
          lightposn[(i * 4) + 3] 
        };
			GLfloat trans[4];

			transformvec(orig, trans);

			lightransf[(i * 4)] = trans[0];
			lightransf[(i * 4) + 1] = trans[1];
			lightransf[(i * 4) + 2] = trans[2];
			lightransf[(i * 4) + 3] = trans[3];
		}

		glUniform1i(numusedcol, numused);
		glUniform4fv(lightpos, numused, lightransf);
		glUniform4fv(lightcol, numused, lightcolor);

  } else {
    glUniform1i(enablelighting,false); 
  }

  // Transformations for objects, involving translation and scaling 
  mat4 sc(1.0) , tr(1.0), transf(1.0); 
  sc = Transform::scale(sx,sy,1.0); 
  tr = glm::transpose( Transform::translate(tx,ty,0.0) ); 

  // YOUR CODE FOR HW 2 HERE.  
  // You need to use scale, translate and modelview to 
  // set up the net transformation matrix for the objects.  
  // Account for GLM issues, matrix order, etc.  
  
  // all of my transforms are in column major order so
  // we multiply it this way.
  transf = modelview * tr * sc;

  // The object draw functions will need to further modify the top of the stack,
  // so assign whatever transformation matrix you intend to work with to modelview
  // rather than use a uniform variable for that.
  
  modelview = transf;
  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &transf[0][0]);

  for (int i = 0 ; i < numobjects ; i++) {
    object* obj = &(objects[i]); // Grabs an object struct.

    // YOUR CODE FOR HW 2 HERE. 
    // Set up the object transformations 
    // And pass in the appropriate material properties
    // Again glUniform() related functions will be useful

        
    pushMatrix( transf);

    mat4 objtransform = transf * obj->transform;
    modelview = objtransform;

		glUniform4fv(ambientcol, 1, obj->ambient);
		glUniform4fv(diffusecol, 1, obj->diffuse);
		glUniform4fv(specularcol, 1, obj->specular);
		glUniform4fv(emissioncol, 1, obj->emission);
		glUniform1fv(shininesscol, 1, &obj->shininess);

    glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &objtransform[0][0]);

    // Actually draw the object
    // We provide the actual drawing functions for you.  
    // Remember that obj->type is notation for accessing struct fields
    
    if (obj->type == cube) {
      solidCube(obj->size); 
    }
    else if (obj->type == sphere) {
      const int tessel = 20; 
      solidSphere(obj->size, tessel, tessel); 
    }
    else if (obj->type == teapot) {
      solidTeapot(obj->size); 
    }
    popMatrix( transf);
    
  }
  glutSwapBuffers();
  // glFlush();
}
