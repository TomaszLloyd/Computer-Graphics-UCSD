/*********************************************************************
* This file provide implementations for the functions declared
* in Geometry.h
* It serves as an alternative to the now deprecated fixed-function
* pipeline that GLUT utilizes.
* Author: Hoang Tran 
* Written in July 2016.                                             
*********************************************************************/

#include "Geometry.h"

// Avoiding linker error caused by multiply defined variables
GLuint defaultVAO, defaultVBO, defaultNBO, defaultEBO;
GLuint vertexshader, fragmentshader, shaderprogram;
GLuint modelviewPos;
glm::mat4 model;
shape lastUsed = NONE;
/** TEAPOT RELATED **/
std::vector <glm::vec3> teapotVertices;
std::vector <glm::vec3> teapotNormals;
std::vector <unsigned int> teapotIndices;

// Initialize the buffer objects. Can only be called after OpenGL is initialized.
void initBufferObjects() {
	// Tell OpenGL to allocate us some space for the VAO
	glGenVertexArrays(1, &defaultVAO);
	// Now allocate some space for all the buffer objects
	glGenBuffers(1, &defaultVBO);
	glGenBuffers(1, &defaultNBO);
	glGenBuffers(1, &defaultEBO);
}

// Free up any dynamically allocated memory here
void destroyBufferObjects() {
	glDeleteVertexArrays(1, &defaultVAO);
	glDeleteBuffers(1, &defaultVBO);
	glDeleteBuffers(1, &defaultNBO);
	glDeleteBuffers(1, &defaultEBO);
}

// OBJ file parser func
void parse(const char * filepath) {
	FILE* fp;
	float x, y, z;
	int fx, fy, fz, ignore;
	int c1, c2;
	float minY = INFINITY, minZ = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY;

	fp = fopen(filepath, "rb");

	if (fp == NULL) {
		std::cerr << "Error loading file: " << filepath << std::endl;
		std::getchar();
		exit(-1);
	}

	while (!feof(fp)) {
		c1 = fgetc(fp);
		while (!(c1 == 'v' || c1 == 'f')) {
			c1 = fgetc(fp);
			if (feof(fp))
				break;
		}
		c2 = fgetc(fp);

		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			teapotVertices.push_back(glm::vec3(x, y, z));
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			teapotNormals.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if (c1 == 'f')
		{
			fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
			teapotIndices.push_back(fx - 1);
			teapotIndices.push_back(fy - 1);
			teapotIndices.push_back(fz - 1);
		}
	}

	fclose(fp); // Finished parsing
	// Recenter the teapot
	float avgY = (minY + maxY) / 2.0f - 0.0234f; // Closest possible approximation
	float avgZ = (minZ + maxZ) / 2.0f;
	for (unsigned int i = 0; i < teapotVertices.size(); ++i) {
		glm::vec3 shiftedVertex = (teapotVertices[i] - glm::vec3(0.0f, avgY, avgZ)) * glm::vec3(0.975f, 0.975f, 0.975f);
		teapotVertices[i] = shiftedVertex;
	}
}

void bindTeapot() {
	glBindVertexArray(defaultVAO);

	glBindBuffer(GL_ARRAY_BUFFER, defaultVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * teapotVertices.size(), &teapotVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, defaultNBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * teapotNormals.size(), &teapotNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, defaultEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * teapotIndices.size(), &teapotIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	lastUsed = TEAPOT;
}

void solidTeapot(float size) {
	model = glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size));
	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(view * model)[0][0]);

	if (lastUsed != TEAPOT) {
		bindTeapot();
	}

	glBindVertexArray(defaultVAO);
	glDrawElements(GL_TRIANGLES, teapotIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // Unbind the VAO when done
}
