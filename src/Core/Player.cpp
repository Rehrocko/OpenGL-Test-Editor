#include "Player.h"
#include "../Util.hpp"
#include "../Core/Input.h"
#include "../EngineState.h"

Player::Player() {
	// For a blank class
}

Player::Player(glm::vec3 position, glm::vec3 rotation) {
	Respawn(position, rotation);
}

void Player::Update(float deltaTime) {

	// View Matrix ( DONT TOUCH ! )
	Transform cameraTransform;
	cameraTransform.position = _position /* Isometric view offset */ + birdseye;
	_viewPos = cameraTransform.position;
	//cameraTransform.rotation = _rotation;
	// There has to be a better way to do this
	//_viewMatrix = glm::inverse(cameraTransform.convert_to_mat4());
	_viewMatrix = glm::lookAt(cameraTransform.position, _position, _up);
	_inverseViewMatrix = glm::inverse(_viewMatrix);
	_right = glm::vec3(_inverseViewMatrix[0]);
	_up = glm::vec3(_inverseViewMatrix[1]);
	_forward = glm::vec3(_inverseViewMatrix[2]);
	_front = glm::normalize(glm::vec3(_forward.x, 0, _forward.z));

	// Handle Movement
	glm::vec3 displacement(0);
	if (EngineState::engineState == "PLAYER") {
		if (Input::KeyDown(GLFW_KEY_W)) {
			displacement -= glm::vec3(1, 0, 1);// *speed;
		}
		if (Input::KeyDown(GLFW_KEY_S)) {
			displacement += glm::vec3(1, 0, 1);// *speed;
		}
		if (Input::KeyDown(GLFW_KEY_A)) {
			displacement += glm::vec3(-1, 0, 1);// *speed;
		}
		if (Input::KeyDown(GLFW_KEY_D)) {
			displacement -= glm::vec3(-1, 0, 1);// *speed;
		}
	}
	MoveRelative(displacement * deltaTime * speed);
}

void Player::printPos() {
	std::cout << "X: " << _position.x << "Y: " << _position.y << "Z: " << _position.z << std::endl;
}

void Player::Respawn(glm::vec3 position, glm::vec3 rotation) {
	_position = position;
	_rotation = rotation;
}

void Player::MoveRelative(glm::vec3 displace) {
	_position = glm::vec3(_position.x + displace.x, _position.y + displace.y, _position.z + displace.z);
}




glm::mat4 Player::GetViewMatrix() {
	return _viewMatrix;
}

glm::mat4 Player::GetInverseViewMatrix() {
	return _inverseViewMatrix;
}

glm::vec3 Player::GetViewPos() {
	return _viewPos;
}

glm::vec3 Player::GetViewRotation() {
	return _rotation;
}

glm::vec3 Player::GetCameraRight() {
	return _right;
}

glm::vec3 Player::GetCameraForward() {
	return _forward;
}

glm::vec3 Player::GetCameraUp() {
	return _up;
}

glm::vec3 Player::GetPos() {
	return _position;
}