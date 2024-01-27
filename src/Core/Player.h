#pragma once
#include "../Common.h"

class Player {
	public:
		Player();
		Player(glm::vec3 position, glm::vec3 rotation);

		void Update(float deltaTime);
		void Respawn(glm::vec3 position, glm::vec3 rotation);
		glm::mat4 GetViewMatrix();
		glm::vec3 GetViewPos();
		glm::vec3 GetViewRotation();
		glm::vec3 GetCameraRight();
		glm::vec3 GetCameraForward();
		glm::vec3 GetCameraUp();
	private:
		glm::vec3 _position = glm::vec3(0, 0, 0);
		glm::vec3 _rotation = glm::vec3(0, 0, 0);
		float speed = 5.0f;
		glm::mat4 viewMatrix = glm::mat4(1);
		glm::mat4 inverseViewMatrix = glm::mat4(1);
		glm::vec3 _front = glm::vec3(0);
		glm::vec3 _forward = glm::vec3(0);
		glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 _right = glm::vec3(0);
		glm::vec3 birdseye = glm::vec3(10, 10, 10);
};