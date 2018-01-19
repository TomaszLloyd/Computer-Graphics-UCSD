/***************************************************************************
* This file defines functions that update array-type objects that should
* then be passed onto the OpenGL shader program(s) to render the familiar
* GLUT shapes in a modern OpenGL context.
* The GLUT shapes SHOULD NOT be used in modern OpenGL due to the use of
* deprecated (often invalid) functions. The shapes defined by GLUT are
* entirely inaccessible by the shaders due to the lack of buffer objects.
* Written in July 2016 by Hoang Tran. Extended September 2016 to support more
* shapes than just the teapot.
*
* DO NOT MODIFY THIS FILE WHEN DOING THE HOMEWORK ASSIGNMENT
***************************************************************************/
#ifndef _GLUT_SHAPES_REPLACEMENT
#define _GLUT_SHAPES_REPLACEMENT

// OSX systems require different headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
// Force glm to use radians since usage of degrees is deprecated
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// These buffer objects should only be used for shapes defined in this file.
// For user-defined objects, please make separate VAOs, VBOs, etc.
extern GLuint VAOs[3], VBOs[3], NBOs[3], EBOs[3];
enum {CUBE, SPHERE, TEAPOT};

// The default shaders used.
extern GLuint vertexshader, fragmentshader, shaderprogram;
// Default associated variables
extern GLuint modelviewPos;

// Other variables

void initBufferObjects();
void destroyBufferObjects();

// Functions used for rendering objects that would normally have been rendered by GLUT

/* TEAPOT */
// The teapot will be fed in as an OBJ file.
// See "Wavefront OBJ" file format on Wikipedia for more information.
extern std::vector <glm::vec3> teapotVertices;
extern std::vector <glm::vec3> teapotNormals;
extern std::vector <unsigned int> teapotIndices;

void initTeapot();
void solidTeapot(float);

/* CUBE */
void initCube();
void solidCube(float);

/* SPHERE */
void initSphere();
void solidSphere(float, int, int);
extern std::vector <glm::vec3> sphereVertices;
extern std::vector <glm::vec3> sphereNormals;
extern int prevStacks, prevSlices;

#endif
