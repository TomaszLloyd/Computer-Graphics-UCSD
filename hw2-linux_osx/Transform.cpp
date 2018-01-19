// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
  // mat3 ret;
  // YOUR CODE FOR HW2 HERE
  // Please implement this.  Likely the same as in HW 1.  
  // return ret;

  // convert degrees to radians
	const float radians = degrees * pi / 180.0;

	// create 3x3 identity matrix 
	glm::mat3 identity(1.0f);

	// there are three terms to the Rodrigues Rotation Formula
	// and we'll break them up here into their separate parts
	// and add them back up at the end
	const glm::mat3 part1 = cos(radians) * identity;
	
	// this is the tricky one, make sure the variables
	// are in column-major order!
	/**
	 *	| a b c | 
	 * 	| d e f |
	 * 	| g h i |
	 * 
	 * should be input as mat3( a, d, g, b, e, h, c, f, i);
	 * 
	 */	

	const glm::mat3 part2 = (1 - cos(radians)) * glm::mat3(
		axis.x * axis.x , 	axis.x * axis.y,	axis.x * axis.z,
		axis.x * axis.y , 	axis.y * axis.y,	axis.y * axis.z,
		axis.x * axis.z , 	axis.y * axis.z,	axis.z * axis.z
	);

	const glm::mat3 part3 = sin(radians) * glm::mat3(
		0,			axis.z, 	-axis.y,
		-axis.z,	0, 			axis.x,
		axis.y,		-axis.x, 	0
	); 

	// add up the three terms and return the result
	glm::mat3 result = part1 + part2 + part3;
	
	// You will change this return call
	return result;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.  


	// set the rotation axis to a normalized Y axis as the up vector
	// const vec3 rotationAxis = glm::normalize(up);
	
	// move the eye vector to a vector rotated about the Y axis,
	// then multiply by the original eye vector to get the 
	// correct distance from the teapot
	// eye = Transform::rotate(degrees, rotationAxis) * eye;
	eye = rotate(degrees, up) * eye;
	// up vector doesn't need to change at all since we rotate about it
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE 
  // Likely the same as in HW 1.  

  // set the rotation axis to a normalized right vector (eye _cross_ up is right vector)
	const vec3 rotationAxis = glm::normalize( glm::cross(eye, up));
	
	// move the eye vector around our rotation axis then multiply by original
	// to get correct distance from the teapot
	eye = Transform::rotate(degrees, rotationAxis) * eye;

	// our up axis has changed after rotating
	// let's reset the up vector 
	up = glm::normalize( Transform::rotate( degrees, rotationAxis) * up );

}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  // mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.  
  // return ret;

	// lets define the center for readiability
	// const vec3 center = vec3(0.0, 0.0, 0.0);

	// get the vector `a` which points from the `eye`, to the center of the object
	// in this case the object lies at the origin
	// and the camera always faces (0,0,-1)
	const vec3 w = glm::normalize(eye - center);
	// but more simply, since we know everything is at 0,0,0, we can just take the eye coordinates

	//  get length of `a` divided by it's magnitude
	// const vec3 w = eye / glm::length( eye );
	// const vec3 w = glm::normalize(a);

	// u = ( b x w / magnitude ( b x w ) )
	// we should first make `b` a unit vector by normalizing it
	const vec3 b = up / glm::length(up);
	// then normalize the cross product of `b` x `w`
	const vec3 u = glm::cross( b, w ) / glm::length( glm::cross( b, w ) ) ;
	// alternatively, since `w` is already a unit vector, we can get the
	// cross product of `w` and the normalized `up` vector in one line
	// const vec3 u = glm::cross( w, glm::normalize(up) );

	// lastly, to find v, we get the crossproduct of `w` x `u`
	const vec3 v = glm::cross( w, u);

	// putting it all together in column-major order
	const glm::mat4 result = glm::mat4(
		u.x, v.x, w.x, 0.0,
		u.y, v.y, w.y, 0.0,
		u.z, v.z, w.z, 0.0,
		-u.x * eye.x - u.y * eye.y - u.z * eye.z, 
		-v.x * eye.x - v.y * eye.y - v.z * eye.z, 
		-w.x * eye.x - w.y * eye.y - w.z * eye.z, 
		1.0
	);
	
	return result;
	
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  // mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.  
  // return ret;

  	float yScale = 1.0F / tan(glm::radians(fovy) * 0.5);
	float xScale = yScale / aspect;
	float range = zNear - zFar;
	float a = ((zNear + zFar) / range);
	float b = (2 * zNear * zFar) / range;

	mat4 result = mat4(
		xScale, 0.0, 0.0, 0.0,
		0.0, yScale, 0.0, 0.0,
		0.0, 0.0, a, -1.0,
		0.0, 0.0, b, 0.0);

	return result;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  // mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Implement scaling 
  // return ret;

  // We essentially multiply an identity matrix
  // by a vector4 of (scalex, scaley, scalez, 1.0)
  // or more simply:
  	mat4 result = mat4(
		sx, 0.0, 0.0, 0.0,
			0.0, sy, 0.0, 0.0,
			0.0, 0.0, sz, 0.0,
			0.0, 0.0, 0.0, 1.0
	);

	return result;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
	// mat4 ret;
	// YOUR CODE FOR HW2 HERE
	// Implement translation 
	// return ret;

	// in our mat4, we use the last column for translation
	// but OpenGL is NOT row-major
  	mat4 result = mat4(
		1.0, 0.0, 0.0, tx,
		0.0, 1.0, 0.0, ty,
		0.0, 0.0, 1.0, tz,
		0.0, 0.0, 0.0, 1.0
	);

	return result;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
