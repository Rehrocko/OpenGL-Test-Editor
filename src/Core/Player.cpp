#include "Player.h"
#include "../Util.hpp"

Player::Player() {
	// For a blank class
}

Player::Player(glm::vec3 position, glm::vec3 rotation) {
	Respawn(position, rotation);
}

void Player::Update(float deltaTime) {
	Transform cameraTransform;
	cameraTransform.position = _position /* Isometric view offset */ + birdseye;
	// There has to be a better way to do this.
	cameraTransform.rotation = glm::eulerAngles(glm::quat_cast(glm::lookAt(_position + birdseye, _position, _up)));
	viewMatrix = glm::inverse(cameraTransform.convert_to_mat4());
	inverseViewMatrix = glm::inverse(viewMatrix);
	_right = glm::vec3(inverseViewMatrix[0]);
	_up = glm::vec3(inverseViewMatrix[1]);
	_forward = glm::vec3(inverseViewMatrix[2]);
	_front = glm::normalize(glm::vec3(_forward.x, 0, _forward.z));
}

void Player::Respawn(glm::vec3 position, glm::vec3 rotation) {
	_position = position;
	_rotation = rotation;
}