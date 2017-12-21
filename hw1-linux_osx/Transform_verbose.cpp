// Transform.cpp: implementation of the Transform class.


#include "Transform.h"
#include <stdio.h>

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
	// YOUR CODE FOR HW1 HERE

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

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE
	
	// set the rotation axis to a normalized Y axis as the up vector
	const vec3 rotationAxis = glm::normalize(up);
	
	// move the eye vector to a vector rotated about the Y axis,
	// then multiply by the original eye vector to get the 
	// correct distance from the teapot
	eye = Transform::rotate(degrees, rotationAxis) * eye;
	// up vector doesn't need to change at all since we rotate about it
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE 

	// set the rotation axis to a normalized right vector (eye _cross_ up is right vector)
	const vec3 rotationAxis = glm::normalize( glm::cross(eye, up));
	
	// move the eye vector around our rotation axis then multiply by original
	// to get correct distance from the teapot
	eye = Transform::rotate(degrees, rotationAxis) * eye;

	// our up axis has changed after rotating
	// let's reset the up vector 
	up = glm::normalize( Transform::rotate( degrees, rotationAxis) * up );
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
	// YOUR CODE FOR HW1 HERE

	// lets define the center for readiability
	const vec3 center = vec3(0.0, 0.0, 0.0);

	// get the vector `a` which points from the `eye`, to the center of the object
	// in this case the object lies at the origin
	// and the camera always faces (0,0,-1)
	// const vec3 a = vec3( eye.x - center.x , eye.y - center.y, eye.z - center.z );
	// but more simply, since we know everything is at 0,0,0, we can just take the eye coordinates

	//  get length of `a` divided by it's magnitude
	// const vec3 w = eye / glm::length( eye );
	const vec3 w = glm::normalize(eye);

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

Transform::Transform()
{

}

Transform::~Transform()
{

}
