#pragma once

#pragma warning(push, 0)
// Includes any common variables, give a quick glance and you will understand

//Standard Libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Math.h"
#pragma warning(pop)

#define PI 3.141592653589793f

#define NEAR_PLANE 0.005f
#define FAR_PLANE  50.0f

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#define BLACK   glm::vec3(0,0,0)
#define WHITE   glm::vec3(1,1,1)
#define RED     glm::vec3(1,0,0)
#define GREEN   glm::vec3(0,1,0)
#define BLUE    glm::vec3(0,0,1)
#define YELLOW  glm::vec3(1,1,0)
#define PURPLE  glm::vec3(1,0,1)
#define GREY    glm::vec3(0.25f)

// Some structuring for wide use

struct Transform {
	// Ah, just like Unity
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 scale    = glm::vec3(1, 1, 1);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::mat4 convert_to_mat4() {
		glm::mat4 m = glm::translate(glm::mat4(1), position);
		m *= glm::mat4_cast(glm::quat(rotation));
		m = glm::scale(m, scale);
		return m;
	}
};

struct Triangle {
	glm::vec3 point1 = glm::vec3(0, 0, 0);
	glm::vec3 point2 = glm::vec3(0, 0, 0);
	glm::vec3 point3 = glm::vec3(0, 0, 0);
	glm::vec3 normal = glm::vec3(0, 0, 0);
	glm::vec3 color  = glm::vec3(0, 0, 0);
};