// Transform.cpp: implementation of the Transform class.


#include "Transform.h"
#include <stdio.h>

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
	
    const float radians = degrees * pi / 180.0;
	
    const glm::mat3 part1 = cos(radians) * glm::mat3(1.0f);

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

	glm::mat3 result = part1 + part2 + part3;
	
	return result;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
	const vec3 rotationAxis = glm::normalize(up);
	eye = Transform::rotate(degrees, rotationAxis) * eye;
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {

	const vec3 rotationAxis = glm::normalize( glm::cross(eye, up));

	eye = Transform::rotate(degrees, rotationAxis) * eye;
	up = glm::normalize( Transform::rotate( degrees, rotationAxis) * up );
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {

	const vec3 w = glm::normalize(eye);

	const vec3 u = glm::cross( glm::normalize(up), w );
	const vec3 v = glm::cross( w, u);

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
